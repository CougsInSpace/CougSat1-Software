package space.cougs.ground.gui.modules;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.image.BufferedImage;

import javax.swing.JComponent;

public class ImageModule extends JComponent {
  private static final long serialVersionUID = 1L;

  private BufferedImage image;

  public ImageModule(BufferedImage image) {
    this.image = image;
  }

  @Override
  protected void paintComponent(Graphics g) {
    super.paintComponent(g);
    Graphics2D g2d = (Graphics2D)(g);

    g2d.setRenderingHint(RenderingHints.KEY_ALPHA_INTERPOLATION,
        RenderingHints.VALUE_ALPHA_INTERPOLATION_QUALITY);

    double aspectRatio = (double)image.getWidth() / image.getHeight();

    int height = getHeight();
    int width  = Math.min(getWidth(), (int)(getHeight() * aspectRatio));
    height     = (int)(width / aspectRatio);

    int x = (getWidth() - width) / 2;
    int y = (getHeight() - height) / 2;
    g2d.drawImage(image, x, y, width, height, null);
  }
}
