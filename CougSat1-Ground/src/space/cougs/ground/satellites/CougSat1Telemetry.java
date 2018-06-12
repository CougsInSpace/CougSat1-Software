package space.cougs.ground.satellites;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.TimeZone;

import space.cougs.ground.utils.CISErrors;
import space.cougs.ground.utils.CISErrors.errors;

public class CougSat1Telemetry {

	private ModeEnum mMode = ModeEnum.NO_CONNECTION;
	private Calendar mTime = Calendar.getInstance();
	private int mResetCount = 0;
	private int mIHUTemp = 0;
	private long mIHUSdCard = 0;
	private int mADCSTemp = 0;
	private errors mADCSStatus = CISErrors.errors.SUCCESS;
	private double mLattitude = 0.0;
	private double mLongitude = 0.0;
	private double mRoll = 0.0;
	private double mPitch = 0.0;
	private double mYaw = 0.0;
	private int mIFJRTemp = 0;
	private int mPMICTemp = 0;
	private double mBattery0Voltage = 0.0;
	private double mBattery0Current = 0.0;
	private double mBattery1Voltage = 0.0;
	private double mBattery1Current = 0.0;
	private int mBattery0Temp = 0;
	private int mBattery1Temp = 0;
	private double mBattery0Heat = 0.0;
	private double mBattery1Heat = 0.0;
	private double mSP0Voltage = 0.0;
	private double mSP1Voltage = 0.0;
	private double mSP2Voltage = 0.0;
	private double mSP3Voltage = 0.0;
	private double mSP0Current = 0.0;
	private double mSP1Current = 0.0;
	private double mSP2Current = 0.0;
	private double mSP3Current = 0.0;
	private double mPowerIn = 0.0;
	private double mPowerOut = 0.0;
	private double m3v3RailVoltage = 0.0;
	private double m3v3RailCurrent = 0.0;
	private double m5vRailVoltage = 0.0;
	private double m5vRailCurrent = 0.0;
	private int mEPSChannels = 0;
	private int mRCSTemp = 0;
	private errors mRCSStatus = CISErrors.errors.SUCCESS;
	private double mTXPower = 0.0;
	private double mRXPower = 0.0;
	private double mRXSNR = 0.0;
	private int mPayloadFrames = 0;
	private errors mIHUStatus = CISErrors.errors.SUCCESS;

	enum ModeEnum {
		BEACON, CHARGING, DATA_TRANSMISION, SCIENCE, ECLIPSE, SAFE, NO_CONNECTION;
	}

	public CougSat1Telemetry() {

	}

	private double rawToVoltage(int raw, boolean lowRange) {

		if (lowRange) {
			return (raw * 0.002);
		}

		return (raw * 0.0001);

	}

	private double rawToCurrent(int raw, boolean lowRange) {

		if (lowRange) {
			return (raw * 0.001);
		}

		return (raw * 0.0001);

	}

	public String getMode() {
		return mMode.name();
	}

	public void setMode(int buff) {

		switch (buff) {
		case 0x31:
			mMode = ModeEnum.BEACON;
			break;
		case 0x32:
			mMode = ModeEnum.CHARGING;
			break;
		case 0x33:
			mMode = ModeEnum.DATA_TRANSMISION;
			break;
		case 0x34:
			mMode = ModeEnum.SCIENCE;
			break;
		case 0x35:
			mMode = ModeEnum.ECLIPSE;
			break;
		case 0x40:
			mMode = ModeEnum.SAFE;
			break;
		}

	}

	public String getTime() {

		SimpleDateFormat format = new SimpleDateFormat("HH:mm:ss MM-dd-YYYY");

		format.setTimeZone(TimeZone.getTimeZone("UTC"));

		return format.format(mTime.getTime());
	}

	public void setTime(int buff) {

		mTime.setTimeInMillis((long) buff * 1000);

	}

	public int getResetCount() {
		return mResetCount;
	}

	public void setResetCount(int mResetCount) {
		this.mResetCount = mResetCount;
	}

	public int getIHUTemp() {
		return mIHUTemp;
	}

	public void setIHUTemp(int mIHUTemp) {

		this.mIHUTemp = mIHUTemp;
	}

	public long getIHUSdCard() {
		return mIHUSdCard;
	}

