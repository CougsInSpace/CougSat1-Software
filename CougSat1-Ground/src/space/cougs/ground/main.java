package space.cougs.ground;

import java.io.IOException;

import space.cougs.ground.gui.GUI;

public class main {
	static GUI gui;
	
	public static void main(String[] args) {
		gui = new GUI();//initializes the Gui
		gui.updateData(new CougSat1Telemetry());
		
		PacketDecoder packetDecoder = new PacketDecoder();
		
		try {
			packetDecoder.parsePacket("test/rawPackets/TestTelemetry");
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public static GUI getGUI(){
		return gui;
	}
	
	
}
