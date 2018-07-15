package space.cougs.ground;

import java.io.File;
import java.io.IOException;

import space.cougs.ground.gui.GUI;
import space.cougs.ground.packetprocessing.downlinkpackets.Telemetry;

public class CougSatGround {
	static GUI gui;

	public static void main(String[] args) {
		gui = new GUI();// initializes the Gui
		// gui.updateData(new CougSat1Telemetry());

		// PacketDecoder packetDecoder = new PacketDecoder();
		Telemetry telemetry = new Telemetry();

		while (true) {
			// File[] files = new
			// File("test/rawPackets/TestTelemetry").listFiles();
			// for (File file : files) {
			// /* do something with this file */
			//
			// // and delete it when finished
			//
			// }

			// Pause for a second before checking again

			// int bytes[] = { 0x12, 0x34, 0x56, 0x78, 0x90 };
			// String string = "Math is not fun!";
			// CRC32 crcTest = new CRC32();
			//
			// for (int b : string.getBytes()) {
			// crcTest.update(b);
			// }
			//
			// int test = crcTest.getCheckSum();
			//
			// System.out.printf("%X\n", test);
			try {

				telemetry.decodePacket(new File("test/rawPackets/TestTelemetry"));

			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			try {

				Thread.sleep(1000);

			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
	}

	public static GUI getGUI() {
		return gui;
	}

}
