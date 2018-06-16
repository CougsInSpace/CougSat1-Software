package space.cougs.ground.packetprocessing.downlinkpackets;

import java.io.IOException;

public class IHUShortTelemery implements DownlinkPacket {

	public static final int ID = 0x28;

	public IHUShortTelemery() {
		super();
	}

	@Override
	public boolean decodePacket(String filePath) throws IOException {

		// int buff;
		// FileInputStream infile = new FileInputStream(new File(filePath));
		//
		// buff = infile.read();
		//
		// infile.close();
		
		return false;
	}

}
