package space.cougs.ground.packetprocessing.downlinkpackets;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import space.cougs.ground.satellites.ADCS;
import space.cougs.ground.satellites.CDH;
import space.cougs.ground.satellites.Comms;
import space.cougs.ground.satellites.CougSat;
import space.cougs.ground.satellites.ECS;
import space.cougs.ground.satellites.EPS;
import space.cougs.ground.utils.CISErrors;
import space.cougs.ground.utils.FileUtils;

public class Telemetry extends DownlinkPacket {
  public static final int  ID     = 0x08;
  private static final int LENGTH = 0x3B;

  public Telemetry() {}

  @Override
  public String toString() {
    return "Telemetry";
  }

  public CISErrors decodePacket(File file, CougSat satellite) {
    FileInputStream inFile        = null;
    CISErrors       result        = CISErrors.SUCCESS;
    CougSat         tempSatellite = satellite;
    try {
      inFile = new FileInputStream(file);

      inFile.read(); // Recipient/header/command ID
      int buf = inFile.read();
      if (buf != LENGTH) {
        System.out.printf(
            "%d does not expected %d length of telemetry packet from %s\n", buf,
            LENGTH, file.getAbsolutePath());
        inFile.close();
        return CISErrors.INVALID_ARGS;
      }

      decodePacketADCS(inFile, tempSatellite);
      decodePacketCDH(inFile, tempSatellite);
      decodePacketComms(inFile, tempSatellite);
      decodePacketECS(inFile, tempSatellite);
      decodePacketEPS(inFile, tempSatellite);

      inFile.read(); // Padding
      inFile.read(); // Padding

      if (inFile.available() != 0) {
        result = CISErrors.EOF;
        System.out.printf(
            "Finished parsing telemetry packet but %d bytes remain\n",
            inFile.available());
      }
      inFile.close();
    } catch (IOException e) {
      System.out.printf(
          "Failed to decode telemetry from %s\n", file.getAbsolutePath());
      e.printStackTrace();
    }
    if (result == CISErrors.SUCCESS) {
      satellite = tempSatellite;
      System.out.printf("%s updated with telemetry packet\n", satellite.toString());
    }
    return result;
  }

  private void decodePacketADCS(FileInputStream file, CougSat satellite)
      throws IOException {
    ADCS adcs = satellite.getADCS();
    adcs.setLatitude(FileUtils.readNextGeographicCoordinate(file));
    adcs.setLongitude(FileUtils.readNextGeographicCoordinate(file));
    adcs.setRoll(FileUtils.readNextEulerAngle(file));
    adcs.setPitch(FileUtils.readNextEulerAngle(file));
    adcs.setYaw(FileUtils.readNextEulerAngle(file));
    adcs.setXPWMOut((int)FileUtils.readNextBytes(file, 2));
    adcs.setYPWMOut((int)FileUtils.readNextBytes(file, 2));
    adcs.setZPWMOut((int)FileUtils.readNextBytes(file, 2));
    adcs.setXCurrent(FileUtils.readNextCurrent(file));
    adcs.setYCurrent(FileUtils.readNextCurrent(file));
    adcs.setZCurrent(FileUtils.readNextCurrent(file));
  }

  private void decodePacketCDH(FileInputStream file, CougSat satellite)
      throws IOException {
    CDH cdh = satellite.getCDH();
    cdh.setMode((int)FileUtils.readNextBytes(file, 1));
    cdh.setTime(FileUtils.readNextBytes(file, 4));
    cdh.setSDCard(FileUtils.readNextBytes(file, 5));
    cdh.setResetCount((int)FileUtils.readNextBytes(file, 1));
    cdh.setErrorStatus((int)FileUtils.readNextBytes(file, 1));
  }

  private void decodePacketComms(FileInputStream file, CougSat satellite)
      throws IOException {
    Comms comms = satellite.getComms();
    comms.setRXPower(FileUtils.readNextPower(file));
    comms.setTX230Power(FileUtils.readNextPower(file));
    comms.setTX700Power(FileUtils.readNextPower(file));
    comms.setRXSNR(FileUtils.readNextDecibels(file));
    comms.setRXCenterFrequency(FileUtils.readNextFrequency(file));
    comms.setTX230CenterFrequency(FileUtils.readNextFrequency(file));
    comms.setTX700CenterFrequency(FileUtils.readNextFrequency(file));
    comms.setReg5V0Voltage(FileUtils.readNextVoltage(file));
    comms.setReg5V0Current(FileUtils.readNextCurrent(file));
    comms.setReg9V0Voltage(FileUtils.readNextVoltage(file));
    comms.setReg9V0Current(FileUtils.readNextCurrent(file));
    comms.setPR3V3Current(0, FileUtils.readNextCurrent(file));
    comms.setPR3V3Current(1, FileUtils.readNextCurrent(file));
    comms.setPR3V3Current(2, FileUtils.readNextCurrent(file));
    comms.setPR5V0Current(0, FileUtils.readNextCurrent(file));
    comms.setPR5V0Current(1, FileUtils.readNextCurrent(file));
    comms.setPR5V0Current(2, FileUtils.readNextCurrent(file));
    comms.setPR9V0Current(0, FileUtils.readNextCurrent(file));
    comms.setPR9V0Current(1, FileUtils.readNextCurrent(file));
    comms.setBadPacketCount((int)FileUtils.readNextBytes(file, 2));
  }

