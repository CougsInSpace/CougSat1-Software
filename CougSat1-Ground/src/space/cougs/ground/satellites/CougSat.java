package space.cougs.ground.satellites;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;

/**
 * Telemetry for a CougSat
 */
public abstract class CougSat implements Serializable{
  private static final long serialVersionUID = 1L;

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
   * @return a deep copy of the satellite
   */
  public CougSat deepCopy() {
    try {
      ByteArrayOutputStream outBytes = new ByteArrayOutputStream();
      ObjectOutputStream out = new ObjectOutputStream(outBytes);
      out.writeObject(this);

      ObjectInputStream in = new ObjectInputStream(new ByteArrayInputStream(outBytes.toByteArray()));
      return (CougSat) in.readObject();
    } catch (IOException | ClassNotFoundException e) {
      System.out.println("Failed to deep copy CougSat");
      e.printStackTrace();
    }
    return null;
	}

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
