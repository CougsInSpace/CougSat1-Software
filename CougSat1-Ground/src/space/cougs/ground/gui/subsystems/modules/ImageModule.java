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

		Rectangle rect = this.getBounds();

		double aspectRatio = (double) image.getWidth() / image.getHeight();

		int height = rect.height;
		int width = Math.min(rect.width, (int) (rect.height * aspectRatio));
		height = (int) (width / aspectRatio);

		int x = (rect.width - width) / 2;
		int y = (rect.height - height) / 2;
		g2d.drawImage(image, x, y, width, height, null);

	}

}
