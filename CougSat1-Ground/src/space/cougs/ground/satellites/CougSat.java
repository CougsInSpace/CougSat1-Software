package space.cougs.ground.satellites;

import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.TimeZone;

import space.cougs.ground.utils.CISErrors;

public abstract class CougSat {

	public abstract int getID();

	private ModeEnum mode = ModeEnum.NO_CONNECTION;
	private int ihuTemp = 0;
	private Calendar time = Calendar.getInstance();
	private long sdCard = 0;
	private int resetCount = 0;
	private CISErrors errorStatus = CISErrors.SUCCESS;
	private int adcsTemp = 0;
	private double lattitude = 0.0;
	private double longitude = 0.0;
	private double roll = 0.0;
	private double pitch = 0.0;
	private double yaw = 0.0;
	private int xPWMOut = 0;
	private int yPWMOut = 0;
	private int zPWMOut = 0;
	private double xCurrent = 0;
	private double yCurrent = 0;
	private double zCurrent = 0;
	private int ifjrTemp = 0;
	private int pmicTemp = 0;
	private int batteryATemp = 0;
	private int batteryBTemp = 0;
	private int reg3V3ATemp = 0;
	private int reg3V3BTemp = 0;
	private int pvTemp[] = new int[8];
	private int mpptTemp[] = new int[8];
	private double pvVoltage[] = new double[8];
	private double pvCurrent[] = new double[8];
	private double batteryAVoltage = 0.0;
	private double batteryACurrent = 0.0;
	private double batteryBVoltage = 0.0;
	private double batteryBCurrent = 0.0;
	private double reg3V3AVoltage = 0.0;
	private double reg3V3ACurrent = 0.0;
	private double reg3V3BVoltage = 0.0;
	private double reg3V3BCurrent = 0.0;
	private double pr3V3Current[] = new double[13];
	private double prBattCurrent[] = new double[7];
	private double pv3V3Current[] = new double[4];
	private double prBatteryHeaterACurrent = 0.0;
	private double prBatteryHeaterBCurrent = 0.0;
	private double prDeployablesCurrent = 0.0;
	private boolean pvSwitchingState[] = new boolean[16];
	private boolean outputSwitchingState[] = new boolean[54];
	private int energyLevel = 0;
	private int commsTemp = 0;
	private int rxTemp = 0;
	private int txTemp = 0;
	private int ampTemp = 0;
	private double rxPower = 0.0;
	private double rxSNR = 0.0;
	private int badPacketCount = 0;
	private int rxCenterFrequency = 0;
	private int txCenterFrequency = 0;
	private double txPower = 0.0;
	private double txAmplifierVoltage = 0.0;

	enum ModeEnum {
		BEACON, CHARGING, DATA_TRANSMISION, SCIENCE, ECLIPSE, SAFE, NO_CONNECTION;
	}

	public String getMode() {

		return mode.name();
	}

	public void setMode(int buff) {

		switch (buff) {
		case 0x31:
			mode = ModeEnum.BEACON;
			break;
		case 0x32:
			mode = ModeEnum.CHARGING;
			break;
		case 0x33:
			mode = ModeEnum.DATA_TRANSMISION;
			break;
		case 0x34:
			mode = ModeEnum.SCIENCE;
			break;
		case 0x35:
			mode = ModeEnum.ECLIPSE;
			break;
		case 0x40:
			mode = ModeEnum.SAFE;
			break;
		}

	}

	public String getTime() {

		SimpleDateFormat format = new SimpleDateFormat("HH:mm:ss");

		format.setTimeZone(TimeZone.getTimeZone("UTC"));

		return format.format(time.getTime());
	}

	public void setTime(Calendar time) {
		this.time = time;
	}

	public int getIHUTemp() {
		return ihuTemp;
	}

	public void setIHUTemp(int ihuTemp) {
		this.ihuTemp = ihuTemp;
	}

	public long getSDCard() {
		return sdCard;
	}

	public void setSDCard(long sdCard) {
		this.sdCard = sdCard;
	}

	public int getResetCount() {
		return resetCount;
	}

	public void setResetCount(int resetCount) {
		this.resetCount = resetCount;
	}

	public CISErrors getErrorStatus() {
		return errorStatus;
	}

	public void setErrorStatus(int i) {
		this.errorStatus = CISErrors.values()[i];
	}

	public int getADCSTemp() {
		return adcsTemp;
	}

	public void setADCSTemp(int adcsTemp) {
		this.adcsTemp = adcsTemp;
	}

	public double getLattitude() {
		return lattitude;
	}

	public void setLattitude(double lattitude) {
		this.lattitude = lattitude;
	}