	public void setIHUSdCard(long mIHUSdCard) {
		this.mIHUSdCard = mIHUSdCard;
	}

	public int getADCSTemp() {
		return mADCSTemp;
	}

	public void setADCSTemp(int mADCSTemp) {
		this.mADCSTemp = mADCSTemp;
	}

	public errors getADCSStatus() {
		return mADCSStatus;
	}

	public void setADCSStatus(int index) {

		mADCSStatus = errors.values()[index];
	}

	public double getLattitude() {
		return mLattitude;
	}

	public void setLattitude(double mLattitude) {

		this.mLattitude = (mLattitude / 600000);
		// System.out.println(this.mLattitude);
	}

	public double getLongitude() {
		return mLongitude;
	}

	public void setLongitude(double mLongitude) {
		this.mLongitude = (mLongitude / 600000);
	}

	public double getRoll() {
		return mRoll;
	}

	public void setRoll(double mRoll) {

		this.mRoll = (mRoll / 60.0);
	}

	public double getPitch() {
		return mPitch;
	}

	public void setPitch(double mPitch) {

		this.mPitch = mPitch / 60.0;
	}

	public double getYaw() {
		return mYaw;
	}

	public void setYaw(double mYaw) {
		this.mYaw = mYaw / 60.0;
	}

	public int getIFJRTemp() {
		return mIFJRTemp;
	}

	public void setIFJRTemp(int mIFJRTemp) {
		this.mIFJRTemp = mIFJRTemp;
	}

	public int getPMICTemp() {
		return mPMICTemp;
	}

	public void setPMICTemp(int mPMICTemp) {
		this.mPMICTemp = mPMICTemp;
	}

	public double getBattery0Voltage() {
		return mBattery0Voltage;
	}

	public void setBattery0Voltage(int mBattery0Voltage, boolean lowRange) {

		this.mBattery0Voltage = rawToVoltage(mBattery0Voltage, lowRange);
	}

	public double getBattery0Current() {
		return mBattery0Current;
	}

	public void setBattery0Current(int mBattery0Current, boolean lowRange) {

		this.mBattery0Current = rawToCurrent(mBattery0Current, lowRange);
	}

	public double getBattery1Voltage() {
		return mBattery1Voltage;
	}

	public void setBattery1Voltage(int mBattery1Voltage, boolean lowRange) {

		this.mBattery1Voltage = rawToVoltage(mBattery1Voltage, lowRange);
	}

	public double getBattery1Current() {
		return mBattery1Current;
	}

	public void setBattery1Current(int mBattery1Current, boolean lowRange) {

		this.mBattery1Current = rawToCurrent(mBattery1Current, lowRange);
	}

	public int getBattery0Temp() {
		return mBattery0Temp;
	}

	public void setBattery0Temp(int mBattery0Temp) {
		this.mBattery0Temp = mBattery0Temp;
	}

	public int getBattery1Temp() {
		return mBattery1Temp;
	}

	public void setBattery1Temp(int mBattery1Temp) {
		this.mBattery1Temp = mBattery1Temp;
	}

	public double getSP0Voltage() {
		return mSP0Voltage;
	}

	public void setSP0Voltage(int mSP0Voltage, boolean lowRange) {

		this.mSP0Voltage = rawToVoltage(mSP0Voltage, lowRange);
		// System.out.println(this.mSP0Voltage);
	}

	public double getSP1Voltage() {
		return mSP1Voltage;
	}

	public void setSP1Voltage(int mSP1Voltage, boolean lowRange) {

		this.mSP1Voltage = rawToVoltage(mSP1Voltage, lowRange);
	}

	public double getSP2Voltage() {
		return mSP2Voltage;
	}

	public void setSP2Voltage(int mSP2Voltage, boolean lowRange) {

		this.mSP2Voltage = rawToVoltage(mSP2Voltage, lowRange);
	}

	public double getSP3Voltage() {
		return mSP3Voltage;
	}

	public void setSP3Voltage(int mSP3Voltage, boolean lowRange) {

		this.mSP3Voltage = rawToVoltage(mSP3Voltage, lowRange);
	}

	public double getSP0Current() {
		return mSP0Current;
	}

