package space.cougs.ground.gui.subsystems.modules;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JPanel;

public class PhotoViewer extends JPanel {

	private static final long serialVersionUID = 1L;
	private BufferedImage thumbnail;

	public PhotoViewer() {

	}

	public void setThumbnail(File newThumbnail) {

//		thumbnail = (BufferedImage) newThumbnail;
		try {
			thumbnail = ImageIO.read(newThumbnail);

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		this.repaint();
	}

	public void paintComponent(Graphics g) {
		if (thumbnail != null) {
			super.paintComponent(g);
			Graphics2D g2d = (Graphics2D) g;
			double aspectRatio = (double) thumbnail.getHeight() / thumbnail.getWidth();

			int width = this.getWidth();
			int height = (int) (width * aspectRatio);
			int extraHeight = (this.getHeight() - height) / 2;

			g2d.drawImage(thumbnail, 0, extraHeight, width, height, null);
		}

	}
}
