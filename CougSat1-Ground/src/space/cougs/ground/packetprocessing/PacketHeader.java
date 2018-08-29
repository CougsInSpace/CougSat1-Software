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
import space.cougs.ground.utils.CISErrors;

public class PacketHeader {

	private final List<CougSat> satellites = new ArrayList<CougSat>();
	private DownlinkPacket currentPacket;
	private static int firstByte = -1;
	private static final int MASK_COMMAND_ID = 0x1F;
	private static final int MASK_SENDER_ID = 0xE0;//

	public PacketHeader() {

	}

	public boolean decodePacketSwitcher(String filePath) {

		File file = new File(filePath);

		try {

			FileInputStream inFile = new FileInputStream(file);

			firstByte = inFile.read();

			inFile.close();

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		if (firstByte == -1) {
			System.out.printf("Error %d - %s %s", CISErrors.FILE_NOT_FOUND, CISErrors.FILE_NOT_FOUND.toString(),
					filePath);
			return false;
		}

		switch (firstByte & MASK_COMMAND_ID) {

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
			if (satellite.getID() == (firstByte & MASK_SENDER_ID)) {

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
