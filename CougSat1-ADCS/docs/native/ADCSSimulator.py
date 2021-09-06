from argparse import ArgumentParser
from typing import Tuple
import colorama
from colorama import Fore
import datetime
import matplotlib.pyplot as pyplot
from multiprocessing import Pool, cpu_count
import numpy as np
from numpy.ma.core import power
import quaternion
import sys

colorama.init(autoreset=True)

# kg mm^2
Ixx = 1754.776
Ixy = 4.906
Ixz = 8.896
Iyy = 1788.534
Iyz = 10.155
Izz = 1758.399
# Ixx = 1754
# Ixy = 0.0
# Ixz = 0.0
# Iyy = Ixx
# Iyz = 0.0
# Izz = Ixx
iBody = np.array([[Ixx, Ixy, Ixz],
                  [Ixy, Iyy, Iyz],
                  [Ixz, Iyz, Izz]]) / 1e6
iBodyInv = np.linalg.inv(iBody)

magFieldStrength = 3.5e-5

coilR = 17.808406
coilL = 0.0112
coilN = 200
coilA = 68e-3 * 62e-3

rCamera = np.array([[0, 0, -1]]).T

debugVectorLocal = True

class ADCS:
  def __init__(self, t: float) -> None:
    '''!@brief Initialize ADCS control algorithm
    '''
    self.lastT = t

  def compute(self, t: float) -> Tuple[np.ndarray, np.ndarray]:
    '''!@brief Compute ADCS loop given input from sensors

    @param t Current time
    @return tuple[np.ndarray, np.ndarray] Coil duty cycle, debug vector
      Coil duty cycle, shape=(3) Duty cycle written to PWM output
      Debug vector, shape=(3, 1) Vector to plot for debug purposes, None will not plot
        debugVectorLocal: True assumes vector is in local reference frame, False is global
    '''
    dCoil = np.array([1.0, 0.0, 0.0])

    debugVector = np.array([[0.5, 0, 0]]).T

    return dCoil, debugVector

def magnetorquer(N, cog: np.ndarray, center: np.ndarray,
                 edge1: np.ndarray, edge2: np.ndarray) -> Tuple[np.ndarray, np.ndarray]:
  '''!@brief Create a magnetorquer for number of loops and geometry

  @param N Number of wire loops
  @param cog Location of center of gravity
  @param center Location of loop center
  @param edge1 Vector from center to an edge
  @param edge1 Vector from center to other edge
  @return Tuple[np.array, np.array] (rCoil, iCoil)
    rCoil is vectors to wire segment center
    lCoil is vectors of wire segments
  '''
  rCoil = np.array([center + edge1, center - edge1,
                    center + edge2, center + edge2]).T
  rCoil = rCoil - np.repeat(cog, 4, axis=1)
  lCoil = N * 2 * np.array([edge2, -edge2, -edge1, edge1]).T
  return rCoil, lCoil

