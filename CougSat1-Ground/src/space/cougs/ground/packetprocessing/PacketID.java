package space.cougs.ground.packetprocessing;

public enum PacketID {
	
	IFJR_PROGRAM_UPLOAD(0x04);
	
	private final int ID;
	
	private PacketID(int ID)
	{
		this.ID = ID;
	}
	
	public int getID()
	{
		return ID;
	}
}
