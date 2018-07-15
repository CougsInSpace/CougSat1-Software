package space.cougs.ground;

import space.cougs.ground.gui.GUI;
import space.cougs.ground.packetprocessing.PacketHeader;
import space.cougs.ground.satellites.CougSat1;
import space.cougs.ground.satellites.CougSat;

public class CougSatGround {
	private static final GUI gui = new GUI();
	private static final CougSat1 cougSat1 = new CougSat1();
	private static final PacketHeader packetHeader = new PacketHeader();

	public static void main(String[] args) {
		packetHeader.addSatellite((CougSat) cougSat1);
		packetHeader.decodePacketSwitcher("test/rawPackets/TestTelemetry");
		
		gui.update(cougSat1);
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
}
