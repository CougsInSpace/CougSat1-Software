package space.cougs.ground.packetprocessing.downlinkpackets;

import java.io.File;
import java.io.IOException;

import space.cougs.ground.satellites.CougSat;
import space.cougs.ground.utils.CISErrors;

public class Payload1Configuration extends DownlinkPacket {
  public static final int ID = 0x14;

  @Override
  public CISErrors decodePacket(File file, CougSat satellite)
      throws IOException {
    return CISErrors.NOT_SUPPORTED;
  }
}
