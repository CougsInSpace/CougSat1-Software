package space.cougs.ground.packetprocessing.packets;

import java.io.IOException;

public interface Packet {

	public static final int ID = -1;
	
	public void decodePacket(String filePath) throws IOException;

	public void encodePacket(String filePath);

}
