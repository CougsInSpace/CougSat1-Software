package space.cougs.ground.gui.subsystems.modules;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.image.BufferedImage;

import javax.swing.JPanel;

public class ImageModule extends JPanel {

	private static final long serialVersionUID = 1L;
	private BufferedImage image;

	public ImageModule(BufferedImage image) {

		this.image = image;
		this.repaint();
	}

	@Override
	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2d = (Graphics2D) (g);

		g2d.setRenderingHint(RenderingHints.KEY_ALPHA_INTERPOLATION, RenderingHints.VALUE_ALPHA_INTERPOLATION_QUALITY);
		g2d.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

		Rectangle rec = this.getBounds();

		int y = 0;
		int x = rec.width / 2 - rec.height / 2;
		
		int temp = Math.min(rec.height, rec.width);

		g2d.drawImage(image, x, y, temp, temp, null);

	}

}
