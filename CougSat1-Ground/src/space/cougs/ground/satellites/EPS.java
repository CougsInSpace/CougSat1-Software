package space.cougs.ground.satellites;

import java.util.Arrays;

/**
 * Telemetry for EPS
 */
public class EPS {
  private double pvVoltages[] = new double[8];
  private double pvCurrents[] = new double[8];

  private double batteryVoltages[] = new double[2];
  private double batteryCurrents[] = new double[2];

  private double reg3V3Voltages[] = new double[2];
  private double reg3V3Currents[] = new double[2];

  private SwitchNode nodesMPPT[]          = new SwitchNode[8];
  private SwitchNode nodesPR3V3[]         = new SwitchNode[13];
  private SwitchNode nodesPRBatt[]        = new SwitchNode[7];
  private SwitchNode nodesPV3V3[]         = new SwitchNode[4];
  private SwitchNode nodesBatteryHeater[] = new SwitchNode[2];
  private SwitchNode nodePRDeployables    = new SwitchNode();

  private int energyLevel = 0;

  /**
   * Switch consisting of two independent switches and current
   */
  private class SwitchNode {
    // TODO add get power that uses the properly connected source voltage
    boolean pathA   = false;
    boolean pathB   = false;
    double  current = 0.0;

    SwitchNode() {}
  }

  /**
   * Constructs a new EPS
   */
  public EPS() {
    for (int i = 0; i < nodesMPPT.length; i++) {
      nodesMPPT[i] = new SwitchNode();
    }
    for (int i = 0; i < nodesPR3V3.length; i++) {
      nodesPR3V3[i] = new SwitchNode();
    }
    for (int i = 0; i < nodesPRBatt.length; i++) {
      nodesPRBatt[i] = new SwitchNode();
    }
    for (int i = 0; i < nodesPV3V3.length; i++) {
      nodesPV3V3[i] = new SwitchNode();
    }
    for (int i = 0; i < nodesBatteryHeater.length; i++) {
      nodesBatteryHeater[i] = new SwitchNode();
    }
  }

  /**
   * @param i index of solar cell
   * @return the voltage
   */
  public double getPVVoltage(int i) {
    return pvVoltages[i];
  }

  /**
   * @return the average voltage
   */
  public double getPVVoltageAvg() {
    return Arrays.stream(pvVoltages).average().getAsDouble();
  }

  /**
   * @param i       index of solar cell
   * @param current the voltage to set
   */
  public void setPVVoltage(int i, double voltage) {
    this.pvVoltages[i] = voltage;
  }

  /**
   * @param i index of solar cell
   * @return the current
   */
  public double getPVCurrent(int i) {
    return pvCurrents[i];
  }

  /**
   * @return the current
   */
  public double getPVCurrentSum() {
    return Arrays.stream(pvCurrents).sum();
  }

  /**
   * @param i       index of solar cell
   * @param current the current to set
   */
  public void setPVCurrent(int i, double current) {
    this.pvCurrents[i] = current;
  }

  /**
   * @param i index of solar cell
   * @return the current
   */
  public double getMPPTCurrent(int i) {
    return nodesMPPT[i].current;
  }

  /**
   * @param i       index of solar cell
   * @param current the current to set
   */
  public void setMPPTCurrent(int i, double current) {
    this.nodesMPPT[i].current += 1;
    this.nodesMPPT[i].current = current;
  }

  /**
   * @param i index of battery
   * @return the voltage
   */
  public double getBatteryVoltage(int i) {
    return batteryVoltages[i];
  }

  /**
   * @param i       index of battery
   * @param current the voltage to set
   */
  public void setBatteryVoltage(int i, double voltage) {
    this.batteryVoltages[i] = voltage;
  }

  /**
   * @param i index of battery
   * @return the current
   */
  public double getBatteryCurrent(int i) {
    return batteryCurrents[i];
  }

  /**
   * @param i       index of battery
   * @param current the current to set
   */
  public void setBatteryCurrent(int i, double current) {
    this.batteryCurrents[i] = current;
  }

  /**
   * @param i index of regulator
   * @return the voltage
   */
  public double getReg3V3Voltage(int i) {
    return reg3V3Voltages[i];
  }

  /**
   * @param i       index of regulator
   * @param current the voltage to set
   */
  public void setReg3V3Voltage(int i, double voltage) {
    this.reg3V3Voltages[i] = voltage;
  }

  /**
   * @param i index of regulator
   * @return the current
   */
  public double getReg3V3Current(int i) {
    return reg3V3Currents[i];
  }

