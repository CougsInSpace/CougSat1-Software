package space.cougs.ground.utils;

import java.io.FileInputStream;
import java.io.IOException;

public class FileUtils {

	public static final long readNextBytes(FileInputStream file, int numberOfBytes) throws IOException {
		long buff = 0;

		for (int i = 0; i < numberOfBytes; i++) {
			buff = buff << 8 | file.read();
		}

		return buff;

	}

}
