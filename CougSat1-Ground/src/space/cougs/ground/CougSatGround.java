package space.cougs.ground;

import space.cougs.ground.gui.GUI;
import space.cougs.ground.packetprocessing.PacketHeader;
import space.cougs.ground.satellites.CougSat;
import space.cougs.ground.satellites.CougSat1;

public class CougSatGround {
	private static final GUI gui = new GUI();
	private static final CougSat1 cougSat1 = new CougSat1();
	private static final PacketHeader packetHeader = new PacketHeader();
	private static final String versionNumber = "1.0.0";

	public static void main(String[] args) {
		packetHeader.addSatellite((CougSat) cougSat1);
		packetHeader.decodePacketSwitcher("test/rawPackets/TestTelemetry");

		gui.update(cougSat1);

		// TODO
		// IFJRProgramUpload packet = new IFJRProgramUpload();
		// packet.setProgramLocation("test/CougSat1-PMIC.bin");
		// try {
		// packet.encodePacket();
		// } catch (IOException e) {
		// // TODO Auto-generated catch block
		// e.printStackTrace();
		// }

		// TODO
		// while (true) {
		//
		// try {
		//
		// Thread.sleep(1000);
		//
		// } catch (InterruptedException e) {
		// // TODO Auto-generated catch block
		// e.printStackTrace();
		// }
		// }
	}

	public static String getVersionnumber() {
		return versionNumber;
	}
	
}