  /**
   * @param i       index of regulator
   * @param current the current to set
   */
  public void setReg3V3Current(int i, double current) {
    this.reg3V3Currents[i] = current;
  }

  /**
   * @param i index of channel
   * @return the current
   */
  public double getPR3V3Current(int i) {
    return nodesPR3V3[i].current;
  }

  /**
   * @return the current
   */
  public double getPR3V3CurrentSum() {
    double sum = 0.0;
    for (SwitchNode node : nodesPR3V3) {
      sum += node.current;
    }
    return sum;
  }

  /**
   * @return sum of power on the 3.3V rails
   */
  public double getPR3V3PowerSum() {
    // TODO multiply the current by the voltage of the connected regulator
    double voltage = (reg3V3Voltages[0] + reg3V3Voltages[1]) / 2.0;
    return voltage * getPR3V3CurrentSum();
  }

  /**
   * @param i       index of channel
   * @param current the current to set
   */
  public void setPR3V3Current(int i, double current) {
    this.nodesPR3V3[i].current = current;
  }

  /**
   * @param i index of channel
   * @return the current
   */
  public double getPRBattCurrent(int i) {
    return nodesPRBatt[i].current;
  }

  /**
   * @return the current
   */
  public double getPRBattCurrentSum() {
    double sum = 0.0;
    for (SwitchNode node : nodesPRBatt) {
      sum += node.current;
    }
    return sum;
  }

  /**
   * @return sum of power on the Batt rails
   */
  public double getPRBattPowerSum() {
    // TODO multiply the current by the voltage of the connected battery
    double voltage = (batteryVoltages[0] + batteryVoltages[1]) / 2.0;
    return voltage * getPRBattCurrentSum();
  }

  /**
   * @param i       index of channel
   * @param current the current to set
   */
  public void setPRBattCurrent(int i, double current) {
    this.nodesPRBatt[i].current = current;
  }

  /**
   * @param i index of channel
   * @return the current
   */
  public double getPV3V3Current(int i) {
    return nodesPV3V3[i].current;
  }

  /**
   * @return the current
   */
  public double getPV3V3CurrentSum() {
    double sum = 0.0;
    for (SwitchNode node : nodesPV3V3) {
      sum += node.current;
    }
    return sum;
  }

  /**
   * @return sum of power on the PV3.3V rails
   */
  public double getPV3V3PowerSum() {
    // TODO multiply the current by the voltage of the connected regulator
    double voltage = (reg3V3Voltages[0] + reg3V3Voltages[1]) / 2.0;
    return voltage * getPV3V3CurrentSum();
  }

  /**
   * @param i       index of channel
   * @param current the current to set
   */
  public void setPV3V3Current(int i, double current) {
    this.nodesPV3V3[i].current = current;
  }

  /**
   * @param i index of channel
   * @return the current
   */
  public double getPRBHCurrent(int i) {
    return nodesBatteryHeater[i].current;
  }

  /**
   * @return the current
   */
  public double getPRBHCurrentSum() {
    double sum = 0.0;
    for (SwitchNode node : nodesBatteryHeater) {
      sum += node.current;
    }
    return sum;
  }

  /**
   * @return sum of power on the heater rails
   */
  public double getHeaterPowerSum() {
    // TODO multiply the current by the voltage of the connected battery
    double voltage = (batteryVoltages[0] + batteryVoltages[1]) / 2.0;
    return voltage * getPRBHCurrentSum();
  }

  /**
   * @param i       index of channel
   * @param current the current to set
   */
  public void setPRBHCurrent(int i, double current) {
    this.nodesBatteryHeater[i].current = current;
  }

  /**
   * @return the prDeployablesCurrent
   */
  public double getPRDeployablesCurrent() {
    return nodePRDeployables.current;
  }

  /**
   * @param current the current to set
   */
  public void setPRDeployablesCurrent(double current) {
    this.nodePRDeployables.current = current;
  }

  /**
   * @param i       index of channel
   * @param isPathA returns path A if true
   * @return the switch state
   */
  public boolean getMPPTSwitchingState(int i, boolean isPathA) {
    if (isPathA)
      return nodesMPPT[i].pathA;
    else
      return nodesMPPT[i].pathB;
  }

  /**
   * @param i     index of channel
   * @param pathA the state of the A switch
   * @param pathB the state of the B switch
   */
  public void setMPPTSwitchingState(int i, boolean pathA, boolean pathB) {
    this.nodesMPPT[i].pathA = pathA;
    this.nodesMPPT[i].pathB = pathB;
  }

