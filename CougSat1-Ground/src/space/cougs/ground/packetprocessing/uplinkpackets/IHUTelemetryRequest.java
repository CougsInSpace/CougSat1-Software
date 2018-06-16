package space.cougs.ground.packetprocessing.uplinkpackets;

public class IHUTelemetryRequest implements UplinkPacket {

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

}
