package space.cougs.ground;

import space.cougs.ground.gui.GUI;
import space.cougs.ground.packetprocessing.PacketHeader;
import space.cougs.ground.satellites.CougSat;
import space.cougs.ground.satellites.CougSat1;
import space.cougs.ground.utils.CISErrors;

public class CougSatGround {
  private static final GUI gui                   = new GUI();
  private static final CougSat1 cougSat1         = new CougSat1();
  private static final PacketHeader packetHeader = new PacketHeader();
  private static final String versionNumber      = "1.0.3";

  public static void main(String[] args) {
    packetHeader.addSatellite((CougSat)cougSat1);
    CISErrors result =
        packetHeader.decodePacket("test/rawPackets/TestTelemetry.cspkt");
    if (result != CISErrors.SUCCESS) {
      System.out.printf("Failed to decode packet: 0x%02X %s\n", result.ordinal(),
          result.toString());
    }

    gui.update(cougSat1);
  }

  public static String getVersionnumber() {
    return versionNumber;
  }
}
