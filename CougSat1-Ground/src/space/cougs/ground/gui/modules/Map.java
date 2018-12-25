package space.cougs.ground.gui.modules;

import java.awt.Dimension;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Insets;
import java.awt.RenderingHints;
import java.awt.image.BufferedImage;

import javax.swing.BorderFactory;
import javax.swing.JComponent;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;
import space.cougs.ground.utils.FileUtils;

public class Map extends JComponent implements UIScaling {
  private static final long serialVersionUID = 1L;

  private final String longitudeText = "%11.6f\u00B0 E";
  private final String latitudeText  = "%10.6f\u00B0 N";
  private final double aspectRatio;

  private BufferedImage map       = FileUtils.getImage("map.jpg");
  private double        longitude = 0.0;
  private double        latitude  = 0.0;
  private int           diameter  = 6;

  public Map(Double aspectRatio) {
    this(0.0, 0.0, aspectRatio);
  }

  public Map(double latitude, double longitude, Double aspectRatio) {
    super();
    this.latitude  = latitude;
    this.longitude = longitude;

    this.setBackground(CustomColors.PRIMARY);
    this.setForeground(CustomColors.PRIMARY_TEXT);
    this.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));

    if (aspectRatio == null) {
      this.aspectRatio = map.getWidth() / (double)map.getHeight();
    } else {
      this.aspectRatio = aspectRatio;
    }
  }

  @Override
  public Dimension getPreferredSize() {
    return getMinimumSize();
  }

  @Override
  public Dimension getMinimumSize() {
    FontMetrics fontMetrics = this.getFontMetrics(this.getFont());

    Insets insets = this.getInsets();

    int width =
        fontMetrics.stringWidth(String.format(longitudeText, longitude)) * 2;
    int height = ((width - fontMetrics.getHeight()) / 2);

    width += insets.left + insets.right;
    height += insets.top + insets.bottom;
    return new Dimension(width, height);
  }

  @Override
  protected void paintComponent(Graphics g) {
    super.paintComponent(g);
    Graphics2D g2d = (Graphics2D)g;
    g2d.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,
        RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
    g2d.setRenderingHint(RenderingHints.KEY_INTERPOLATION,
        RenderingHints.VALUE_INTERPOLATION_BICUBIC);
    g2d.setRenderingHint(
        RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
    g2d.setRenderingHint(
        RenderingHints.KEY_RENDERING, RenderingHints.VALUE_RENDER_QUALITY);

    FontMetrics fontMetrics = this.getFontMetrics(this.getFont());
    Insets      insets      = this.getInsets();

    g2d.setColor(this.getBackground());
    g2d.fillRect(0, 0, this.getWidth(), this.getHeight());

    int x1 = insets.left;
    int x2 = this.getWidth() - insets.right;
    int y1 = insets.top;
    int y2 = this.getHeight() - insets.bottom - fontMetrics.getHeight();
    y2     = (int)Math.min(y2, (x2 - x1) / aspectRatio);

    int newWidth = (int)(y2 * aspectRatio);

    x1 = (this.getWidth() / 2) - (newWidth / 2);
    x2 = x1 + newWidth;
    g2d.drawImage(
        map, x1, y1, x2, y2, 0, 0, map.getWidth(), map.getHeight(), null);

    int x = x1 - insets.left;
    int y = y2 + fontMetrics.getAscent();
    g2d.setColor(this.getForeground());
    g2d.drawString(String.format(latitudeText, latitude), x, y);

    x = x2 - fontMetrics.stringWidth(String.format(longitudeText, longitude));
    g2d.drawString(String.format(longitudeText, longitude), x, y);

    x = (int)((longitude + 180.0) / 360.0 * (x2 - x1) + insets.left);
    y = (int)((-latitude + 90.0) / 180 * (y2 - y1) + insets.top);
    g2d.fillOval(x1 + x - diameter / 2, y - diameter / 2, diameter, diameter);
  }

  public void setValue(double latitude, double longitude) {
    this.longitude = longitude;
    this.latitude  = latitude;
    this.repaint();
  }

  @Override
  public void updateUIScaling(UIScale uiScale) {
    this.setFont(Fonts.BODY_16);

    switch (uiScale) {
      case SCALE_100:
        this.setFont(Fonts.BODY_16);
        this.diameter = 6;
        break;
      case SCALE_150:
        this.setFont(Fonts.BODY_24);
        this.diameter = 8;
        break;
      case SCALE_200:
        this.setFont(Fonts.BODY_32);
        this.diameter = 12;
        break;
      case SCALE_300:
        this.setFont(Fonts.BODY_48);
        this.diameter = 18;
        break;
      case SCALE_75:
        this.setFont(Fonts.BODY_12);
        this.diameter = 4;
        break;
      default:
        System.out.println("Map unknown UIscale: " + uiScale);
        break;
    }
  }
}