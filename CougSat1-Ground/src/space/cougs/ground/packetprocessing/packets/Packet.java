package space.cougs.ground.packetprocessing.packets;

public interface Packet {
	
	public void decodePacket(String filePath);
	
	public void encodePacket(String filePath);
	
}
