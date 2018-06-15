package space.cougs.ground.packetprocessing;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import space.cougs.ground.packetprocessing.packets.DownIHUShortTelemery;
import space.cougs.ground.packetprocessing.packets.Packet;

public class PacketHeader {

	private Packet currentPacket;

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

		case (DownIHUShortTelemery.ID):
			currentPacket = (Packet) new DownIHUShortTelemery();
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
