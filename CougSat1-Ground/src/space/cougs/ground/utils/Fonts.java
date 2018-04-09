package space.cougs.ground.utils;

import java.awt.Font;
import java.awt.FontFormatException;
import java.awt.GraphicsEnvironment;
import java.io.File;
import java.io.IOException;
import java.util.Arrays;

public class Fonts {

	private static Fonts instance;

	public Font[] ethnocentric = new Font[6];
	public Font[] monofonto = new Font[6];
	public Font[] codeNewRoman = new Font[6];

	public final byte SMALL = 0;
	public final byte MEDIUM = 1;
	public final byte LARGE = 2;
	public final byte X_LARGE = 3;
	public final byte XX_LARGE = 4;
	public final byte XXX_LARGE = 5;

	public static Fonts getInstance() {
		if (instance == null) {
			instance = new Fonts();
		}
		return instance;
	}

	/**
	 * This registers custom fonts
	 */
	private Fonts() {
		try {
			GraphicsEnvironment ge = GraphicsEnvironment.getLocalGraphicsEnvironment();
			ge.registerFont(Font.createFont(Font.TRUETYPE_FONT, new File("resources/fonts/Ethnocentric.ttf")));
			ge.registerFont(Font.createFont(Font.TRUETYPE_FONT, new File("resources/fonts/Monofonto.ttf")));
			ge.registerFont(Font.createFont(Font.TRUETYPE_FONT, new File("resources/fonts/CodeNewRoman.ttf")));
			//System.out.println(Arrays.toString(ge.getAvailableFontFamilyNames()));
		} catch (IOException | FontFormatException e) {
			e.printStackTrace();
		}
		
		monofonto[SMALL] = new Font("Monofonto", Font.PLAIN, 16);
		monofonto[MEDIUM] = new Font("Monofonto", Font.PLAIN, 32);
		monofonto[LARGE] = new Font("Monofonto", Font.PLAIN, 48);
		monofonto[X_LARGE] = new Font("Monofonto", Font.PLAIN, 64);
		monofonto[XX_LARGE] = new Font("Monofonto", Font.PLAIN, 96);
		monofonto[XXX_LARGE] = new Font("Monofonto", Font.PLAIN, 128);

		ethnocentric[SMALL] = new Font("Ethnocentric Rg", Font.PLAIN, 16);
		ethnocentric[MEDIUM] = new Font("Ethnocentric Rg", Font.PLAIN, 32);
		ethnocentric[LARGE] = new Font("Ethnocentric Rg", Font.PLAIN, 48);
		ethnocentric[X_LARGE] = new Font("Ethnocentric Rg", Font.PLAIN, 64);
		ethnocentric[XX_LARGE] = new Font("Ethnocentric Rg", Font.PLAIN, 96);
		ethnocentric[XXX_LARGE] = new Font("Ethnocentric Rg", Font.PLAIN, 128);

		codeNewRoman[SMALL] = new Font("Code New Roman", Font.PLAIN, 16);
		codeNewRoman[MEDIUM] = new Font("Code New Roman", Font.PLAIN, 32);
		codeNewRoman[LARGE] = new Font("Code New Roman", Font.PLAIN, 48);
		codeNewRoman[X_LARGE] = new Font("Code New Roman", Font.PLAIN, 64);
		codeNewRoman[XX_LARGE] = new Font("Code New Roman", Font.PLAIN, 96);
		codeNewRoman[XXX_LARGE] = new Font("Code New Roman", Font.PLAIN, 128);
	}
}