	public double getLongitude() {
		return longitude;
	}

	public void setLongitude(double longitude) {
		this.longitude = longitude;
	}

	public double getRoll() {
		return roll;
	}

	public void setRoll(double roll) {
		this.roll = roll;
	}

	public double getPitch() {
		return pitch;
	}

	public void setPitch(double pitch) {
		this.pitch = pitch;
	}

	public double getYaw() {
		return yaw;
	}

	public void setYaw(double yaw) {
		this.yaw = yaw;
	}

	public int getXPWMOut() {
		return xPWMOut;
	}

	public void setXPWMOut(int xPWMOut) {
		this.xPWMOut = xPWMOut;
	}

	public int getYPWMOut() {
		return yPWMOut;
	}

	public void setYPWMOut(int yPWMOut) {
		this.yPWMOut = yPWMOut;
	}

	public int getZPWMOut() {
		return zPWMOut;
	}

	public void setZPWMOut(int zPWMOut) {
		this.zPWMOut = zPWMOut;
	}

	public double getXCurrent() {
		return xCurrent;
	}

	public void setXCurrent(double xCurrent) {
		this.xCurrent = xCurrent;
	}

	public double getYCurrent() {
		return yCurrent;
	}

	public void setYCurrent(double yCurrent) {
		this.yCurrent = yCurrent;
	}

	public double getZCurrent() {
		return zCurrent;
	}

	public void setZCurrent(double zCurrent) {
		this.zCurrent = zCurrent;
	}

	public int getIFJRTemp() {
		return ifjrTemp;
	}

	public void setIFJRTemp(int ifjrTemp) {
		this.ifjrTemp = ifjrTemp;
	}

	public int getPMICTemp() {
		return pmicTemp;
	}

	public void setPMICTemp(int pmicTemp) {
		this.pmicTemp = pmicTemp;
	}

	public int getBatteryATemp() {
		return batteryATemp;
	}

	public void setBatteryATemp(int batteryATemp) {
		this.batteryATemp = batteryATemp;
	}

	public int getBatteryBTemp() {
		return batteryBTemp;
	}

	public void setBatteryBTemp(int batteryBTemp) {
		this.batteryBTemp = batteryBTemp;
	}

	public int getReg3V3ATemp() {
		return reg3V3ATemp;
	}

	public void setReg3V3ATemp(int reg3v3aTemp) {
		reg3V3ATemp = reg3v3aTemp;
	}

	public int getReg3V3BTemp() {
		return reg3V3BTemp;
	}

	public void setReg3V3BTemp(int reg3v3bTemp) {
		reg3V3BTemp = reg3v3bTemp;
	}

	public int getPVTemp(int i) {
		return pvTemp[i];
	}

	public void setPVTemp(int i, int pvTemp) {
		this.pvTemp[i] = pvTemp;
	}

	public int getMPPTTemp(int i) {
		return mpptTemp[i];
	}

	public void setMPPTTemp(int i, int mpptTemp) {
		this.mpptTemp[i] = mpptTemp;
	}

	public double getPVVoltage(int i) {
		return pvVoltage[i];
	}

	public void setPVVoltage(int i, double pvVoltage) {
		this.pvVoltage[i] = pvVoltage;
	}

	public double getPVCurrent(int i) {
		return pvCurrent[i];
	}

	public void setPVCurrent(int i, double pvCurrent) {
		this.pvCurrent[i] = pvCurrent;
	}

	public double getBatteryAVoltage() {
		return batteryAVoltage;
	}

	public void setBatteryAVoltage(double batteryAVoltage) {
		this.batteryAVoltage = batteryAVoltage;
	}

	public double getBatteryACurrent() {
		return batteryACurrent;
	}

	public void setBatteryACurrent(double batteryACurrent) {
		this.batteryACurrent = batteryACurrent;
	}

	public double getBatteryBVoltage() {
		return batteryBVoltage;
	}

	public void setBatteryBVoltage(double batteryBVoltage) {
		this.batteryBVoltage = batteryBVoltage;
	}

	public double getBatteryBCurrent() {
		return batteryBCurrent;
	}

	public void setBatteryBCurrent(double batteryBCurrent) {
		this.batteryBCurrent = batteryBCurrent;
	}

	public double getReg3V3AVoltage() {
		return reg3V3AVoltage;
	}

	public void setReg3V3AVoltage(double reg3v3aVoltage) {
		reg3V3AVoltage = reg3v3aVoltage;
	}

	public double getReg3V3ACurrent() {
		return reg3V3ACurrent;
	}

	public void setReg3V3ACurrent(double reg3v3aCurrent) {
		reg3V3ACurrent = reg3v3aCurrent;
	}

