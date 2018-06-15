package space.cougs.ground.packetprocessing.packets;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

public class DownIHUShortTelemery implements Packet {

	public static final int ID = 0x28;

	public DownIHUShortTelemery() {
		super();
	}

	public void decodePacket(String filePath) throws IOException {

		FileInputStream infile = new FileInputStream(new File(filePath));

	}

	public void encodePacket(String filePath) {

	}

}
