package space.cougs.ground.packetprocessing.downlinkpackets;

import java.io.IOException;

public interface DownlinkPacket {

	public static final int ID = -1;

	public boolean decodePacket(String filePath) throws IOException;

}
