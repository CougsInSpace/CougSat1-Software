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
		FileInputStream inStream = new FileInputStream(programFile);
		long programLength = programFile.length();

		String folderPath = "upPackets/" + timeStamp.toString() + "/";
		File destination = new File(folderPath);
		destination.mkdirs();

		// First packet
		int serialNumber = 0;
		FileOutputStream outStream = new FileOutputStream(new File(folderPath + serialNumber));
		outStream.write(PacketID.IFJR_PROGRAM_UPLOAD.getID());

		// 2 - packet header, 2 - multiPacket SerialNumber, 4 - fileSize,
		// 4 - CRC32, 1 - Null Character
		int headerLength = 2 + 2 + 4 + 4 + programPath.length() + 1;
		long packetLength = Math.min(getMaxPacketLength(), programLength + headerLength);
		int packetLengthDwords = (int) Math.ceil(packetLength / 4.0);
		outStream.write(packetLengthDwords);

		outStream.write((serialNumber >> 8) & 0xFF);
		outStream.write(serialNumber & 0xFF);

		outStream.write((int) (programLength >> 24) & 0xFF);
		outStream.write((int) (programLength >> 16) & 0xFF);
		outStream.write((int) (programLength >> 8) & 0xFF);
		outStream.write((int) (programLength >> 0) & 0xFF);

		int i = 0;
		while (programFile.length() > 0)// the other packets
		{

			serialNumber++;

			while (i < packetLength) {

				i++;
			}
		}

		inStream.close();
		outStream.close();

		return destination;
	}

	public void setProgramLocation(String programPath) {

		this.programPath = programPath;
	}

	// public File createPacket(File outFile)
	// {
	// return outFile;
	//
	// }
}