class Satellite:

  def __init__(self, initialOmega: float) -> None:
    '''!@brief Create a Satellite simulation

    @param initialOmega Initial angular momentum with magnitude gaussian(mu=initialOmega, sigma=initialOmega)
    '''
    self.batteryVoltage = np.random.uniform(3.3, 4.2)

    self.vCoil = np.zeros(3)
    self.iCoil = np.zeros(3)

    self.target = np.random.uniform(-1, 1, size=(3, 1))
    self.target = self.target / np.linalg.norm(self.target)

    self.magFieldU = np.random.uniform(-1, 1, size=(3, 1))
    self.magFieldU = self.magFieldU / np.linalg.norm(self.magFieldU)
    self.magField = self.magFieldU * magFieldStrength

    self.debugVector = np.zeros((3, 1))

    # State variables
    self.q = quaternion.from_float_array(np.random.uniform(-1, 1, 4))
    self.q = self.q / abs(self.q)

    omega = np.random.uniform(-1, 1, size=(3, 1))
    omega = omega / np.linalg.norm(omega)
    omega = omega * np.random.normal(initialOmega, initialOmega)
    self.l = iBody @ omega

    self.loopFreq = 10

    self.t = []
    self.tStepList = []
    self.omegaList = []
    self.qList = []
    self.rList = []
    self.rInvList = []
    self.mList = []
    self.torqueList = []
    self.cameraList = []
    self.angleErrorList = []
    self.iCoilList = []
    self.vCoilList = []
    self.pCoilList = []
    self.wCoilList = []
    self.debugVectorList = []

    self.minTStep = 1 / (self.loopFreq * 100)
    self.maxOmega = 100
    self.maxAcceleration = np.deg2rad(50) / 1
    self.maxTorque = np.max(
      iBody @ np.repeat([[self.maxAcceleration]], 3, axis=0))
    self.maxAngleStep = self.maxOmega * self.minTStep
    self.maxLStep = self.maxTorque * self.minTStep
    self.minTStepReached = False

    self.omegaThreshold = np.deg2rad(1) / 1
    self.angleThreshold = np.deg2rad(1)
    self.currentThreshold = 1e-3

    self.converged = False

  def __solveODE(self, duration: float) -> float:
    '''!@brief Solve ODE at current point up to duration in the future

    @param duration Max time to increment up to
    @return float actual time step
    '''
    # Calculate state vector and derivative
    r = quaternion.as_rotation_matrix(self.q)
    rInv = np.linalg.inv(r)
    iInv = r @ iBodyInv @ r.T

    self.magFieldLocal = rInv @ self.magField

    mLocal = np.zeros(3)
    mLocal += coilN * self.iCoil[0] * np.array([coilA, 0, 0])
    mLocal += coilN * self.iCoil[1] * np.array([0, coilA, 0])
    mLocal += coilN * self.iCoil[2] * np.array([0, 0, coilA])
    m = r @ mLocal
    torque = np.cross(m, self.magField.reshape(3)).reshape((3, 1))
    m = m.reshape((3, 1))

    pCoil = sum(self.vCoil * self.iCoil)

    omega = iInv @ self.l
    omegaQ = quaternion.from_vector_part(omega, 0)[0]
    qDot = 0.5 * omegaQ * self.q

    # Determine appropriate tStep
    tStep = duration

    torqueAbs = np.linalg.norm(torque)
    if torqueAbs != 0:
      suggestTStep = self.maxLStep / torqueAbs
      tStep = min(tStep, suggestTStep)
      # if suggestTStep == tStep:
      #   print("torque")
      if tStep < self.minTStep:
        tStep = self.minTStep
        if not self.minTStepReached:
          print(
            f'{Fore.RED} tStep has reached minimum value with torque = {torqueAbs} Nm')
          self.minTStepReached = True
    omegaQAbs = abs(omegaQ)
    if omegaQAbs != 0:
      suggestTStep = self.maxAngleStep / omegaQAbs
      tStep = min(tStep, suggestTStep)
      # if suggestTStep == tStep:
      #   print("omega")
      if tStep < self.minTStep:
        tStep = self.minTStep
        if not self.minTStepReached:
          print(
            f'{Fore.RED} tStep has reached minimum value with omega = {omegaQAbs} rad/s')
          self.minTStepReached = True

    # Euler's method
    self.l = self.l + torque * tStep
    self.q = self.q + qDot * tStep
    self.q = self.q / abs(self.q)

    # Replace with exp
    iCoilLimit = self.vCoil / coilR
    expFactor = np.exp(-tStep * coilR / coilL)
    self.iCoil = self.iCoil * expFactor + iCoilLimit * (1 - expFactor)

    self.omegaList.append(omega)
    self.qList.append(self.q)
    self.rList.append(r)
    self.rInvList.append(rInv)
    self.mList.append(m)
    self.torqueList.append(torque)
    self.iCoilList.append(self.iCoil)
    self.vCoilList.append(self.vCoil)
    self.pCoilList.append(pCoil)
    self.wCoilList.append(pCoil * tStep)
    self.debugVectorList.append(self.debugVector)

    # Calculate residuals
    camera = r @ rCamera
    self.cameraList.append(camera)

    cameraNorm = camera / np.linalg.norm(camera)
    dotP = np.sum(cameraNorm * self.target)
    angleError = np.arccos(np.clip(dotP, -1.0, 1.0))
    self.angleErrorList.append(angleError)

    currentAbs = np.linalg.norm(self.iCoil)

    if omegaQAbs > self.omegaThreshold:
      self.converged = False
    if angleError > self.angleThreshold:
      self.converged = False
    if currentAbs > self.currentThreshold:
      self.converged = False

    return tStep

  def run(self, durationMax: float = 180) -> dict:
    '''!@brief Run simulation until residuals reach zero or max duration

    @param durationMax Maximum duration to run before declaring system does not converge
    @return dict Collection of success parameters
    '''
    tStep = 1 / self.loopFreq
    t = self.__solveODE(tStep)
    self.t.append(t)
    self.tStepList.append(t)
    
    adcs = ADCS(t)

    n = int(np.ceil(durationMax / tStep))

    for i in range(1, n):
      tTarget = tStep * i
      tRemaining = tTarget - t
      while t < tTarget and tRemaining > self.minTStep:  # TODO add a keyboardinterrupt
        dt = self.__solveODE(tRemaining)
        t += dt
        self.t.append(t)
        self.tStepList.append(dt)
        tRemaining = tTarget - t

      dCoil, self.debugVector = adcs.compute(t)
      self.vCoil = dCoil * self.batteryVoltage
      if self.converged:
        break

    results = {
      'converged': self.converged,
      'timeToConvergence': t,
      'averagePower': np.average(self.pCoilList),
      'totalEnergy': sum(self.wCoilList)
    }
    return results

  def plot(self) -> None:
    '''!@brief Plot simulation
    '''
    # Global coordinates
    ax = pyplot.figure().add_subplot(projection='3d')

    rList = np.array(self.rList)
    rInvList = np.array(self.rInvList)
    m = np.array(self.mList)
    torque = np.array(self.torqueList)
    camera = np.array(self.cameraList)
    omega = np.array(self.omegaList)
    iCoil = np.array(self.iCoilList)
    vCoil = np.array(self.vCoilList)
    debugV = np.array(self.debugVectorList)

    maxM = np.max(np.linalg.norm(m, axis=1))
    m = m / maxM

    maxTorque = np.max(np.linalg.norm(torque, axis=1))
    torque = torque / maxTorque

    maxDebug = np.max(np.linalg.norm(debugV, axis=1))
    debugV = debugV / maxDebug

    if debugVectorLocal:
      debugVLocal = debugV
      debugVGlobal = []
      for i in range(len(rList)):
        debugVGlobal.append(rList[i] @ debugV[i])
      debugVGlobal = np.array(debugVGlobal)
    else:
      debugVLocal = []
      debugVGlobal = debugV
      for i in range(len(rInvList)):
        debugVLocal.append(rInvList[i] @ debugV[i])
      debugVLocal = np.array(debugVLocal)

    ax.quiver(
        [0], [0], [0], [
            self.target[0]], [
            self.target[1]], [
            self.target[2]], colors='y', label='Target')

    ax.quiver(
        [0], [0], [0], [
            self.magFieldU[0]], [
            self.magFieldU[1]], [
            self.magFieldU[2]], colors='k', label='Magnetic Field')

    x = camera[:, 0, 0]
    y = camera[:, 1, 0]
    z = camera[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]],
              [z[-1]], colors='m', label='Camera')
    ax.plot3D(x, y, z, 'm')

    x = m[:, 0, 0]
    y = m[:, 1, 0]
    z = m[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]],
              [z[-1]], colors='orange', label='Magnetic Dipole')
    ax.plot3D(x, y, z, 'orange', alpha=0.5)

    x = torque[:, 0, 0]
    y = torque[:, 1, 0]
    z = torque[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]],
              [z[-1]], colors='c', label='Torque')
    ax.plot3D(x, y, z, 'c', alpha=0.5)

    x = debugVGlobal[:, 0, 0]
    y = debugVGlobal[:, 1, 0]
    z = debugVGlobal[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]],
              [z[-1]], colors='violet', label='Debug Vector')
    ax.plot3D(x, y, z, 'violet', alpha=0.5)


    labels = ['x', 'y', 'z']
    colors = ['r', 'g', 'b']
    for i in range(3):
      x = rList[:, 0, i] * 0.5
      y = rList[:, 1, i] * 0.5
      z = rList[:, 2, i] * 0.5
      ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]],
                colors=colors[i], label=labels[i])
      ax.plot3D(x, y, z, colors[i], alpha=0.2)

    ax.set_xlim(-1, 1)
    ax.set_ylim(-1, 1)
    ax.set_zlim(-1, 1)
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')
    ax.legend()
    ax.set_title('Global')

    # Local coordinates
    ax = pyplot.figure().add_subplot(projection='3d')

    targetLocal = []
    magFieldULocal = []
    mLocal = []
    torqueLocal = []
    for i in range(len(rInvList)):
      targetLocal.append(rInvList[i] @ self.target)
      magFieldULocal.append(rInvList[i] @ self.magFieldU)
      mLocal.append(rInvList[i] @ m[i])
      torqueLocal.append(rInvList[i] @ torque[i])
    targetLocal = np.array(targetLocal)
    magFieldULocal = np.array(magFieldULocal)
    mLocal = np.array(mLocal)
    torqueLocal = np.array(torqueLocal)

    x = targetLocal[:, 0, 0]
    y = targetLocal[:, 1, 0]
    z = targetLocal[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]],
              [z[-1]], colors='y', label='Target')
    ax.plot3D(x, y, z, 'y')

    x = magFieldULocal[:, 0, 0]
    y = magFieldULocal[:, 1, 0]
    z = magFieldULocal[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]],
              [z[-1]], colors='k', label='Magnetic Field')
    ax.plot3D(x, y, z, 'k', alpha=0.5)

    ax.quiver(
        [0], [0], [0], [
            rCamera[0]], [
            rCamera[1]], [
            rCamera[2]], colors='m', label='Camera')

    x = mLocal[:, 0, 0]
    y = mLocal[:, 1, 0]
    z = mLocal[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]],
              [z[-1]], colors='orange', label='Magnetic Dipole')
    ax.plot3D(x, y, z, 'orange', alpha=0.5)

    x = torqueLocal[:, 0, 0]
    y = torqueLocal[:, 1, 0]
    z = torqueLocal[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]],
              [z[-1]], colors='c', label='Torque')
    ax.plot3D(x, y, z, 'c', alpha=0.5)

    x = debugVLocal[:, 0, 0]
    y = debugVLocal[:, 1, 0]
    z = debugVLocal[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]],
              [z[-1]], colors='violet', label='Debug Vector')
    ax.plot3D(x, y, z, 'violet', alpha=0.5)

    ax.quiver(
        [0],
        [0],
        [0],
        [0.5],
        [0],
        [0],
        colors=colors[0],
        label=labels[0],
        alpha=0.2)
    ax.quiver(
        [0],
        [0],
        [0],
        [0],
        [0.5],
        [0],
        colors=colors[1],
        label=labels[1],
        alpha=0.2)
    ax.quiver(
        [0],
        [0],
        [0],
        [0],
        [0],
        [0.5],
        colors=colors[2],
        label=labels[2],
        alpha=0.2)

    ax.set_xlim(-1, 1)
    ax.set_ylim(-1, 1)
    ax.set_zlim(-1, 1)
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')
    ax.legend()
    ax.set_title('Local')

    # Time domain
    _, subplots = pyplot.subplots(6, 1, sharex=True)

    x = omega[:, 0, 0]
    y = omega[:, 1, 0]
    z = omega[:, 2, 0]
    subplots[0].plot(self.t, x, 'r')
    subplots[0].plot(self.t, y, 'g')
    subplots[0].plot(self.t, z, 'b')
    subplots[0].set_title('Omega')
    subplots[0].axhline(y=0, color='k')

    x = iCoil[:, 0]
    y = iCoil[:, 1]
    z = iCoil[:, 2]
    subplots[1].plot(self.t, x, 'r')
    subplots[1].plot(self.t, y, 'g')
    subplots[1].plot(self.t, z, 'b')
    subplots[1].set_title('Coil Current')
    subplots[1].axhline(y=0, color='k')

    x = vCoil[:, 0]
    y = vCoil[:, 1]
    z = vCoil[:, 2]
    subplots[2].plot(self.t, x, 'r')
    subplots[2].plot(self.t, y, 'g')
    subplots[2].plot(self.t, z, 'b')
    subplots[2].set_title('Coil Voltage')
    subplots[2].axhline(y=0, color='k')

    subplots[3].plot(self.t, self.pCoilList, 'c')
    subplots[3].set_title('Coil power')
    subplots[3].axhline(y=0, color='k')

    subplots[4].plot(self.t, self.angleErrorList, 'm')
    subplots[4].set_title('Target Error')
    subplots[4].axhline(y=0, color='k')

    subplots[5].plot(self.t, self.tStepList, 'k')
    subplots[5].set_title('tStep')
    subplots[5].set_ylim([0, 1 / self.loopFreq])

    pyplot.show()

