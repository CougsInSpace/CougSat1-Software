package space.cougs.ground.packetprocessing.downlinkpackets;

import java.io.File;
import java.io.IOException;

import space.cougs.ground.satellites.CougSat;
import space.cougs.ground.utils.CISErrors;

public abstract class DownlinkPacket {
  public abstract CISErrors decodePacket(File file, CougSat satellite)
      throws IOException;
}
