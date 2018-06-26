package space.cougs.ground.packetprocessing.uplinkpackets;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import com.sun.jmx.snmp.Timestamp;

import space.cougs.ground.packetprocessing.PacketID;

public class IFJRProgramUpload extends UplinkPacket {

	private String programPath;

	@Override
	public File encodePacket() throws IOException {

		if (!isDataLoaded()) {
			return null;
		}

		Timestamp timeStamp = new Timestamp(System.currentTimeMillis());
		File programFile = new File(programPath);
		FileInputStream inFile = new FileInputStream(programFile);
		long programLength = programFile.length();

		String folderPath = "upPackets/" + timeStamp.toString() + "/";
		File destination = new File(folderPath);
		destination.mkdirs();

		// First packet
		int serialNumber = 0;
		FileOutputStream outFile = new FileOutputStream(new File(folderPath + serialNumber));
		outFile.write(PacketID.IFJR_PROGRAM_UPLOAD.getID());

		// 2 - packet header, 2 - multiPacket SerialNumber, 4 - fileSize,
		// 4 - CRC32, 1 - Null Character
		int headerLength = 2 + 2 + 4 + 4 + programPath.length() + 1;
		long packetLength = Math.min(getMaxPacketLength(), programLength + headerLength);

		while (true)// the other packets
		{
			while (true) {

			}
		}

		return destination;

	}

	public void setProgramLocation(String programPath) {

		this.programPath = programPath;
	}
}
