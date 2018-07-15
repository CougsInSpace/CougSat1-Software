package space.cougs.ground.packetprocessing;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import space.cougs.ground.packetprocessing.downlinkpackets.DownlinkPacket;
import space.cougs.ground.packetprocessing.downlinkpackets.Payload1Configuration;
import space.cougs.ground.packetprocessing.downlinkpackets.Payload2Configuration;
import space.cougs.ground.packetprocessing.downlinkpackets.Payload3Configuration;
import space.cougs.ground.packetprocessing.downlinkpackets.RadioConfiguration;
import space.cougs.ground.packetprocessing.downlinkpackets.Telemetry;
import space.cougs.ground.satellites.CougSat;

public class PacketHeader {

	private final List<CougSat> satellites = new ArrayList<CougSat>();
	private DownlinkPacket currentPacket;

	public PacketHeader() {

	}

	public boolean decodePacketSwitcher(String filePath) {

		File file = new File(filePath);
		int firstByte = -1;
		try {

			FileInputStream inFile = new FileInputStream(file);

			firstByte = inFile.read();

			inFile.close();

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		if (firstByte == -1) {
			System.out.printf("Error 4 - Incorrect filePath for %s", filePath);
			return false;
		}

		switch (firstByte & 0x1F) {

		case Telemetry.ID:
			currentPacket = new Telemetry();
			break;
		case RadioConfiguration.ID:
			break;
		case Payload1Configuration.ID:
			break;
		case Payload2Configuration.ID:
			break;
		case Payload3Configuration.ID:
			break;
		}

		for (CougSat satellite : satellites) {
			if (satellite.getID() == (firstByte & 0xE0)) {

				try {
					return currentPacket.decodePacket(file, satellite);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		return false;
	}

	public void addSatellite(CougSat satellite) {

		satellites.add(satellite);

	}

}
