package space.cougs.ground.utils;

import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileFilter;
import java.io.FileInputStream;
import java.io.IOException;

import javax.imageio.ImageIO;

public final class FileUtils {
  public static FileFilter imageFilter = new FileFilter() {
    @Override
    public boolean accept(File pathname) {
      String name = pathname.getName().toLowerCase();
      return name.endsWith(".png") || name.endsWith(".jpg") ||
          name.endsWith(".jpeg") || name.endsWith(".gif") ||
          name.endsWith(".bmp");
    }
  };

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

  public static int readNextEnergy(FileInputStream file)
      throws IOException {
    return Units.rawToEnergy(readNextBytes(file, 1));
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

  public static BufferedImage getImage(int size, String name) {
    return getImage(String.valueOf(size) + "\\" + name);
  }

  public static BufferedImage getImage(String name) {
    return getImage(new File("resources\\images\\" + name));
  }

  public static BufferedImage getImage(File file) {
    if (!file.exists()) {
      System.out.printf("Image cannot be found: %s\n", file.getAbsolutePath());
      return null;
    }
    BufferedImage image = null;
    try {
      image = ImageIO.read(file);
    } catch (IOException e) {
      System.out.printf(
          "Failed to get image from: %s\n", file.getAbsolutePath());
      e.printStackTrace();
    }
    return image;
  }

  public static BufferedImage getThumbnail(File file) {
    if (!file.exists()) {
      System.out.printf("Image cannot be found: %s\n", file.getAbsolutePath());
      return null;
    }
    String thumbnailPath = file.getParentFile().getAbsolutePath();
    thumbnailPath += "\\thumbnails\\" + file.getName();
    File thumbnailFile = new File(thumbnailPath);
    if (thumbnailFile.exists()) {
      return getImage(thumbnailFile);
    } else {

      // Get image resize to 150sq px.
      BufferedImage image = getImage(file);
      if (image == null) {
        return null;
      } else {
        BufferedImage thumbnail =
            new BufferedImage(150, 150, BufferedImage.TYPE_INT_RGB);

        Graphics2D g2d = (Graphics2D)thumbnail.getGraphics();

        g2d.drawImage(image, 0, 0, 150, 150, 0, 0, image.getWidth(),
            image.getHeight(), null);

        try {
          thumbnailFile.mkdirs();
          // thumbnailFile.createNewFile();
          ImageIO.write(thumbnail, "png", thumbnailFile);
        } catch (IOException e) {
          System.out.printf("Failed to save thumbnail image at: %s\n",
              thumbnailFile.getAbsolutePath());
          e.printStackTrace();
        }
        return thumbnail;
      }
    }
  }
}
