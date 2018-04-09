package space.cougs.ground;

public class CougSat1Telemetry {

	private String mMode = "";
	private String mTime = "";
	private int mResetCount = 0;
	private int mIHUTemp = 0;
	private long mIHUSdCard = 0;
	private int mADCSTemp = 0;
	private String mADCSStatus = "";
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
	private double mSP0Volatage = 0.0;
	private double mSP1Volatage = 0.0;
	private double mSP2Volatage = 0.0;
	private double mSP3Volatage = 0.0;
	private double mSP0Current = 0.0;
	private double mSP1Current = 0.0;	
	private double mSP2Current = 0.0;
	private double mSP3Current = 0.0;
	private double mPowerIn = 0.0;
	private double mPowerOut = 0.0;
	private double m3v3RailVoltage = 0.0;
	private double m3v3RailCurrent = 0.0;
	private short mEPSChannels = 0;
	private int mRCSTemp = 0;
	private String mRCSStatus = "";
	private double mTXPower = 0.0;
	private double mRXPower = 0.0;
	private double mRXSNR = 0.0;
	private int mPayloadFrames = 0;
	
	public CougSat1Telemetry() {
		
		
		
		
	}


	public String getMode() {
		return mMode;
	}


	public void setMode(String mMode) {
		this.mMode = mMode;
	}


	public String getTime() {
		return mTime;
	}


	public void setTime(String mTime) {
		this.mTime = mTime;
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


	public String getADCSStatus() {
		return mADCSStatus;
	}


	public void setADCSStatus(String mADCSStatus) {
		this.mADCSStatus = mADCSStatus;
	}


	public double getLattitude() {
		return mLattitude;
	}


	public void setLattitude(double mLattitude) {
		this.mLattitude = mLattitude;
	}


	public double getLongitude() {
		return mLongitude;
	}


	public void setLongitude(double mLongitude) {
		this.mLongitude = mLongitude;
	}


	public double getRoll() {
		return mRoll;
	}


	public void setRoll(double mRoll) {
		this.mRoll = mRoll;
	}


	public double getPitch() {
		return mPitch;
	}


	public void setPitch(double mPitch) {
		this.mPitch = mPitch;
	}


	public double getYaw() {
		return mYaw;
	}


	public void setYaw(double mYaw) {
		this.mYaw = mYaw;
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


	public void setBattery0Voltage(double mBattery0Voltage) {
		this.mBattery0Voltage = mBattery0Voltage;
	}


	public double getBattery0Current() {
		return mBattery0Current;
	}


	public void setBattery0Current(double mBattery0Current) {
		this.mBattery0Current = mBattery0Current;
	}


	public double getBattery1Voltage() {
		return mBattery1Voltage;
	}


	public void setBattery1Voltage(double mBattery1Voltage) {
		this.mBattery1Voltage = mBattery1Voltage;
	}


	public double getBattery1Current() {
		return mBattery1Current;
	}


	public void setBattery1Current(double mBattery1Current) {
		this.mBattery1Current = mBattery1Current;
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


	public double getSP0Volatage() {
		return mSP0Volatage;
	}


	public void setSP0Volatage(double mSP0Volatage) {
		this.mSP0Volatage = mSP0Volatage;
	}


	public double getSP1Volatage() {
		return mSP1Volatage;
	}


	public void setSP1Volatage(double mSP1Volatage) {
		this.mSP1Volatage = mSP1Volatage;
	}


	public double getSP2Volatage() {
		return mSP2Volatage;
	}


	public void setSP2Volatage(double mSP2Volatage) {
		this.mSP2Volatage = mSP2Volatage;
	}


	public double getSP3Volatage() {
		return mSP3Volatage;
	}


	public void setSP3Volatage(double mSP3Volatage) {
		this.mSP3Volatage = mSP3Volatage;
	}


	public double getSP0Current() {
		return mSP0Current;
	}


	public void setSP0Current(double mSP0Current) {
		this.mSP0Current = mSP0Current;
	}


	public double getSP1Current() {
		return mSP1Current;
	}


	public void setSP1Current(double mSP1Current) {
		this.mSP1Current = mSP1Current;
	}


	public double getSP2Current() {
		return mSP2Current;
	}


	public void setSP2Current(double mSP2Current) {
		this.mSP2Current = mSP2Current;
	}


	public double getSP3Current() {
		return mSP3Current;
	}


	public void setSP3Current(double mSP3Current) {
		this.mSP3Current = mSP3Current;
	}


	public double getPowerIn() {
		return mPowerIn;
	}


	public void setPowerIn(double mPowerIn) {
		this.mPowerIn = mPowerIn;
	}


	public double getPowerOut() {
		return mPowerOut;
	}


	public void setPowerOut(double mPowerOut) {
		this.mPowerOut = mPowerOut;
	}


	public double get3v3RailVoltage() {
		return m3v3RailVoltage;
	}


	public void set3v3RailVoltage(double m3v3RailVoltage) {
		this.m3v3RailVoltage = m3v3RailVoltage;
	}


	public double get3v3RailCurrent() {
		return m3v3RailCurrent;
	}


	public void set3v3RailCurrent(double m3v3RailCurrent) {
		this.m3v3RailCurrent = m3v3RailCurrent;
	}


	public short getEPSChannels() {
		return mEPSChannels;
	}


	public void setEPSChannels(short mEPSChannels) {
		this.mEPSChannels = mEPSChannels;
	}


	public int getRCSTemp() {
		return mRCSTemp;
	}


	public void setRCSTemp(int mRCSTemp) {
		this.mRCSTemp = mRCSTemp;
	}


	public String getRCSStatus() {
		return mRCSStatus;
	}


	public void setRCSStatus(String mRCSStatus) {
		this.mRCSStatus = mRCSStatus;
	}


	public double getTXPower() {
		return mTXPower;
	}


	public void setTXPower(double mTXPower) {
		this.mTXPower = mTXPower;
	}


	public double getRXPower() {
		return mRXPower;
	}


	public void setRXPower(double mRXPower) {
		this.mRXPower = mRXPower;
	}


	public double getRXSNR() {
		return mRXSNR;
	}


	public void setRXSNR(double mRXSNR) {
		this.mRXSNR = mRXSNR;
	}


	public int getPayloadFrames() {
		return mPayloadFrames;
	}


	public void setPayloadFrames(int mPayloadFrames) {
		this.mPayloadFrames = mPayloadFrames;
	}
}
