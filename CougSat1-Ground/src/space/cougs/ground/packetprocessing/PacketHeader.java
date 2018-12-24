package space.cougs.ground.packetprocessing;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import space.cougs.ground.packetprocessing.downlinkpackets.DownlinkPacket;
import space.cougs.ground.packetprocessing.downlinkpackets.Payload1Configuration;
import space.cougs.ground.packetprocessing.downlinkpackets.Payload2Configuration;
import space.cougs.ground.packetprocessing.downlinkpackets.Payload3Configuration;
import space.cougs.ground.packetprocessing.downlinkpackets.RadioConfiguration;
import space.cougs.ground.packetprocessing.downlinkpackets.Telemetry;
import space.cougs.ground.satellites.CougSat;
import space.cougs.ground.utils.CISErrors;

public class PacketHeader {
  private static final int MASK_COMMAND_ID = 0x1F;
  private static final int MASK_SENDER_ID  = 0xE0;

  private final List<CougSat> satellites = new ArrayList<CougSat>();

  private DownlinkPacket currentPacket;

  public PacketHeader() {}

  public CISErrors decodePacket(String filePath) {
    File file = new File(filePath);
    if (!file.exists()) {
      System.out.printf("Decoding packet file not found: %s\n", filePath);
      return CISErrors.FILE_NOT_FOUND;
    }

    int buf = 0;

    try {
      FileInputStream inFile = new FileInputStream(file);

      buf = inFile.read();

      inFile.close();

    } catch (IOException e) {
      System.out.printf("Failed to read file for packet decoding: %s\n",
          file.getAbsolutePath());
      e.printStackTrace();
    }

    switch (buf & MASK_COMMAND_ID) {
      case Telemetry.ID:
        currentPacket = new Telemetry();
        break;
      case RadioConfiguration.ID:
        currentPacket = new RadioConfiguration();
        break;
      case Payload1Configuration.ID:
        currentPacket = new Payload1Configuration();
        break;
      case Payload2Configuration.ID:
        currentPacket = new Payload2Configuration();
        break;
      case Payload3Configuration.ID:
        currentPacket = new Payload3Configuration();
        break;
    }

    CougSat reciepient = null;
    for (CougSat satellite : satellites) {
      if (satellite.getID() == (buf & MASK_SENDER_ID)) {
        reciepient = satellite;
      }
    }
    if (reciepient == null) {
      return CISErrors.INVALID_ADDRESS;
    }

    CougSat   temp   = reciepient.deepCopy();
    CISErrors result = currentPacket.decodePacket(file, temp);
    if (result == CISErrors.SUCCESS) {
      // Make the changes iff the decode was successful
      satellites.remove(reciepient);
      satellites.add(temp);
    }
    return result;
  }

  public void addSatellite(CougSat satellite) {
    satellites.add(satellite);
  }

  public List<CougSat> getSatellites(){
    return satellites;
  }
}
