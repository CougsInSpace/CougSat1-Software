from argparse import ArgumentParser
import colorama
from colorama import Fore
import matplotlib.pyplot as pyplot
import numpy as np
import quaternion
import sys

colorama.init(autoreset=True)

class Satellite:

  def __init__(self, detumble: float) -> None:
    '''!@brief Create a Satellite simulation

    @param detumble Initial angular momentum with magnitude gaussian(mu=detumble, sigma=detumble)
    '''
    self.batteryVoltage = np.random.uniform(3.3, 4.2)

    self.xyzCamera = np.transpose(np.array([[0, 0, -1]]))

    self.coilR = 1
    self.coilL = 1
    self.coilN = 1
    self.coilA = 1

    self.target = np.random.uniform(-1, 1, size=(3, 1))
    self.target = self.target / np.linalg.norm(self.target)

    self.magField = quaternion.from_float_array(np.random.uniform(-1, 1, 4))
    self.magField = self.magField / abs(self.magField)

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
    self.iBody = np.array([[Ixx, Ixy, Ixz],
                           [Ixy, Iyy, Iyz],
                           [Ixz, Iyz, Izz]]) / 1e6
    self.iBodyInv = np.linalg.inv(self.iBody)

    # State variables
    self.q = quaternion.from_float_array(np.random.uniform(-1, 1, 4))
    self.q = self.q / abs(self.q)
    # rotate pi over 30 seconds
    omega = np.random.uniform(-1, 1, size=(3, 1))
    # np.random.normal(detumble, detumble)
    omega = omega / np.linalg.norm(omega) * detumble
    self.l = self.iBody @ omega

    self.loopFreq = 10

    self.t = []
    self.omegaList = []
    self.qList = []
    self.rList = []
    self.cameraList = []
    self.angleErrorList = []

    self.minTStep = 1 / (self.loopFreq * 100)
    self.maxOmega = 100
    self.maxAcceleration = np.deg2rad(5) / 1
    self.maxTorque = np.max(
      self.iBody @ np.repeat([[self.maxAcceleration]], 3, axis=0))
    self.maxAngleStep = self.maxOmega * self.minTStep
    self.maxLStep = self.maxTorque * self.minTStep
    self.minTStepReached = False

    self.omegaThreshold = np.deg2rad(1) / 1
    self.angleThreshold = np.deg2rad(1)

    self.converged = False

  def __solveODE(self, duration: float) -> float:
    '''!@brief Solve ODE at current point up to duration in the future

    @param duration Max time to increment up to
    @return float actual time step
    '''
    # Calculate state vector and derivative
    r = quaternion.as_rotation_matrix(self.q)
    iInv = r @ self.iBodyInv @ np.transpose(r)
    torque = np.array([[1e-5], [0], [0]])
    omega = iInv @ self.l
    omegaQ = quaternion.from_vector_part(omega, 0)[0]
    qDot = 0.5 * omegaQ * self.q

    # Determine appropriate tStep
    tStep = duration

    torqueAbs = np.linalg.norm(torque)
    if torqueAbs != 0:
      tStep = min(tStep, self.maxLStep / torqueAbs)
      if tStep != duration and tStep < self.minTStep:
        tStep = self.minTStep
        if not self.minTStepReached:
          print(
            f'{Fore.RED} tStep has reached minimum value with torque = {torqueAbs} Nm')
          self.minTStepReached = True
    omegaQAbs = abs(omegaQ)
    if omegaQAbs != 0:
      tStep = min(tStep, self.maxAngleStep / omegaQAbs)
      if tStep != duration and tStep < self.minTStep:
        tStep = self.minTStep
        if not self.minTStepReached:
          print(
            f'{Fore.RED} tStep has reached minimum value with omega = {omegaQAbs} rad/s')
          self.minTStepReached = True

    # Euler's method
    self.l = self.l + torque * tStep
    self.q = self.q + qDot * tStep
    self.q = self.q / abs(self.q)

    self.omegaList.append(omega)
    self.qList.append(self.q)
    self.rList.append(r)

    # Calculate residuals
    camera = r @ self.xyzCamera
    self.cameraList.append(camera)

    cameraNorm = camera / np.linalg.norm(camera)
    dotP = np.sum(cameraNorm * self.target)
    angleError = np.arccos(np.clip(dotP, -1.0, 1.0))
    self.angleErrorList.append(angleError)

    if omegaQAbs > self.omegaThreshold:
      self.converged = False
    if angleError > self.angleThreshold:
      self.converged = False

    return tStep

  def run(self, durationMax: float = 180) -> dict:
    '''!@brief Run simulation until residuals reach zero or max duration

    @param durationMax Maximum duration to run before declaring system does not converge
    @return dict Collection of success parameters
    '''
    tStep = 1 / self.loopFreq
    t = 0
    n = int(np.ceil(durationMax / tStep))

    for i in range(n):
      tTarget = tStep * i
      tRemaining = tTarget - t
      while t < tTarget:
        t += self.__solveODE(tRemaining)
        self.t.append(t)
        tRemaining = tTarget - t

      self.controlLoop()
      if self.converged:
        break

    results = {
      'converged': self.converged,
      'timeToConvergence': t
    }
    return results

  def plot(self) -> None:
    '''!@brief Plot simulation
    '''
    ax = pyplot.figure().add_subplot(projection='3d')

    rList = np.array(self.rList)
    camera = np.array(self.cameraList)
    omega = np.array(self.omegaList)

    ax.quiver(
        [0], [0], [0], [
            self.target[0]], [
            self.target[1]], [
            self.target[2]], colors='y')

    x = camera[:, 0, 0]
    y = camera[:, 1, 0]
    z = camera[:, 2, 0]
    ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors='m')
    ax.plot3D(x, y, z, 'm')

    colors = ['r', 'g', 'b']
    for i in range(3):
      x = rList[:, 0, i]
      y = rList[:, 1, i]
      z = rList[:, 2, i]
      ax.quiver([0], [0], [0], [x[-1]], [y[-1]], [z[-1]], colors=colors[i])
      ax.plot3D(x, y, z, colors[i])

    ax.set_xlim(-1, 1)
    ax.set_ylim(-1, 1)
    ax.set_zlim(-1, 1)
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    ax.set_zlabel('z')

    pyplot.figure()
    x = omega[:, 0, 0]
    y = omega[:, 1, 0]
    z = omega[:, 2, 0]
    pyplot.plot(self.t, x, 'r', label='omegaX')
    pyplot.plot(self.t, y, 'g', label='omegaY')
    pyplot.plot(self.t, z, 'b', label='omegaZ')
    pyplot.plot(self.t, self.angleErrorList, 'm', label='Target Error')
    pyplot.title('Residuals')
    pyplot.legend()

    pyplot.show()

  def controlLoop(self) -> None:
    '''!@brief Do ADCS control loop taking in sensor inputs and outputing desired currents
    '''
    pass


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
      default=10)
  parser.add_argument(
      '--detumble',
      default=0,
      help='Initialize angular momentum with random value up to --detumble rad/s')

  args = parser.parse_args(sys.argv[1:])

  args.detumble = float(args.detumble)

  if args.monte_carlo:
    args.n = int(args.n)
    digits = int(np.ceil(np.log10(args.n)))

    converged = []
    timeToConvergence = []
    for i in range(args.n):
      sim = Satellite(detumble=args.detumble)
      results = sim.run()
      converged.append(results['converged'])

      if results['converged']:
        timeToConvergence.append(results['timeToConvergence'])
        print(
          f'{Fore.CYAN}[{i:{digits}}]{Fore.RESET} {Fore.GREEN}Convergence after {results["timeToConvergence"]:6.2f}s')
      else:
        print(
          f'{Fore.CYAN}[{i:{digits}}]{Fore.RESET} {Fore.RED}No convergence')
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

  else:
    sim = Satellite(detumble=args.detumble)
    results = sim.run()
    if results['converged']:
      print(
        f'{Fore.GREEN}Converged after {results["timeToConvergence"]:6.2f}s')
    else:
      print(
        f'{Fore.RED}Did not converged after {results["timeToConvergence"]:6.2f}s')
    sim.plot()


if __name__ == '__main__':
  main()
