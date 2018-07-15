package space.cougs.ground.utils;

import java.util.Calendar;
import java.util.TimeZone;

public final class Units {

	public static double rawToVoltage(long raw) {

		return raw * 0.0001;
	}

	public static double rawToCurrent(long raw) {

		return (short) raw * 0.00015;
	}

	public static double rawToPower(long raw) {

		return raw * 0.000250;
	}

	public static int rawToEnergy(long raw) {

		return (int) (raw * 500);
	}

	public static double rawToGeographicCoordinate(long raw) {

		return (int) raw * 0.00010 / 60.0;
	}

	public static Calendar rawToTime(long raw) {

		Calendar buff = Calendar.getInstance(TimeZone.getTimeZone("UTC"));
		buff.setTimeInMillis(raw * 1000);

		return buff;
	}

	public static int rawToTemp(long raw) {

		return (byte) raw;
	}

	public static double rawToAngle(long raw) {

		return (raw * 360.0 / 0xFFFFFFFF);
	}

	public static double rawToDecibels(long raw) {

		return (short) raw * 0.001;
	}

	public static int rawToFrequency(long raw) {

		return (int) raw * 100;
	}

	public static double rawToDataRate(long raw) {

		return Math.pow(raw, 2);
	}

}