  /**
   * @param i       index of channel
   * @param isPathA returns path A if true
   * @return the switch state
   */
  public boolean getPR3V3SwitchingState(int i, boolean isPathA) {
    if (isPathA)
      return nodesPR3V3[i].pathA;
    else
      return nodesPR3V3[i].pathB;
  }

  /**
   * @param i     index of channel
   * @param pathA the state of the A switch
   * @param pathB the state of the B switch
   */
  public void setPR3V3SwitchingState(int i, boolean pathA, boolean pathB) {
    this.nodesPR3V3[i].pathA = pathA;
    this.nodesPR3V3[i].pathB = pathB;
  }

  /**
   * @param i       index of channel
   * @param isPathA returns path A if true
   * @return the switch state
   */
  public boolean getPRBattSwitchingState(int i, boolean isPathA) {
    if (isPathA)
      return nodesPRBatt[i].pathA;
    else
      return nodesPRBatt[i].pathB;
  }

  /**
   * @param i     index of channel
   * @param pathA the state of the A switch
   * @param pathB the state of the B switch
   */
  public void setPRBattSwitchingState(int i, boolean pathA, boolean pathB) {
    this.nodesPRBatt[i].pathA = pathA;
    this.nodesPRBatt[i].pathB = pathB;
  }

  /**
   * @param i       index of channel
   * @param isPathA returns path A if true
   * @return the switch state
   */
  public boolean getPV3V3SwitchingState(int i, boolean isPathA) {
    if (isPathA)
      return nodesPV3V3[i].pathA;
    else
      return nodesPV3V3[i].pathB;
  }

  /**
   * @param i     index of channel
   * @param pathA the state of the A switch
   * @param pathB the state of the B switch
   */
  public void setPV3V3SwitchingState(int i, boolean pathA, boolean pathB) {
    this.nodesPV3V3[i].pathA = pathA;
    this.nodesPV3V3[i].pathB = pathB;
  }

  /**
   * @param i       index of channel
   * @param isPathA returns path A if true
   * @return the switch state
   */
  public boolean getPRBatteryHeaterSwitchingState(int i, boolean isPathA) {
    if (isPathA)
      return nodesBatteryHeater[i].pathA;
    else
      return nodesBatteryHeater[i].pathB;
  }

  /**
   * @param i     index of channel
   * @param pathA the state of the A switch
   * @param pathB the state of the B switch
   */
  public void setPRBatteryHeaterSwitchingState(
      int i, boolean pathA, boolean pathB) {
    this.nodesBatteryHeater[i].pathA = pathA;
    this.nodesBatteryHeater[i].pathB = pathB;
  }

  /**
   * @param isPathA returns path A if true
   * @return the switch state
   */
  public boolean getPRDeployablesSwitchingState(boolean isPathA) {
    if (isPathA)
      return nodePRDeployables.pathA;
    else
      return nodePRDeployables.pathB;
  }

  /**
   * @param pathA the state of the A switch
   * @param pathB the state of the B switch
   */
  public void setPRDeployablesSwitchingState(boolean pathA, boolean pathB) {
    this.nodePRDeployables.pathA = pathA;
    this.nodePRDeployables.pathB = pathB;
  }

  /**
   * @return the energyLevel
   */
  public int getEnergyLevel() {
    return energyLevel;
  }

  /**
   * @param energyLevel the energyLevel to set
   */
  public void setEnergyLevel(int energyLevel) {
    this.energyLevel = energyLevel;
  }

  /**
   * @return sum of power on the Comms rails
   */
  public double getCommsPowerSum() {
    // TODO multiply the current by the voltage of the connected battery
    double voltageBatt = (batteryVoltages[0] + batteryVoltages[1]) / 2.0;
    double voltage3V3  = (reg3V3Voltages[0] + reg3V3Voltages[1]) / 2.0;
    return voltageBatt * nodesPRBatt[1].current +
        voltage3V3 * nodesPR3V3[6].current;
  }

  /**
   * @return sum of power into the EPS
   */
  public double getInPowerSum() {
    double power = 0.0;
    for (int i = 0; i < pvCurrents.length; i++){
      power += pvCurrents[i] + pvVoltages[i];
    }
    return power;
  }

  /**
   * @return sum of power out of the EPS
   */
  public double getOutPowerSum() {
    double power = getPR3V3PowerSum();
    power += getPRBattPowerSum();
    power += getHeaterPowerSum();
    power += getPV3V3PowerSum();
    return power;
  }
}