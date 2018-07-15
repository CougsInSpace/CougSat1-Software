package space.cougs.ground.packetprocessing.downlinkpackets;

import java.io.File;
import java.io.IOException;

import space.cougs.ground.satellites.CougSat;

public abstract class DownlinkPacket {

	public static final int ID = -1;

	public abstract boolean decodePacket(File file, CougSat satellite) throws IOException;

}
