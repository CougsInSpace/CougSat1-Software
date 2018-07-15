package space.cougs.ground.packetprocessing.downlinkpackets;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import space.cougs.ground.satellites.CougSat1Telemetry;
import space.cougs.ground.utils.FileUtils;
import space.cougs.ground.utils.Units;

public class Telemetry extends DownlinkPacket {

	public static final int ID = 0x28;
	private FileInputStream inFile = null;
	private CougSat1Telemetry telemetry = new CougSat1Telemetry();

	public Telemetry() {

	}

	public boolean decodePacket(File file) throws IOException {

		inFile = new FileInputStream(file);

		inFile.read();// Recipient/header/command ID
		inFile.read();// length

		telemetry.setMode(inFile.read());// mode
		telemetry.setIHUTemp(Units.rawToTemp(inFile.read()));// IHU Temp
		telemetry.setTime(Units.rawToTime(FileUtils.readNextBytes(inFile, 4)));
		telemetry.setSDCard(FileUtils.readNextBytes(inFile, 5));
		telemetry.setResetCount(inFile.read());
		telemetry.setErrorStatus(inFile.read());
		telemetry.setADCSTemp(Units.rawToTemp(inFile.read()));
		telemetry.setLattitude(Units.rawToGeographicCoordinate(FileUtils.readNextBytes(inFile, 4)));
		telemetry.setLongitude(Units.rawToGeographicCoordinate(FileUtils.readNextBytes(inFile, 4)));
		telemetry.setRoll(FileUtils.readNextBytes(inFile, 2));
		telemetry.setPitch(FileUtils.readNextBytes(inFile, 2));
		telemetry.setYaw(FileUtils.readNextBytes(inFile, 2));
		telemetry.setXPWMOut((int) FileUtils.readNextBytes(inFile, 2));
		telemetry.setYPWMOut((int) FileUtils.readNextBytes(inFile, 2));
		telemetry.setZPWMOut((int) FileUtils.readNextBytes(inFile, 2));
		telemetry.setXCurrent((int) FileUtils.readNextBytes(inFile, 2));
		telemetry.setYCurrent((int) FileUtils.readNextBytes(inFile, 2));
		telemetry.setZCurrent((int) FileUtils.readNextBytes(inFile, 2));
		telemetry.setIFJRTemp(Units.rawToTemp(inFile.read()));
		telemetry.setPMICTemp(Units.rawToTemp(inFile.read()));
		telemetry.setBatteryATemp(Units.rawToTemp(inFile.read()));
		telemetry.setBatteryBTemp(Units.rawToTemp(inFile.read()));
		telemetry.setReg3V3ATemp(Units.rawToTemp(inFile.read()));
		telemetry.setReg3V3BTemp(Units.rawToTemp(inFile.read()));
		for (int i = 0; i < 8; i++) {
			telemetry.setPVTemp(i, Units.rawToTemp(inFile.read()));
		}
		for (int i = 0; i < 8; i++) {
			telemetry.setMPPTTemp(i, Units.rawToTemp(inFile.read()));
		}
		for (int i = 0; i < 8; i++) {
			telemetry.setPVVoltage(i, Units.rawToVoltage(FileUtils.readNextBytes(inFile, 2)));
			telemetry.setPVCurrent(i, Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		}
		telemetry.setBatteryAVoltage(Units.rawToVoltage(FileUtils.readNextBytes(inFile, 2)));
		telemetry.setBatteryACurrent(Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		telemetry.setBatteryBVoltage(Units.rawToVoltage(FileUtils.readNextBytes(inFile, 2)));
		telemetry.setBatteryBCurrent(Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		telemetry.setReg3V3AVoltage(Units.rawToVoltage(FileUtils.readNextBytes(inFile, 2)));
		telemetry.setReg3V3ACurrent(Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		telemetry.setReg3V3BVoltage(Units.rawToVoltage(FileUtils.readNextBytes(inFile, 2)));
		telemetry.setReg3V3BCurrent(Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		for (int i = 0; i < 13; i++) {
			telemetry.setPR3V3Current(i, Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		}
		for (int i = 0; i < 7; i++) {
			telemetry.setPRBattCurrent(i, Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		}
		for (int i = 0; i < 4; i++) {
			telemetry.setPV3V3Current(i, Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		}
		telemetry.setPRBatteryHeaterACurrent(Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		telemetry.setPRBatteryHeaterBCurrent(Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		telemetry.setPRDeployablesCurrent(Units.rawToCurrent(FileUtils.readNextBytes(inFile, 2)));
		telemetry.setPVSwitchingState((FileUtils.readNextBytes(inFile, 2)));
		telemetry.setOutputSwitchingState(FileUtils.readNextBytes(inFile, 7));
		telemetry.setEnergyLevel(Units.rawToEnergy(inFile.read()));
		telemetry.setRCSTemp(Units.rawToTemp(inFile.read()));
		telemetry.setRXTemp(Units.rawToTemp(inFile.read()));
		telemetry.setTXTemp(Units.rawToTemp(inFile.read()));
		telemetry.setAMPTemp(Units.rawToTemp(inFile.read()));
		telemetry.setRXPower(Units.rawToPower(FileUtils.readNextBytes(inFile, 2)));
		telemetry.setRXSNR(Units.rawToDecibels(FileUtils.readNextBytes(inFile, 2)));
		telemetry.setBadPacketCount((int) FileUtils.readNextBytes(inFile, 2));
		telemetry.setRXCenterFrequency(Units.rawToFrequency(FileUtils.readNextBytes(inFile, 3)));
		telemetry.setTXCenterFrequency(Units.rawToFrequency(FileUtils.readNextBytes(inFile, 3)));
		telemetry.setTxPower(Units.rawToPower(FileUtils.readNextBytes(inFile, 2)));
		telemetry.setTxAmplifierVoltage(Units.rawToVoltage(FileUtils.readNextBytes(inFile, 2)));

		return true;
	}
}
