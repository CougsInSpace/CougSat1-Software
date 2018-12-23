package space.cougs.ground.utils;

import java.io.FileInputStream;
import java.io.IOException;

public final class FileUtils {
  public static long readNextBytes(FileInputStream file, int numberOfBytes)
      throws IOException {
    long result = 0;

    while (numberOfBytes > 0) {
      int buf = file.read();
      if (buf == -1) {
        throw new IOException("Reached end of file");
      }
      result = result << 8 | buf;
      numberOfBytes--;
    }

    return result;
  }

  public static double readNextGeographicCoordinate(FileInputStream file)
      throws IOException {
    return Units.rawToGeographicCoordinate(readNextBytes(file, 4));
  }

  public static double readNextEulerAngle(FileInputStream file)
      throws IOException {
    return Units.rawToEulerAngle(readNextBytes(file, 2));
  }

  public static double readNextCurrent(FileInputStream file)
      throws IOException {
    return Units.rawToCurrent(readNextBytes(file, 2));
  }

  public static double readNextPower(FileInputStream file) throws IOException {
    return Units.rawToPower(readNextBytes(file, 2));
  }

  public static double readNextDecibels(FileInputStream file)
      throws IOException {
    return Units.rawToDecibels(readNextBytes(file, 2));
  }

  public static int readNextFrequency(FileInputStream file) throws IOException {
    return Units.rawToFrequency(readNextBytes(file, 3));
  }

  public static double readNextVoltage(FileInputStream file)
      throws IOException {
    return Units.rawToVoltage(readNextBytes(file, 2));
  }

  public static int readNextTemperature(FileInputStream file)
      throws IOException {
    return Units.rawToTemperature(readNextBytes(file, 1));
  }
}
