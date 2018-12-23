package space.cougs.ground.satellites;

/**
 * Telemetry for ECS
 */
public class ECS {
  private int adcsTemp = 0;

  private int ihuTemp = 0;

  private int ifjrTemp = 0;

  private int commsTemp  = 0;
  private int rxTemp     = 0;
  private int tx230Temp  = 0;
  private int tx700Temp  = 0;
  private int reg5V0Temp = 0;
  private int reg9V0Temp = 0;

  private int pmicTemp       = 0;
  private int batteryTemps[] = new int[2];
  private int reg3V3Temps[]  = new int[2];
  private int pvTemps[]      = new int[8];
  private int mpptTemps[]    = new int[8];

  /**
   * Constructs a new ECS
   */
  public ECS() {}

  /**
   * @return the adcsTemp
   */
  public int getADCSTemp() {
    return adcsTemp;
  }

  /**
   * @param adcsTemp the adcsTemp to set
   */
  public void setADCSTemp(int adcsTemp) {
    this.adcsTemp = adcsTemp;
  }

  /**
   * @return the ihuTemp
   */
  public int getIHUTemp() {
    return ihuTemp;
  }

  /**
   * @param ihuTemp the ihuTemp to set
   */
  public void setIHUTemp(int ihuTemp) {
    this.ihuTemp = ihuTemp;
  }

  /**
   * @return the ifjrTemp
   */
  public int getIFJRTemp() {
    return ifjrTemp;
  }

  /**
   * @param ifjrTemp the ifjrTemp to set
   */
  public void setIFJRTemp(int ifjrTemp) {
    this.ifjrTemp = ifjrTemp;
  }

  /**
   * @return the commsTemp
   */
  public int getCommsTemp() {
    return commsTemp;
  }

  /**
   * @param commsTemp the commsTemp to set
   */
  public void setCommsTemp(int commsTemp) {
    this.commsTemp = commsTemp;
  }

  /**
   * @return the rxTemp
   */
  public int getRXTemp() {
    return rxTemp;
  }

  /**
   * @param rxTemp the rxTemp to set
   */
  public void setRXTemp(int rxTemp) {
    this.rxTemp = rxTemp;
  }

  /**
   * @return the tx230Temp
   */
  public int getTX230Temp() {
    return tx230Temp;
  }

  /**
   * @param tx230Temp the tx230Temp to set
   */
  public void setTX230Temp(int tx230Temp) {
    this.tx230Temp = tx230Temp;
  }

  /**
   * @return the tx700Temp
   */
  public int getTX700Temp() {
    return tx700Temp;
  }

  /**
   * @param tx700Temp the tx700Temp to set
   */
  public void setTX700Temp(int tx700Temp) {
    this.tx700Temp = tx700Temp;
  }

  /**
   * @return the reg5V0Temp
   */
  public int getReg5V0Temp() {
    return reg5V0Temp;
  }

  /**
   * @param reg5V0Temp the reg5V0Temp to set
   */
  public void setReg5V0Temp(int reg5V0Temp) {
    this.reg5V0Temp = reg5V0Temp;
  }

  /**
   * @return the reg9V0Temp
   */
  public int getReg9V0Temp() {
    return reg9V0Temp;
  }

  /**
   * @param reg9V0Temp the reg9V0Temp to set
   */
  public void setReg9V0Temp(int reg9V0Temp) {
    this.reg9V0Temp = reg9V0Temp;
  }

  /**
   * @return the pmicTemp
   */
  public int getPMICTemp() {
    return pmicTemp;
  }

  /**
   * @param pmicTemp the pmicTemp to set
   */
  public void setPMICTemp(int pmicTemp) {
    this.pmicTemp = pmicTemp;
  }

  /**
   * @param i index of the battery
   * @return the temp
   */
  public int getBatteryTemp(int i) {
    return batteryTemps[i];
  }

  /**
   * @param i    index of the battery
   * @param temp the temp to set
   */
  public void setBatteryTemp(int i, int temp) {
    this.batteryTemps[i] = temp;
  }

  /**
   * @param i index of the reg
   * @return the temp
   */
  public int getReg3V3Temp(int i) {
    return reg3V3Temps[i];
  }

  /**
   * @param i    index of the reg
   * @param temp the temp to set
   */
  public void setReg3V3Temp(int i, int temp) {
    this.reg3V3Temps[i] = temp;
  }

  /**
   * @param i index of the solar cell
   * @return the temp
   */
  public int getPVTemp(int i) {
    return pvTemps[i];
  }

  /**
   * @param i    index of the solar cell
   * @param temp the temp to set
   */
  public void setPVTemp(int i, int temp) {
    this.pvTemps[i] = temp;
  }

  /**
   * @param i index of the mppt
   * @return the temp
   */
  public int getMPPTTemp(int i) {
    return mpptTemps[i];
  }

  /**
   * @param i    index of the mppt
   * @param temp the temp to set
   */
  public void setMPPTTemp(int i, int temp) {
    this.mpptTemps[i] = temp;
  }
}