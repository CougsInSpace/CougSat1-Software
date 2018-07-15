package space.cougs.ground.packetprocessing.downlinkpackets;

import java.io.File;
import java.io.IOException;

public abstract class DownlinkPacket {

	public static final int ID = -1;
	private boolean packetDecoded = false;

	public abstract boolean decodePacket(File file) throws IOException;

	boolean isPacketDecoded() {
		return packetDecoded;
	}

	final void setPacketDecoded(boolean value) {
		packetDecoded = value;
	}
}
