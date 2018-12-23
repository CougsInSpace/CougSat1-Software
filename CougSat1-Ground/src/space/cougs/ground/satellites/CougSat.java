package space.cougs.ground.satellites;

/**
 * Telemetry for a CougSat
 */
public abstract class CougSat {

  /**
   * @return the ID of the satellite for packet routing
   */
  public abstract int getID();

  private final ADCS adcs   = new ADCS();
  private final CDH cdh     = new CDH();
  private final Comms comms = new Comms();
  private final ECS ecs     = new ECS();
  private final EPS eps     = new EPS();

  /**
   * Constructs a new CougSat
   */
  public CougSat() {}

  /**
   * @return the ADCS
   */
  public ADCS getADCS() {
    return adcs;
  }

  /**
   * @return the CDH
   */
  public CDH getCDH() {
    return cdh;
  }

  /**
   * @return the Comms
   */
  public Comms getComms() {
    return comms;
  }

  /**
   * @return the ECS
   */
  public ECS getECS() {
    return ecs;
  }

  /**
   * @return the EPS
   */
  public EPS getEPS() {
    return eps;
  }
}
