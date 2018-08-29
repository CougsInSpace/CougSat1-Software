package space.cougs.ground.packetprocessing.downlinkpackets;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import space.cougs.ground.satellites.CougSat;
import space.cougs.ground.utils.FileUtils;
import space.cougs.ground.utils.Units;

public class Telemetry extends DownlinkPacket {

	public static final int ID = 0x08;

	public Telemetry() {

	}

	public boolean decodePacket(File file, CougSat satellite) throws IOException {

		FileInputStream inFile = new FileInputStream(file);

		inFile.read();// Recipient/header/command ID
		inFile.read();// length

		satellite.setMode(inFile.read());// mode
		satellite.setIHUTemp(Units.rawToTemp(inFile.read()));// IHU Temp
		satellite.setTime(Units.rawToTime(FileUtils.readNextBytes(inFile, 4)));
		satellite.setSDCard(FileUtils.readNextBytes(inFile, 5));
		satellite.setResetCount(inFile.read());
		satellite.setErrorStatus(inFile.read());
		satellite.setADCSTemp(Units.rawToTemp(inFile.read()));
		satellite.setLattitude(Units.rawToGeographicCoordinate(FileUtils.readNextBytes(inFile, 4)));
		satellite.setLongitude(Units.rawToGeographicCoordinate(FileUtils.readNextBytes(inFile, 4)));
		satellite.setRoll(Units.rawToAngle(FileUtils.readNextBytes(inFile, 2)));
		satellite.setPitch(Units.rawToAngle(FileUtils.readNextBytes(inFile, 2)));
		satellite.setYaw(Units.rawToAngle(FileUtils.readNextBytes(inFile, 2)));
		satellite.setXPWMOut((int) FileUtils.readNextBytes(inFile, 2));
		satellite.setYPWMOut((int) FileUtils.readNextBytes(inFile, 2));
		satellite.setZPWMOut((int) FileUtils.readNextBytes(inFile, 2));
		satellite.setXCurrent(Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		satellite.setYCurrent(Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		satellite.setZCurrent(Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		satellite.setIFJRTemp(Units.rawToTemp(inFile.read()));
		satellite.setPMICTemp(Units.rawToTemp(inFile.read()));
		satellite.setBatteryATemp(Units.rawToTemp(inFile.read()));
		satellite.setBatteryBTemp(Units.rawToTemp(inFile.read()));
		satellite.setReg3V3ATemp(Units.rawToTemp(inFile.read()));
		satellite.setReg3V3BTemp(Units.rawToTemp(inFile.read()));
		for (int i = 0; i < 8; i++) {
			satellite.setPVTemp(i, Units.rawToTemp(inFile.read()));
		}
		for (int i = 0; i < 8; i++) {
			satellite.setMPPTTemp(i, Units.rawToTemp(inFile.read()));
		}
		for (int i = 0; i < 8; i++) {
			satellite.setPVVoltage(i, Units.rawToVoltage(FileUtils.readNextBytes(inFile, 2)));
			satellite.setPVCurrent(i, Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		}
		satellite.setBatteryAVoltage(Units.rawToVoltage(FileUtils.readNextBytes(inFile, 2)));
		satellite.setBatteryACurrent(Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		satellite.setBatteryBVoltage(Units.rawToVoltage(FileUtils.readNextBytes(inFile, 2)));
		satellite.setBatteryBCurrent(Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		satellite.setReg3V3AVoltage(Units.rawToVoltage(FileUtils.readNextBytes(inFile, 2)));
		satellite.setReg3V3ACurrent(Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		satellite.setReg3V3BVoltage(Units.rawToVoltage(FileUtils.readNextBytes(inFile, 2)));
		satellite.setReg3V3BCurrent(Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		for (int i = 0; i < 13; i++) {
			satellite.setPR3V3Current(i, Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		}
		for (int i = 0; i < 7; i++) {
			satellite.setPRBattCurrent(i, Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		}
		for (int i = 0; i < 4; i++) {
			satellite.setPV3V3Current(i, Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		}
		
		satellite.setPRBatteryHeaterACurrent(Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		satellite.setPRBatteryHeaterBCurrent(Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		satellite.setPRDeployablesCurrent(Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		satellite.setPVSwitchingState((FileUtils.readNextBytes(inFile, 2)));
		satellite.setOutputSwitchingState(FileUtils.readNextBytes(inFile, 7));
		satellite.setEnergyLevel(Units.rawToEnergy(inFile.read()));
		satellite.setCommsTemp(Units.rawToTemp(inFile.read()));
		satellite.setRXTemp(Units.rawToTemp(inFile.read()));
		satellite.setTXTemp(Units.rawToTemp(inFile.read()));
		satellite.setAMPTemp(Units.rawToTemp(inFile.read()));
		satellite.setRXPower(Units.rawToPower(FileUtils.readNextBytes(inFile, 2)));
		satellite.setRXSNR(Units.rawToDecibels(FileUtils.readNextBytes(inFile, 2)));
		satellite.setBadPacketCount((int) FileUtils.readNextBytes(inFile, 2));
		satellite.setRXCenterFrequency(Units.rawToFrequency(FileUtils.readNextBytes(inFile, 3)));
		satellite.setTXCenterFrequency(Units.rawToFrequency(FileUtils.readNextBytes(inFile, 3)));
		satellite.setTxPower(Units.rawToPower(FileUtils.readNextBytes(inFile, 2)));
		satellite.setTxAmplifierVoltage(Units.rawToVoltage(FileUtils.readNextBytes(inFile, 2)));

		int lastByte = inFile.read();
		inFile.close();

		return lastByte == -1;
	}
}
