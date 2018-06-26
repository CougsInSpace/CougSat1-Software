package space.cougs.ground.packetprocessing.uplinkpackets;

import java.io.File;
import java.io.IOException;

public class IHUTelemetryRequest extends UplinkPacket {

	public boolean encodePacket(String filePath) {
		if (!isDataLoaded()) {
			return false;
		}

		return false;
	}

	@Override
	public boolean isDataLoaded() {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public File encodePacket() throws IOException {
		// TODO Auto-generated method stub
		return null;
	}

}
