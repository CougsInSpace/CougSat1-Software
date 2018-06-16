package space.cougs.ground.packetprocessing.uplinkpackets;

import java.io.IOException;

public interface UplinkPacket {

	public static final int ID = -1;

	public boolean encodePacket(String filePath) throws IOException;

	boolean isDataLoaded();
}