  private void decodePacketECS(FileInputStream file, CougSat satellite)
      throws IOException {
    ECS ecs = satellite.getECS();
    ecs.setADCSTemp(FileUtils.readNextTemperature(file));
    ecs.setIHUTemp(FileUtils.readNextTemperature(file));
    ecs.setIFJRTemp(FileUtils.readNextTemperature(file));
    ecs.setCommsTemp(FileUtils.readNextTemperature(file));
    ecs.setRXTemp(FileUtils.readNextTemperature(file));
    ecs.setTX230Temp(FileUtils.readNextTemperature(file));
    ecs.setTX700Temp(FileUtils.readNextTemperature(file));
    ecs.setReg5V0Temp(FileUtils.readNextTemperature(file));
    ecs.setReg9V0Temp(FileUtils.readNextTemperature(file));
    ecs.setPMICTemp(FileUtils.readNextTemperature(file));
    ecs.setBatteryTemp(0, FileUtils.readNextTemperature(file));
    ecs.setBatteryTemp(1, FileUtils.readNextTemperature(file));
    ecs.setReg3V3Temp(0, FileUtils.readNextTemperature(file));
    ecs.setReg3V3Temp(1, FileUtils.readNextTemperature(file));
    for (int i = 0; i < 8; i++) {
      ecs.setPVTemp(i, FileUtils.readNextTemperature(file));
    }
    for (int i = 0; i < 8; i++) {
      ecs.setMPPTTemp(i, FileUtils.readNextTemperature(file));
    }
  }

  private void decodePacketEPS(FileInputStream file, CougSat satellite)
      throws IOException {
    EPS eps = satellite.getEPS();
    for (int i = 0; i < 8; i++) {
      eps.setPVVoltage(i, FileUtils.readNextVoltage(file));
    }
    for (int i = 0; i < 8; i++) {
      eps.setPVCurrent(i, FileUtils.readNextCurrent(file));
    }
    for (int i = 0; i < 8; i++) {
      eps.setMPPTCurrent(i, FileUtils.readNextCurrent(file));
    }
    eps.setBatteryVoltage(0, FileUtils.readNextVoltage(file));
    eps.setBatteryVoltage(1, FileUtils.readNextVoltage(file));
    eps.setBatteryCurrent(0, FileUtils.readNextCurrent(file));
    eps.setBatteryCurrent(1, FileUtils.readNextCurrent(file));
    eps.setReg3V3Voltage(0, FileUtils.readNextVoltage(file));
    eps.setReg3V3Voltage(1, FileUtils.readNextVoltage(file));
    eps.setReg3V3Current(0, FileUtils.readNextCurrent(file));
    eps.setReg3V3Current(1, FileUtils.readNextCurrent(file));
    for (int i = 0; i < 13; i++) {
      eps.setPR3V3Current(i, FileUtils.readNextCurrent(file));
    }
    for (int i = 0; i < 7; i++) {
      eps.setPRBattCurrent(i, FileUtils.readNextCurrent(file));
    }
    for (int i = 0; i < 4; i++) {
      eps.setPV3V3Current(i, FileUtils.readNextCurrent(file));
    }
    eps.setPRBHCurrent(0, FileUtils.readNextCurrent(file));
    eps.setPRBHCurrent(1, FileUtils.readNextCurrent(file));
    eps.setPRDeployablesCurrent(FileUtils.readNextCurrent(file));
    long buf = FileUtils.readNextBytes(file, 2);
    for (int i = 0; i < 8; i++) {
      boolean pathA = (buf & (1 << (2 * i))) != 0;
      boolean pathB = (buf & (1 << (2 * i + 1))) != 0;
      eps.setMPPTSwitchingState(i, pathA, pathB);
    }
    buf = FileUtils.readNextBytes(file, 5);
    for (int i = 0; i < 13; i++) {
      boolean pathA = (buf & (1 << (2 * i))) != 0;
      boolean pathB = (buf & (1 << (2 * i + 1))) != 0;
      eps.setPR3V3SwitchingState(i, pathA, pathB);
    }
    for (int i = 0; i < 7; i++) {
      boolean pathA = (buf & (1 << (2 * i + 26))) != 0;
      boolean pathB = (buf & (1 << (2 * i + 27))) != 0;
      eps.setPRBattSwitchingState(i, pathA, pathB);
    }
    buf = FileUtils.readNextBytes(file, 2);
    for (int i = 0; i < 4; i++) {
      boolean pathA = (buf & (1 << (2 * i))) != 0;
      boolean pathB = (buf & (1 << (2 * i + 1))) != 0;
      eps.setPV3V3SwitchingState(i, pathA, pathB);
    }
    for (int i = 0; i < 2; i++) {
      boolean pathA = (buf & (1 << (2 * i + 8))) != 0;
      boolean pathB = (buf & (1 << (2 * i + 9))) != 0;
      eps.setPRBatteryHeaterSwitchingState(i, pathA, pathB);
    }
    boolean pathA = (buf & (1 << 12)) != 0;
    boolean pathB = (buf & (1 << 13)) != 0;
    eps.setPRDeployablesSwitchingState(pathA, pathB);
  }
}
