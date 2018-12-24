package space.cougs.ground;

import java.io.File;

import space.cougs.ground.gui.GUI;
import space.cougs.ground.packetprocessing.PacketHeader;
import space.cougs.ground.satellites.CougSat;
import space.cougs.ground.satellites.CougSat1;
import space.cougs.ground.utils.CISErrors;

public class CougSatGround {
  private static final String versionNumber = "1.1.0";
  private static final long   start         = System.currentTimeMillis();

  private static File home = new File("");

  private static final GUI gui = new GUI();

  private static final CougSat1 cougSat1         = new CougSat1();
  private static final PacketHeader packetHeader = new PacketHeader();

  public static void main(String[] args) {
    System.out.printf(
        "GUI boot took: %dms\n", System.currentTimeMillis() - start);

    packetHeader.addSatellite((CougSat)cougSat1);

    CISErrors result =
        packetHeader.decodePacket("test/rawPackets/TestTelemetry.cspkt");
    if (result != CISErrors.SUCCESS) {
      System.out.printf("Failed to decode packet: 0x%02X %s\n",
          result.ordinal(), result.toString());
    }

    gui.updateSatellite(cougSat1);
  }

  public static String getVersionnumber() {
    return versionNumber;
  }

  public static File getHomeDir() {
    return home;
  }
}