def _runnerSim(*args, **kwargs) -> dict:
  '''!@brief Multithreaded runner to execute simulation and return results

  @return dict same as Satellite.run
  '''
  sim = Satellite(*args, **kwargs)
  return sim.run()

def main():
  parser = ArgumentParser()
  parser.add_argument(
      '--monte-carlo',
      action='store_true',
      default=False,
      help='Run a monte carlo simulation of the ADCS')
  parser.add_argument(
      '-n',
      help='Number of monte carlo simulations to run',
      default=100)
  parser.add_argument(
      '-j',
      help='Number of threads to use for monte carlo simulation',
      default=cpu_count())
  parser.add_argument(
      '--initial_omega', '-w',
      default=0,
      help='Initialize angular momentum with random value up to --initial_omega rad/s')

  args = parser.parse_args(sys.argv[1:])

  args.initial_omega = float(args.initial_omega)
  args.j = max(1, int(args.j))

  kwargs = {
    'initialOmega': args.initial_omega
  }

  if args.monte_carlo:
    args.n = int(args.n)
    digits = int(np.ceil(np.log10(args.n)))

    converged = []
    timeToConvergence = []
    averagePower = []
    totalEnergy = []
    start = datetime.datetime.now()
    simDuration = 0
    with Pool(args.j) as p:
      results = [p.apply_async(_runnerSim, kwds=kwargs) for _ in range(args.n)]
      i = 0
      for p in results:
        results = p.get()
        converged.append(results['converged'])
        averagePower.append(results['averagePower'])
        totalEnergy.append(results['totalEnergy'])
        simDuration += results['timeToConvergence']

        if results['converged']:
          timeToConvergence.append(results['timeToConvergence'])
          print(
            f'{Fore.CYAN}[{i:{digits}}]{Fore.RESET} {Fore.GREEN}Convergence after {results["timeToConvergence"]:6.2f}s')
        else:
          print(
            f'{Fore.CYAN}[{i:{digits}}]{Fore.RESET} {Fore.RED}No convergence')
        i += 1
    irlDuration = (datetime.datetime.now() - start).total_seconds()
    print('-----------')
    success = sum(converged) / len(converged)
    if success == 1:
      print(f'{Fore.GREEN}{success * 100:6.2f}% converged')
      averageConvergence = sum(timeToConvergence) / len(timeToConvergence)
      print(f'Average time to convergence: {averageConvergence:6.2f}s')
    elif success != 0:
      print(f'{Fore.YELLOW}{success * 100:6.2f}% converged')
      averageConvergence = sum(timeToConvergence) / len(timeToConvergence)
      print(f'Average time to convergence: {averageConvergence:6.2f}s')
    else:
      print(f'{Fore.RED}{success * 100:6.2f}% converged')
    print(f'Average Power {np.average(averagePower):6.2f}W')
    print(f'Total Energy  {np.average(totalEnergy):6.2f}J')
    speed = simDuration / irlDuration
    print(f'Sim speed     {speed:6.2f}s/s')

  else:
    start = datetime.datetime.now()
    sim = Satellite(**kwargs)
    results = sim.run()
    if results['converged']:
      print(
        f'{Fore.GREEN}Converged after {results["timeToConvergence"]:6.2f}s')
    else:
      print(
        f'{Fore.RED}Did not converged after {results["timeToConvergence"]:6.2f}s')
    print(f'Average Power {results["averagePower"]:6.2f}W')
    print(f'Total Energy  {results["totalEnergy"]:6.2f}J')
    irlDuration = (datetime.datetime.now() - start).total_seconds()
    simDuration = results["timeToConvergence"]
    speed = simDuration / irlDuration
    print(f'Sim speed     {speed:6.2f}s/s')
    sim.plot()


if __name__ == '__main__':
  main()
