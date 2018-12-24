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
    Calendar buf = Calendar.getInstance(TimeZone.getTimeZone("UTC"));
    buf.setTimeInMillis(raw * 1000);
    return buf;
  }

  public static int rawToTemperature(long raw) {
    return (byte)raw;
  }

  public static double rawToEulerAngle(long raw) {
    return raw * 360.0 / (1 << 16); // 2^16 = 2π
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

  public static String toBytes(long bytes) {
    if (bytes > (1L << 30L)) {
      return String.format("%6.2fGiB", bytes / (double)(1L << 30L));
    } else if (bytes > (1L << 20L)) {
      return String.format("%6.2fMiB", bytes / (double)(1L << 20L));
    } else if (bytes > (1L << 10L)) {
      return String.format("%6.2fkiB", bytes / (double)(1L << 10L));
    } else {
      return String.format("%6.2f B", bytes / 1.0);
    }
  }
}
