package space.cougs.ground.satellites;

/**
 * Telemetry for Comms
 */
public class Comms {
  private double rxPower    = 0.0;
  private double tx230Power = 0.0;
  private double tx700Power = 0.0;

  private double rxSNR = 0.0;

  private int rxCenterFrequency    = 0;
  private int tx230CenterFrequency = 0;
  private int tx700CenterFrequency = 0;

  private double reg5V0Voltage = 0.0;
  private double reg5V0Current = 0.0;
  private double reg9V0Voltage = 0.0;
  private double reg9V0Current = 0.0;

  private double pr3V3Current[] = new double[3];
  private double pr5V0Current[] = new double[3];
  private double pr9V0Current[] = new double[2];

  private int badPacketCount = 0;

  /**
   * Constructs a new Comms
   */
  public Comms() {}

  /**
   * @return the rxPower
   */
  public double getRXPower() {
    return rxPower;
  }

  /**
   * @param rxPower the rxPower to set
   */
  public void setRXPower(double rxPower) {
    this.rxPower = rxPower;
  }

  /**
   * @return the total TX power
   */
  public double getTXPower() {
    return tx230Power + tx700Power;
  }

  /**
   * @return the tx230Power
   */
  public double getTX230Power() {
    return tx230Power;
  }

  /**
   * @param tx230Power the tx230Power to set
   */
  public void setTX230Power(double tx230Power) {
    this.tx230Power = tx230Power;
  }

  /**
   * @return the tx700Power
   */
  public double getTX700Power() {
    return tx700Power;
  }

  /**
   * @param tx700Power the tx700Power to set
   */
  public void setTX700Power(double tx700Power) {
    this.tx700Power = tx700Power;
  }

  /**
   * @return the rxSNR
   */
  public double getRXSNR() {
    return rxSNR;
  }

  /**
   * @param rxSNR the rxSNR to set
   */
  public void setRXSNR(double rxSNR) {
    this.rxSNR = rxSNR;
  }

  /**
   * @return the rxCenterFrequency
   */
  public int getRXCenterFrequency() {
    return rxCenterFrequency;
  }

  /**
   * @param rxCenterFrequency the rxCenterFrequency to set
   */
  public void setRXCenterFrequency(int rxCenterFrequency) {
    this.rxCenterFrequency = rxCenterFrequency;
  }

  /**
   * @return the tx230CenterFrequency
   */
  public int getTX230CenterFrequency() {
    return tx230CenterFrequency;
  }

  /**
   * @param tx230CenterFrequency the tx230CenterFrequency to set
   */
  public void setTX230CenterFrequency(int tx230CenterFrequency) {
    this.tx230CenterFrequency = tx230CenterFrequency;
  }

  /**
   * @return the tx700CenterFrequency
   */
  public int getTX700CenterFrequency() {
    return tx700CenterFrequency;
  }

  /**
   * @param tx700CenterFrequency the tx700CenterFrequency to set
   */
  public void setTX700CenterFrequency(int tx700CenterFrequency) {
    this.tx700CenterFrequency = tx700CenterFrequency;
  }

  /**
   * @return the reg5V0Voltage
   */
  public double getReg5V0Voltage() {
    return reg5V0Voltage;
  }

  /**
   * @param reg5V0Voltage the reg5V0Voltage to set
   */
  public void setReg5V0Voltage(double reg5V0Voltage) {
    this.reg5V0Voltage = reg5V0Voltage;
  }

  /**
   * @return the reg5V0Current
   */
  public double getReg5V0Current() {
    return reg5V0Current;
  }

  /**
   * @param reg5V0Current the reg5V0Current to set
   */
  public void setReg5V0Current(double reg5V0Current) {
    this.reg5V0Current = reg5V0Current;
  }

  /**
   * @return the reg9V0Voltage
   */
  public double getReg9V0Voltage() {
    return reg9V0Voltage;
  }

  /**
   * @param reg9V0Voltage the reg9V0Voltage to set
   */
  public void setReg9V0Voltage(double reg9V0Voltage) {
    this.reg9V0Voltage = reg9V0Voltage;
  }

  /**
   * @return the reg9V0Current
   */
  public double getReg9V0Current() {
    return reg9V0Current;
  }

  /**
   * @param reg9V0Current the reg9V0Current to set
   */
  public void setReg9V0Current(double reg9V0Current) {
    this.reg9V0Current = reg9V0Current;
  }

  /**
   * @param i index of channel
   * @return the pr9V0Current
   */
  public double getPR3V3Current(int i) {
    return pr3V3Current[i];
  }

  /**
   * @param i       index of channel
   * @param current the current to set
   */
  public void setPR3V3Current(int i, double current) {
    this.pr3V3Current[i] = current;
  }

  /**
   * @param i index of channel
   * @return the pr9V0Current
   */
  public double getPR5V0Current(int i) {
    return pr5V0Current[i];
  }

  /**
   * @param i       index of channel
   * @param current the current to set
   */
  public void setPR5V0Current(int i, double current) {
    this.pr5V0Current[i] = current;
  }

  /**
   * @param i index of channel
   * @return the pr9V0Current
   */
  public double getPR9V0Current(int i) {
    return pr9V0Current[i];
  }

  /**
   * @param i       index of channel
   * @param current the current to set
   */
  public void setPR9V0Current(int i, double current) {
    this.pr9V0Current[i] = current;
  }

  /**
   * @return the badPacketCount
   */
  public int getBadPacketCount() {
    return badPacketCount;
  }

  /**
   * @param badPacketCount the badPacketCount to set
   */
  public void setBadPacketCount(int badPacketCount) {
    this.badPacketCount = badPacketCount;
  }
}