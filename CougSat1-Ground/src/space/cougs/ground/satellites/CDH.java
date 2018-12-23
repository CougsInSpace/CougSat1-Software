package space.cougs.ground.satellites;

import space.cougs.ground.utils.CISErrors;

/**
 * Telemetry for C&DH
 */
public class CDH {
  private ModeEnum mode = ModeEnum.NO_CONNECTION;

  private long      time        = 0;
  private long      sdCard      = 0;
  private int       resetCount  = 0;
  private CISErrors errorStatus = CISErrors.SUCCESS;

  /**
   * Operating mode of the satellite
   */
  public enum ModeEnum {
    SAFE,
    STANDBY,
    SCIENCE,
    TRANSMIT,
    NO_CONNECTION
  }

  /**
   * Constructs a new C&DH
   */
  public CDH() {}

  /**
   * @return the operating mode
   */
  public ModeEnum getMode() {
    return mode;
  }

  /**
   * @param mode of operation set
   */
  public void setMode(int mode) {
    switch (mode) {
      case -1:
        this.mode = ModeEnum.NO_CONNECTION;
        break;
      case 0x1:
        this.mode = ModeEnum.SAFE;
        break;
      case 0x2:
        this.mode = ModeEnum.STANDBY;
        break;
      case 0x3:
        this.mode = ModeEnum.SCIENCE;
        break;
      case 0x4:
        this.mode = ModeEnum.TRANSMIT;
        break;
      default:
        System.out.printf("Unknown operating mode: %d\n", mode);
    }
  }

  /**
   * @return the errorStatus
   */
  public CISErrors getErrorStatus() {
    return errorStatus;
  }

  /**
   * @param errorStatus to set
   */
  public void setErrorStatus(int errorStatus) {
    this.errorStatus = CISErrors.values()[errorStatus];
  }

  /**
   * @return the resetCount
   */
  public int getResetCount() {
    return resetCount;
  }

  /**
   * @param resetCount to set
   */
  public void setResetCount(int resetCount) {
    this.resetCount = resetCount;
  }

  /**
   * @return the sdCard used bytes
   */
  public long getSDCard() {
    return sdCard;
  }

  /**
   * @param sdCard used bytes to set
   */
  public void setSDCard(long sdCard) {
    this.sdCard = sdCard;
  }

  /**
   * @return the time in seconds since epoch
   */
  public long getTime() {
    return time;
  }

  /**
   * @param time in seconds since epoch to set
   */
  public void setTime(long time) {
    this.time = time;
  };
}