	public void setSP0Current(int mSP0Current, boolean lowRange) {

		this.mSP0Current = rawToCurrent(mSP0Current, lowRange);
	}

	public double getSP1Current() {
		return mSP1Current;
	}

	public void setSP1Current(int mSP1Current, boolean lowRange) {

		this.mSP1Current = rawToCurrent(mSP1Current, lowRange);
	}

	public double getSP2Current() {
		return mSP2Current;
	}

	public void setSP2Current(int mSP2Current, boolean lowRange) {

		this.mSP2Current = rawToCurrent(mSP2Current, lowRange);
	}

	public double getSP3Current() {
		return mSP3Current;
	}

	public void setSP3Current(int mSP3Current, boolean lowRange) {

		this.mSP3Current = rawToCurrent(mSP3Current, lowRange);
	}

	public double getPowerIn() {
		return mPowerIn;
	}

	public void setPowerIn(double mPowerIn) {
		this.mPowerIn = mPowerIn * 0.001;
	}

	public double getPowerOut() {
		return mPowerOut;
	}

	public void setPowerOut(double mPowerOut) {
		this.mPowerOut = mPowerOut * 0.001;
	}

	public double get3v3RailVoltage() {
		return m3v3RailVoltage;
	}

	public void set3v3RailVoltage(int m3v3RailVoltage, boolean lowRange) {

		this.m3v3RailVoltage = rawToVoltage(m3v3RailVoltage, lowRange);
	}

	public double get3v3RailCurrent() {
		return m3v3RailCurrent;
	}

	public void set3v3RailCurrent(int m3v3RailCurrent, boolean lowRange) {

		this.m3v3RailCurrent = rawToCurrent(m3v3RailCurrent, lowRange);
	}

	public int getEPSChannels() {
		return mEPSChannels;
	}

	public void setEPSChannels(int mEPSChannels) {
		this.mEPSChannels = mEPSChannels;
	}

	public int getRCSTemp() {
		return mRCSTemp;
	}

	public void setRCSTemp(int mRCSTemp) {
		this.mRCSTemp = mRCSTemp;
	}

	public errors getRCSStatus() {
		return mRCSStatus;
	}

	public void setRCSStatus(int mRCSStatus) {

		this.mRCSStatus = CISErrors.errors.values()[mRCSStatus];
	}

	public double getTXPower() {
		return mTXPower;
	}

	public void setTXPower(int mTXPower) {

		this.mTXPower = (mTXPower * 0.01);
		// System.out.println(this.mTXPower);
	}

	public double getRXPower() {
		return mRXPower;
	}

	public void setRXPower(int mRXPower) {

		this.mRXPower = mRXPower * 0.001;
		// System.out.println(this.mRXPower);
	}

	public double getRXSNR() {
		return mRXSNR;
	}

	public void setRXSNR(int mRXSNR) {

		this.mRXSNR = (mRXSNR * 0.1);
		// System.out.println(this.mRXSNR);
	}

	public int getPayloadFrames() {
		return mPayloadFrames;
	}

	public void setPayloadFrames(int mPayloadFrames) {
		this.mPayloadFrames = mPayloadFrames;
	}

	public errors getIHUStatus() {
		return mIHUStatus;
	}

	public void setIHUStatus(int index) {

		mIHUStatus = errors.values()[index];
	}

	public double getBattery0Heat() {
		return mBattery0Heat;
	}

	public void setBattery0Heat(double mBattery0Heat) {
		this.mBattery0Heat = mBattery0Heat * 50;
	}

	public double getBattery1Heat() {
		return mBattery1Heat;
	}

	public void setBattery1Heat(double mBattery1Heat) {
		this.mBattery1Heat = mBattery1Heat * 50;
	}

	public double get5vRailVoltage() {
		return m5vRailVoltage;
	}

	public void set5vRailVoltage(int m5vRailVoltage, boolean lowRange) {

		this.m5vRailVoltage = rawToVoltage(m5vRailVoltage, lowRange);
	}

	public double get5vRailCurrent() {
		return m5vRailCurrent;
	}

	public void set5vRailCurrent(int m5vRailCurrent, boolean lowRange) {

		this.m5vRailCurrent = rawToCurrent(m5vRailCurrent, lowRange);
	}
}
