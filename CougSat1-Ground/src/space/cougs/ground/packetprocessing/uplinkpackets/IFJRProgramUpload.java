package space.cougs.ground.packetprocessing.uplinkpackets;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import com.sun.jmx.snmp.Timestamp;

import space.cougs.ground.utils.CRC32;

public class IFJRProgramUpload extends UplinkPacket {

	private final String programPath;
	private static final int IFJR_PROGRAM_UPLOAD = 0x04;

	public IFJRProgramUpload(String programPath) {

		this.programPath = programPath;
		setDataLoaded(true);
	}

	@Override
	public File encodePacket() throws IOException {

		if (!isDataLoaded()) {
			return null;
		}

		Timestamp timeStamp = new Timestamp(System.currentTimeMillis());
		File programFile = new File(programPath);

		long programLength = programFile.length();

		String folderPath = "UplinkPackets/" + timeStamp.toString() + "/";
		File destination = new File(folderPath);
		destination.mkdirs();

		FileInputStream inStream = new FileInputStream(programFile);

		// First packet
		int serialNumber = 0;
		FileOutputStream outStream = new FileOutputStream(new File(folderPath + serialNumber));
		outStream.write(IFJR_PROGRAM_UPLOAD);

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

		CRC32 crc = new CRC32();
		crc.update(programFile);
		int checkSum = crc.getCheckSum();
		outStream.write((int) (checkSum >> 24) & 0xFF);
		outStream.write((int) (checkSum >> 16) & 0xFF);
		outStream.write((int) (checkSum >> 8) & 0xFF);
		outStream.write((int) (checkSum >> 0) & 0xFF);

		packetLength -= 12;
		programLength -= 12;

		while (packetLength > 0) {
			outStream.write(inStream.read());
			packetLength--;
			programLength--;
		}

		while (programFile.length() > 0)// the other packets
		{
			packetLength = Math.min(getMaxPacketLength(), programLength + 4);
			serialNumber++;
			outStream.write((serialNumber >> 8) & 0xFF);
			outStream.write(serialNumber & 0xFF);

			while (packetLength > 0) {
				outStream.write(inStream.read());
				packetLength--;
				programLength--;
			}
		}

		inStream.close();
		outStream.close();

		return destination;
	}

}
