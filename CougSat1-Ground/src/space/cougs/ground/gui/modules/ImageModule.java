package space.cougs.ground.gui.modules;

import java.awt.Cursor;
import java.awt.Desktop;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

import javax.swing.JComponent;

import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;

public class ImageModule extends JComponent {
  private static final long serialVersionUID = 1L;

  private BufferedImage image = null;

  private String link = "";

  private final MouseListener mouseListener = new MouseListener() {
    @Override
    public void mouseReleased(MouseEvent e) {
      if (getBounds().contains(e.getPoint()) && !link.isBlank()) {
        try {
          Desktop.getDesktop().browse(new URI(link));
        } catch (IOException | URISyntaxException e1) {
          System.out.println("Image Module failed to open link: " + link);
          e1.printStackTrace();
        }
      }
    }

    @Override
    public void mousePressed(MouseEvent e) {}

    @Override
    public void mouseExited(MouseEvent e) {}

    @Override
    public void mouseEntered(MouseEvent e) {}

    @Override
    public void mouseClicked(MouseEvent e) {}
  };

  public ImageModule() {
    this(null);
  }

  public ImageModule(BufferedImage image) {
    this.image = image;
    this.setFont(Fonts.BODY_16);
    this.addMouseListener(mouseListener);
  }

  public void setImage(BufferedImage image) {
    this.image = image;
    this.repaint();
  }

  public void setLink(String link) {
    this.link = link;
    if (!link.isBlank()) {
      this.setCursor(Cursor.getPredefinedCursor(Cursor.HAND_CURSOR));
    }
  }

  @Override
  protected void paintComponent(Graphics g) {
    super.paintComponent(g);
    Graphics2D g2d = (Graphics2D)(g);

    g2d.setRenderingHint(RenderingHints.KEY_ALPHA_INTERPOLATION,
        RenderingHints.VALUE_ALPHA_INTERPOLATION_QUALITY);
    g2d.setRenderingHint(RenderingHints.KEY_INTERPOLATION,
        RenderingHints.VALUE_INTERPOLATION_BICUBIC);
    g2d.setRenderingHint(
        RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
    g2d.setRenderingHint(
        RenderingHints.KEY_RENDERING, RenderingHints.VALUE_RENDER_QUALITY);

    if (image == null) {
      g2d.setColor(CustomColors.STATUS_BAD);
      FontMetrics fontMetrics = this.getFontMetrics(this.getFont());
      int         width       = fontMetrics.stringWidth("Image not set");
      int         x           = (this.getWidth() - width) / 2;
      int         y = (this.getHeight() / 2) - fontMetrics.getAscent();
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
