package space.cougs.ground;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class PacketDecoder {
	private FileReader infile = null;
	private CougSat1Telemetry telemetry = new CougSat1Telemetry();

	public PacketDecoder() {

	}

	public void parsePacket(String filePath) throws IOException {

		infile = new FileReader(new File(filePath));

		int buff = (int) infile.read();// two f's strong buffer

		int sender = (buff >> 5) & 0b111;//checks for who is sending/recieving packet

		switch(sender)
		{
		
		case 0://ground
			//invalid
			break;
		case 1://ADCS
			break;
		case 2://IFJR
			break;
		case 3://IHU
			parseIHU();
			break;
		case 4://PMIC
			break;
		case 5://RCS
			break;
		case 6://payload - checks for multipacket
			break;
		case 7://camera - checks for multipacket
		}
		
		// update telemetry

		main.getGUI().updateData(telemetry);
	}
	private void parseIHU() throws IOException
	{
		infile.read();//length of packet - only relevent on multipacket 
		
		infile.read();
	}
}
