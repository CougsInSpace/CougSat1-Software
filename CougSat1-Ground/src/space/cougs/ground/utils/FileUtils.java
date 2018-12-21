package space.cougs.ground.utils;

import java.io.FileInputStream;
import java.io.IOException;

public class FileUtils {
  public static final long readNextBytes(
      FileInputStream file, int numberOfBytes) throws IOException {
    long buff = 0;

    while (numberOfBytes > 0) {
      buff = buff << 8 | file.read();
      numberOfBytes--;
    }

    return buff;
  }
}