	public double getReg3V3BVoltage() {
		return reg3V3BVoltage;
	}

	public void setReg3V3BVoltage(double reg3v3bVoltage) {
		reg3V3BVoltage = reg3v3bVoltage;
	}

	public double getReg3V3BCurrent() {
		return reg3V3BCurrent;
	}

	public void setReg3V3BCurrent(double reg3v3bCurrent) {
		reg3V3BCurrent = reg3v3bCurrent;
	}

	public double getPR3V3Current(int i) {
		return pr3V3Current[i];
	}

	public void setPR3V3Current(int i, double pr3v3Current) {
		pr3V3Current[i] = pr3v3Current;
	}

	public double getPRBattCurrent(int i) {
		return prBattCurrent[i];
	}

	public void setPRBattCurrent(int i, double prBattCurrent) {
		this.prBattCurrent[i] = prBattCurrent;
	}

	public double getPV3V3Current(int i) {
		return pv3V3Current[i];
	}

	public void setPV3V3Current(int i, double pv3v3Current) {
		pv3V3Current[i] = pv3v3Current;
	}

	public double getPRBatteryHeaterACurrent() {
		return prBatteryHeaterACurrent;
	}

	public void setPRBatteryHeaterACurrent(double prBatteryHeaterACurrent) {
		this.prBatteryHeaterACurrent = prBatteryHeaterACurrent;
	}

	public double getPRBatteryHeaterBCurrent() {
		return prBatteryHeaterBCurrent;
	}

	public void setPRBatteryHeaterBCurrent(double prBatteryHeaterBCurrent) {
		this.prBatteryHeaterBCurrent = prBatteryHeaterBCurrent;
	}

	public double getPRDeployablesCurrent() {
		return prDeployablesCurrent;
	}

	public void setPRDeployablesCurrent(double prDeployablesCurrent) {
		this.prDeployablesCurrent = prDeployablesCurrent;
	}

	public boolean getPVSwitchingState(int i) {
		return pvSwitchingState[i];
	}

	public void setPVSwitchingState(long pvSwitchingState) {
		for (int i = 0; i < 16; i++) {
			this.pvSwitchingState[i] = ((pvSwitchingState >> i) & 0x1) == 0x1;
		}
	}

	public boolean getOutputSwitchingState(int i) {
		return outputSwitchingState[i];
	}

	public void setOutputSwitchingState(long outputSwitchingState) {

		for (int i = 0; i < 54; i++) {
			this.outputSwitchingState[i] = ((outputSwitchingState >> i) & 0x1) == 0x1;
		}
	}

	public int getEnergyLevel() {
		return energyLevel;
	}

	public void setEnergyLevel(int energyLevel) {
		this.energyLevel = energyLevel;
	}

	public int getCommsTemp() {
		return commsTemp;
	}

	public void setCommsTemp(int commsTemp) {
		this.commsTemp = commsTemp;
	}

	public int getRXTemp() {
		return rxTemp;
	}

	public void setRXTemp(int rxTemp) {
		this.rxTemp = rxTemp;
	}

	public int getTXTemp() {
		return txTemp;
	}

	public void setTXTemp(int txTemp) {
		this.txTemp = txTemp;
	}

	public int getAMPTemp() {
		return ampTemp;
	}

	public void setAMPTemp(int ampTemp) {
		this.ampTemp = ampTemp;
	}

	public double getRXPower() {
		return rxPower;
	}

	public void setRXPower(double rxPower) {
		this.rxPower = rxPower;
	}

	public double getRXSNR() {
		return rxSNR;
	}

	public void setRXSNR(double rxSNR) {
		this.rxSNR = rxSNR;
	}

	public int getBadPacketCount() {
		return badPacketCount;
	}

	public void setBadPacketCount(int badPacketCount) {
		this.badPacketCount = badPacketCount;
	}

	public int getRXCenterFrequency() {
		return rxCenterFrequency;
	}

	public void setRXCenterFrequency(int rxCenterFrequency) {
		this.rxCenterFrequency = rxCenterFrequency;
	}

	public int getTXCenterFrequency() {
		return txCenterFrequency;
	}

	public void setTXCenterFrequency(int txCenterFrequency) {
		this.txCenterFrequency = txCenterFrequency;
	}

	public double getTxPower() {
		return txPower;
	}

	public void setTxPower(double txPower) {
		this.txPower = txPower;
	}

	public double getTxAmplifierVoltage() {
		return txAmplifierVoltage;
	}

	public void setTxAmplifierVoltage(double txAmplifierVoltage) {
		this.txAmplifierVoltage = txAmplifierVoltage;
	}

}
