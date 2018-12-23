package space.cougs.ground.satellites;

/**
 * Telemetry for ADCS
 */
public class ADCS {
  private double latitude  = 0.0;
  private double longitude = 0.0;

  private double roll  = 0.0;
  private double pitch = 0.0;
  private double yaw   = 0.0;

  private int xPWMOut = 0;
  private int yPWMOut = 0;
  private int zPWMOut = 0;

  private double xCurrent = 0;
  private double yCurrent = 0;
  private double zCurrent = 0;

  /**
   * Constructs a new ADCS
   */
  public ADCS() {}

  /**
   * @return the latitude
   */
  public double getLatitude() {
    return latitude;
  }

  /**
   * @param latitude the latitude to set
   */
  public void setLatitude(double latitude) {
    this.latitude = latitude;
  }

  /**
   * @return the longitude
   */
  public double getLongitude() {
    return longitude;
  }

  /**
   * @param longitude the longitude to set
   */
  public void setLongitude(double longitude) {
    this.longitude = longitude;
  }

  /**
   * @return the roll
   */
  public double getRoll() {
    return roll;
  }

  /**
   * @param roll the roll to set
   */
  public void setRoll(double roll) {
    this.roll = roll;
  }

  /**
   * @return the pitch
   */
  public double getPitch() {
    return pitch;
  }

  /**
   * @param pitch the pitch to set
   */
  public void setPitch(double pitch) {
    this.pitch = pitch;
  }

  /**
   * @return the yaw
   */
  public double getYaw() {
    return yaw;
  }

  /**
   * @param yaw the yaw to set
   */
  public void setYaw(double yaw) {
    this.yaw = yaw;
  }

  /**
   * @return the xPWMOut
   */
  public int getXPWMOut() {
    return xPWMOut;
  }

  /**
   * @param xPWMOut the xPWMOut to set
   */
  public void setXPWMOut(int xPWMOut) {
    this.xPWMOut = xPWMOut;
  }

  /**
   * @return the yPWMOut
   */
  public int getYPWMOut() {
    return yPWMOut;
  }

  /**
   * @param yPWMOut the yPWMOut to set
   */
  public void setYPWMOut(int yPWMOut) {
    this.yPWMOut = yPWMOut;
  }

  /**
   * @return the zPWMOut
   */
  public int getZPWMOut() {
    return zPWMOut;
  }

  /**
   * @param zPWMOut the zPWMOut to set
   */
  public void setZPWMOut(int zPWMOut) {
    this.zPWMOut = zPWMOut;
  }

  /**
   * @return the xCurrent
   */
  public double getXCurrent() {
    return xCurrent;
  }

  /**
   * @param xCurrent the xCurrent to set
   */
  public void setXCurrent(double xCurrent) {
    this.xCurrent = xCurrent;
  }

  /**
   * @return the yCurrent
   */
  public double getYCurrent() {
    return yCurrent;
  }

  /**
   * @param yCurrent the yCurrent to set
   */
  public void setYCurrent(double yCurrent) {
    this.yCurrent = yCurrent;
  }

  /**
   * @return the zCurrent
   */
  public double getZCurrent() {
    return zCurrent;
  }

  /**
   * @param zCurrent the zCurrent to set
   */
  public void setZCurrent(double zCurrent) {
    this.zCurrent = zCurrent;
  }
}