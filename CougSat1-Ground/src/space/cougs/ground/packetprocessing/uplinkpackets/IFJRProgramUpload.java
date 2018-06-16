package space.cougs.ground.packetprocessing.uplinkpackets;

import java.io.IOException;

public class IFJRProgramUpload implements UplinkPacket {

	@Override
	public boolean encodePacket(String filePath) throws IOException {
		if (!isDataLoaded())
		{
			return false;
		}
//		while( != -1)
		return false;
		
		
	}

	@Override
	public boolean isDataLoaded() {
		// TODO Auto-generated method stub
		return false;
	}

}
