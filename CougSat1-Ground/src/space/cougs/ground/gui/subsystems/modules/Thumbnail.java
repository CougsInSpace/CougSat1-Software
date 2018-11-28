package space.cougs.ground.gui.subsystems.modules;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JPanel;

public class Thumbnail extends JPanel {

	private static final long serialVersionUID = 1L;

	private int x = 0;
	private int y = 0;
	private int size = 32;
	
	BufferedImage thumbNail;
	private static int width;
	private static int height;

	public Thumbnail(File thumbnail, int newHeight, int newWidth) {
		super();

		width = newWidth;
		height = newHeight;
		
		setImage(thumbnail);
	}

	public void setImage(File set_image) {
		try {
			thumbNail = ImageIO.read(set_image);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	@Override
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2d = (Graphics2D) g;

//		g2d.drawImage(thumbNail, 0, 0, Thumbnail.width, Thumbnail.height, null);
		g2d.drawImage(thumbNail, 0, 0, 400, 300, 0, 0, thumbNail.getWidth(), thumbNail.getHeight(), null);
	}

}
