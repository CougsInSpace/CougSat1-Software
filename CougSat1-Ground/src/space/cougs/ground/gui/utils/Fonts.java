package space.cougs.ground.gui.utils;

import java.awt.Font;
import java.awt.FontFormatException;
import java.awt.GraphicsEnvironment;
import java.io.File;
import java.io.IOException;

public class Fonts {
  public static final Font BODY_8  = new Font("Monofonto", Font.PLAIN, 8);
  public static final Font BODY_12 = new Font("Monofonto", Font.PLAIN, 12);
  public static final Font BODY_16 = new Font("Monofonto", Font.PLAIN, 16);
  public static final Font BODY_18 = new Font("Monofonto", Font.PLAIN, 18);
  public static final Font BODY_24 = new Font("Monofonto", Font.PLAIN, 24);
  public static final Font BODY_32 = new Font("Monofonto", Font.PLAIN, 32);
  public static final Font BODY_36 = new Font("Monofonto", Font.PLAIN, 36);
  public static final Font BODY_48 = new Font("Monofonto", Font.PLAIN, 48);

  public static final Font TITLE_8 = new Font("Ethnocentric Rg", Font.PLAIN, 8);
  public static final Font TITLE_12 =
      new Font("Ethnocentric Rg", Font.PLAIN, 12);
  public static final Font TITLE_16 =
      new Font("Ethnocentric Rg", Font.PLAIN, 16);
  public static final Font TITLE_18 =
      new Font("Ethnocentric Rg", Font.PLAIN, 18);
  public static final Font TITLE_24 =
      new Font("Ethnocentric Rg", Font.PLAIN, 24);
  public static final Font TITLE_32 =
      new Font("Ethnocentric Rg", Font.PLAIN, 32);
  public static final Font TITLE_36 =
      new Font("Ethnocentric Rg", Font.PLAIN, 36);
  public static final Font TITLE_48 =
      new Font("Ethnocentric Rg", Font.PLAIN, 48);

  public static final Font CONSOLE_8 =
      new Font("Code New Roman", Font.PLAIN, 8);
  public static final Font CONSOLE_12 =
      new Font("Code New Roman", Font.PLAIN, 12);
  public static final Font CONSOLE_16 =
      new Font("Code New Roman", Font.PLAIN, 16);
  public static final Font CONSOLE_18 =
      new Font("Code New Roman", Font.PLAIN, 18);
  public static final Font CONSOLE_24 =
      new Font("Code New Roman", Font.PLAIN, 24);
  public static final Font CONSOLE_32 =
      new Font("Code New Roman", Font.PLAIN, 32);
  public static final Font CONSOLE_36 =
      new Font("Code New Roman", Font.PLAIN, 36);
  public static final Font CONSOLE_48 =
      new Font("Code New Roman", Font.PLAIN, 48);

  public static void loadFonts() {
    try {
      GraphicsEnvironment ge =
          GraphicsEnvironment.getLocalGraphicsEnvironment();
      ge.registerFont(Font.createFont(
          Font.TRUETYPE_FONT, new File("resources/fonts/Ethnocentric.ttf")));
      ge.registerFont(Font.createFont(
          Font.TRUETYPE_FONT, new File("resources/fonts/Monofonto.ttf")));
      ge.registerFont(Font.createFont(
          Font.TRUETYPE_FONT, new File("resources/fonts/CodeNewRoman.ttf")));
      // Prints out all the different fonts for debugging purposes
      // System.out.println(Arrays.toString(ge.getAvailableFontFamilyNames()));
    } catch (IOException | FontFormatException e) {
      e.printStackTrace();
    }
  }
}