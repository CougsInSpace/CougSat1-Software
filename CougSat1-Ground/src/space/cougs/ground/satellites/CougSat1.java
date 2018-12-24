package space.cougs.ground.satellites;

/**
 * CougSat-1 Telemtery and Payload Data
 */
public class CougSat1 extends CougSat{
  private static final long serialVersionUID = 1L;

  /**
   * Constructs a new CougSat-1
   */
  public CougSat1() {}

  @Override
  public int getID() {
    return 0x20;
  }

  @Override
  public String toString() {
    return "CougSat-1";
  }
}
