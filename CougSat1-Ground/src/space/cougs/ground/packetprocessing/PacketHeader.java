package space.cougs.ground.packetprocessing;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import space.cougs.ground.packetprocessing.downlinkpackets.IHUShortTelemery;
import space.cougs.ground.packetprocessing.downlinkpackets.DownlinkPacket;

public class PacketHeader {

	private DownlinkPacket currentPacket;

	public PacketHeader() {

	}

	public void decodePacketSwitcher(String filePath) {

		int firstByte = -1;
		try {

			FileInputStream infile = new FileInputStream(new File(filePath));

			firstByte = infile.read();

			infile.close();

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		if (firstByte == -1) {
			System.out.printf("Error 4 - Incorrect filePath for %s", filePath);
			return;
		}

		switch (firstByte) {

		case (IHUShortTelemery.ID):
			currentPacket = (DownlinkPacket) new IHUShortTelemery();
			break;
		// case ()
		// break;
		}

		try {
			currentPacket.decodePacket(filePath);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

}
