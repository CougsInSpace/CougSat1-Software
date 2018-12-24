package space.cougs.ground.gui.modules;

import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.image.BufferedImage;

import javax.swing.JComponent;

import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;

public class ImageModule extends JComponent {
  private static final long serialVersionUID = 1L;

  private BufferedImage image = null;

  public ImageModule() {
    this(null);
  }

  public ImageModule(BufferedImage image) {
    this.image = image;
    this.setFont(Fonts.BODY_16);
  }

  public void setImage(BufferedImage image) {
    this.image = image;
    this.repaint();
  }

  @Override
  protected void paintComponent(Graphics g) {
    super.paintComponent(g);
    Graphics2D g2d = (Graphics2D)(g);

    g2d.setRenderingHint(RenderingHints.KEY_ALPHA_INTERPOLATION,
        RenderingHints.VALUE_ALPHA_INTERPOLATION_QUALITY);

    if (image == null) {
      g2d.setColor(CustomColors.STATUS_BAD);
      FontMetrics fontMetrics = this.getFontMetrics(this.getFont());
      int width = fontMetrics.stringWidth("Image not set");
      int x = (this.getWidth() - width) / 2;
      int y = (this.getHeight() / 2) - fontMetrics.getAscent();
      g2d.drawString("Image not set", x, y);
      return;
    }

    double aspectRatio = (double)image.getWidth() / image.getHeight();

    int height = getHeight();
    int width  = Math.min(getWidth(), (int)(getHeight() * aspectRatio));
    height     = (int)(width / aspectRatio);

    int x = (getWidth() - width) / 2;
    int y = (getHeight() - height) / 2;
    g2d.drawImage(image, x, y, width, height, null);
  }
}
