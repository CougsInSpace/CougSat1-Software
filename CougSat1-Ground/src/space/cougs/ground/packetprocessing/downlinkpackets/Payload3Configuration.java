package space.cougs.ground.packetprocessing.downlinkpackets;

import java.io.File;

import space.cougs.ground.satellites.CougSat;
import space.cougs.ground.utils.CISErrors;

public class Payload3Configuration extends DownlinkPacket {
  public static final int ID = 0x1C;

  @Override
  public CISErrors decodePacket(File file, CougSat satellite) {
    return CISErrors.NOT_SUPPORTED;
  }
}
