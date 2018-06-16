package space.cougs.ground.packetprocessing.downlinkpackets;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

import space.cougs.ground.satellites.CougSat1Telemetry;

public class Telemetry {

	private FileInputStream infile = null;
	private CougSat1Telemetry telemetry = new CougSat1Telemetry();

	public Telemetry() {

	}

	public void parsePacket(String filePath) throws IOException {

		infile = new FileInputStream(new File(filePath));

		int buff = (int) infile.read();// two f's strong buffer

		int sender = (buff >> 5) & 0b111;// checks for who is sending/recieving
											// packet

		switch (sender) {

		case 0:// ground
				// invalid
			break;
		case 1:// ADCS
			break;
		case 2:// IFJR
			break;
		case 3:// IHU
			parseIHU();
			break;
		case 4:// PMIC
			break;
		case 5:// RCS
			break;
		case 6:// payload - checks for multipacket
			break;
		case 7:// camera - checks for multipacket
		}

		// update telemetry

		// CougSatGround.getGUI().updateData(telemetry);
		infile.close();
	}

	private void parseIHU() throws IOException {
		infile.read();// length of packet - only relevent on multipacket

		int buff = infile.read();// command 0x81 or 0x80
		long longBuff = 0;// for longs - aka memory storage
		int buff2 = 0;// for the 12 bit voltage/current

		if (buff == 0x81) {// standard telemetry downlink

			buff = infile.read();// mode as enum type
			telemetry.setMode(buff);

			buff = infile.read();// time stuff
			buff = buff << 8;
			buff |= infile.read();
			buff = buff << 8;
			buff |= infile.read();
			buff = buff << 8;
			buff |= infile.read();
			telemetry.setTime(buff);

			buff = infile.read();// reset count
			telemetry.setResetCount(buff);

			buff = (byte) infile.read();// IHU temp
			telemetry.setIHUTemp(buff);

			longBuff = infile.read();// SD CARD
			longBuff = longBuff << 8;
			longBuff |= infile.read();
			longBuff = longBuff << 8;
			longBuff |= infile.read();
			longBuff = longBuff << 8;
			longBuff |= infile.read();
			longBuff = longBuff << 8;
			longBuff |= infile.read();
			telemetry.setIHUSdCard(longBuff);

			buff = infile.read();
			telemetry.setIHUStatus(buff);// IHUStatus

			buff = (byte) infile.read();
			telemetry.setADCSTemp(buff);// ADCS Temp

			buff = infile.read();// ADCS Status
			telemetry.setADCSStatus(buff);

			buff = infile.read();// lattitude
			buff = buff << 8;
			buff |= infile.read();
			buff = buff << 8;
			buff |= infile.read();
			buff = buff << 8;
			buff |= infile.read();
			telemetry.setLattitude(buff);

			buff = infile.read();// longitude
			buff = buff << 8;
			buff |= infile.read();
			buff = buff << 8;
			buff |= infile.read();
			buff = buff << 8;
			buff |= infile.read();
			telemetry.setLongitude(buff);

			buff = infile.read();// roll
			buff = buff << 8;
			buff |= infile.read();
			buff = buff << 8;
			buff |= infile.read();
			telemetry.setRoll(buff);

			buff = infile.read();// pitch
			buff = buff << 8;
			buff |= infile.read();
			buff = buff << 8;
			buff |= infile.read();
			telemetry.setPitch(buff);

			buff = infile.read();// yaw
			buff = buff << 8;
			buff |= infile.read();
			buff = buff << 8;
			buff |= infile.read();
			telemetry.setYaw(buff);

			buff = (byte) infile.read();// IFJR Temp
			telemetry.setIFJRTemp(buff);

			buff = (byte) infile.read();// PMIC Temp - power
			telemetry.setPMICTemp(buff);

			buff = infile.read();// voltage - b0
			buff = buff << 8;
			buff2 = infile.read();
			buff = buff + buff2;
			buff = buff >> 4;
			telemetry.setBattery0Voltage(buff, true);
			buff2 = buff2 << 8;// current - b0
			buff2 |= infile.read();
			buff2 = (buff2) & 0b111111111111;
			telemetry.setBattery0Current(buff2, true);

			buff = infile.read();// voltage - b1
			buff = buff << 8;
			buff2 = infile.read();
			buff = buff + buff2;
			buff = buff >> 4;
			telemetry.setBattery1Voltage(buff, true);
			buff2 = buff2 << 8;// current - b1
			buff2 |= infile.read();
			buff2 = (buff2) & 0b111111111111;
			telemetry.setBattery1Current(buff2, true);

			buff = (byte) infile.read();// battery 0 temp
			telemetry.setBattery0Temp(buff);

			buff = (byte) infile.read();// battery 1 temp
			telemetry.setBattery1Temp(buff);

			buff = infile.read();// battery 0 heat
			telemetry.setBattery0Heat(buff);

			buff = infile.read();// battery 1 heat
			telemetry.setBattery1Heat(buff);

			buff = infile.read();// voltage - sp0
			buff = buff << 8;
			buff2 = infile.read();
			buff = buff + buff2;
			buff = buff >> 4;
			telemetry.setSP0Voltage(buff, true);
			buff2 = buff2 << 8;// current - sp0
			buff2 |= infile.read();
			buff2 = (buff2) & 0b111111111111;
			telemetry.setSP0Current(buff2, true);

			buff = infile.read();// voltage - sp1
			buff = buff << 8;
			buff2 = infile.read();
			buff = buff + buff2;
			buff = buff >> 4;
			telemetry.setSP1Voltage(buff, true);
			buff2 = buff2 << 8;// current - sp1
			buff2 |= infile.read();
			buff2 = (buff2) & 0b111111111111;
			telemetry.setSP1Current(buff2, true);

			buff = infile.read();// voltage - sp2
			buff = buff << 8;
			buff2 = infile.read();
			buff = buff + buff2;
			buff = buff >> 4;
			telemetry.setSP2Voltage(buff, true);
			buff2 = buff2 << 8;// current - sp2
			buff2 |= infile.read();
			buff2 = (buff2) & 0b111111111111;
			telemetry.setSP2Current(buff2, true);

			buff = infile.read();// voltage - sp3
			buff = buff << 8;
			buff2 = infile.read();
			buff = buff + buff2;
			buff = buff >> 4;
			telemetry.setSP3Voltage(buff, true);
			buff2 = buff2 << 8;// current - sp3
			buff2 |= infile.read();
			buff2 = (buff2) & 0b111111111111;
			telemetry.setSP3Current(buff2, true);

			buff = infile.read();// total power coming out of solar converters
			buff = buff << 8;
			buff |= infile.read();
			telemetry.setPowerIn(buff);

			buff = infile.read();// total power being used on the sat
			buff = buff << 8;
			buff |= infile.read();
			telemetry.setPowerOut(buff);

			buff = infile.read();// voltage - 3.3v rail
			buff = buff << 8;
			buff2 = infile.read();
			buff = buff | buff2;
			buff = buff >> 4;
			telemetry.set3v3RailVoltage(buff, true);
			buff2 = buff2 << 8;// current - 3.3v rail
			buff2 |= infile.read();
			buff2 = (buff2) & 0b111111111111;
			telemetry.set3v3RailCurrent(buff2, true);

			buff = infile.read();// voltage - 5.0v rail
			buff = buff << 8;
			buff2 = infile.read();
			buff = buff + buff2;
			buff = buff >> 4;
			telemetry.set5vRailVoltage(buff, true);
			buff2 = buff2 << 8;// current - 5.0v rail
			buff2 |= infile.read();
			buff2 = (buff2) & 0b111111111111;
			telemetry.set5vRailCurrent(buff2, true);

			buff = infile.read();// Channels
			buff = buff << 8;
			buff |= infile.read();
			telemetry.setEPSChannels(buff);

			buff = (byte) infile.read();// RCS Temp
			telemetry.setRCSTemp(buff);

			buff = infile.read();
			telemetry.setRCSStatus(buff); // RCS Status

			buff = infile.read();
			telemetry.setTXPower(buff); // TX Power

			buff = infile.read();
			telemetry.setRXPower(buff); // RX Power

			buff = infile.read();
			telemetry.setRXSNR(buff); // RX Signal Noise Ratio

			buff = infile.read();
			buff = buff << 8;
			buff |= infile.read();
			telemetry.setPayloadFrames(buff); // # of payload frames ready to
												// download - aka pictures

			infile.close();

		} else if (buff == 0x80) {// diagnostic report
			// tbd
		}
	}
}
