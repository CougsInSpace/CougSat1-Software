package space.cougs.ground.utils;

import java.util.Calendar;
import java.util.TimeZone;

public final class Units {
  public static double rawToVoltage(long raw) {
    return raw * 100.0e-6; // 100 µV/LSB
  }

  public static double rawToCurrent(long raw) {
    return raw * 150.0e-6; // 100 µA/LSB;
  }

  public static double rawToPower(long raw) {
    return raw * 250.0e-6; // 250 µW/LSB;
  }

  public static int rawToEnergy(long raw) {
    return (int)(raw * 500); // 500J/LSB
  }

  public static double rawToGeographicCoordinate(long raw) {
    return (int)raw * 10.0e-6 / 60.0; // 10 µmin/LSB
  }

  public static Calendar rawToTime(long raw) {
    Calendar buff = Calendar.getInstance(TimeZone.getTimeZone("UTC"));
    buff.setTimeInMillis(raw * 1000);

    return buff;
  }

  public static int rawToTemp(long raw) {
    return (byte)raw;
  }

  public static double rawToAngle(long raw) {
    return raw * 360.0 / (1 << 16); // 2^16 = 2Pi
  }

  public static double rawToDecibels(long raw) {
    return (short)raw * 0.001; // 1 mdB/LSB
  }

  public static int rawToFrequency(long raw) {
    return (int)raw * 100; // 100 Hz/LSB
  }

  public static double rawToDataRate(long raw) {
    return Math.pow(raw, 2); // 1 sqrt(bps)/LSB
  }
}
