package space.cougs.ground.packetprocessing;

public class PacketEncoder {

	public PacketEncoder() {

	}

	public void packetSwitcher(String filePath, int packet) {

		switch (packet) // packet will determine what type of packet we are
						// building
						//filePath only relevant for 
		{
		case (0):
			//multipacket
			break;
		case (1):
			//command 1
			break;
		case (2):
			//command 2
			break;
		}
	}

}
