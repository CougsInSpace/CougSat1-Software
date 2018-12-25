package space.cougs.ground.gui.modules;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.util.LinkedList;
import java.util.List;

import javax.swing.JComponent;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;

public class PieChart extends JComponent implements UIScaling {
  private static final long serialVersionUID = 1L;

  private final List<Data> datas = new LinkedList<Data>();
  private final String title;
  private final int    border = 10;

  private double totalValue = 0.0;

  private Font dataFont = Fonts.CONSOLE_12;

  private class Data {
    public String label;
    public double value;
    public Color  color;

    public Data(String label, double value, Color color) {
      this.label = label;
      this.value = value;
      this.color = color;
    }
  }

  public PieChart(String title) {
    super();
    this.title = title;
    this.setBackground(CustomColors.PRIMARY);
    this.setForeground(CustomColors.PRIMARY_TEXT);
    this.setFont(Fonts.BODY_16);
  }

  @Override
  public Dimension getPreferredSize() {
    return getMinimumSize();
  }

  @Override
  public Dimension getMinimumSize() {
    FontMetrics fontMetrics = this.getFontMetrics(this.getFont());

    int width  = fontMetrics.stringWidth(title);
    int height = width + fontMetrics.getHeight();

    return new Dimension(width, height);
  }

  @Override
  public void paintComponent(Graphics g) {
    super.paintComponents(g);
    Graphics2D g2d = (Graphics2D)g;
    g2d.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,
        RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
    g2d.setRenderingHint(
        RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

    FontMetrics fontMetrics     = this.getFontMetrics(this.getFont());
    FontMetrics dataFontMetrics = this.getFontMetrics(dataFont);

    g2d.setColor(this.getBackground());
    g2d.fillRect(0, 0, this.getWidth(), this.getHeight());

    int x = (this.getWidth() - fontMetrics.stringWidth(title)) / 2;
    int y = fontMetrics.getAscent();
    g2d.setColor(this.getForeground());
    g2d.drawString(title, x, y);

    int height = dataFontMetrics.getHeight();
    int labelWidth = 0;
    for (Data data : datas) {
      labelWidth += dataFontMetrics.stringWidth(data.label);
      labelWidth += height * 3 /2;
    }

    x = (this.getWidth() - labelWidth) / 2;
    y = this.getHeight() - dataFontMetrics.getDescent();

    int colorHeight = dataFontMetrics.getAscent();
    g2d.setFont(dataFont);
    for (Data data : datas) {
      g2d.setColor(data.color);
      g2d.fillRect(x, y - colorHeight, colorHeight, colorHeight);
      x += height;

      g2d.setColor(this.getForeground());
      g2d.drawString(data.label, x, y);
      x += dataFontMetrics.stringWidth(data.label) + height / 2;
    }

    int diameter = (this.getHeight() - fontMetrics.getHeight() -
                    dataFontMetrics.getHeight() - border * 2);
    diameter     = Math.min(diameter, this.getWidth() - border * 2);

    x = (this.getWidth() - diameter) / 2;
    y = (this.getHeight() - fontMetrics.getHeight() -
         dataFontMetrics.getHeight() - diameter);
    y = y / 2 + fontMetrics.getHeight();
    drawDataSlices(g2d, x, y, diameter);
    g2d.setColor(this.getBackground());
    g2d.setStroke(new BasicStroke(3));
    g2d.drawOval(x, y, diameter, diameter);
  }

  private void drawDataSlices(Graphics2D g2d, int x, int y, int diameter) {
    if (datas.isEmpty()) {
      return;
    }
    double currentAngle = 90.0;
    g2d.setColor(datas.get(0).color);
    g2d.fillOval(x, y, diameter, diameter);
    for (Data data : datas) {
      g2d.setColor(data.color);
      double arcAngle = 360.0 * data.value / totalValue;
      g2d.fillArc(x, y, diameter, diameter, (int)currentAngle, (int)arcAngle);
      currentAngle += arcAngle;
    }
  }

  public void clear() {
    datas.clear();
  }

  public void setValue(String label, double value, Color color) {
    boolean update = false;
    totalValue     = 0.0;
    for (Data data : datas) {
      if (data.label.equals(label)) {
        data.value = value;
        data.color = color;
        update     = true;
      }
      totalValue += data.value;
    }
    if (!update) {
      datas.add(new Data(label, value, color));
      totalValue += value;
    }
    this.repaint();
  }

  @Override
  public void updateUIScaling(UIScale uiScale) {
    switch (uiScale) {
      case SCALE_100:
        dataFont = Fonts.CONSOLE_12;
        this.setFont(Fonts.BODY_16);
        break;
      case SCALE_150:
        dataFont = Fonts.CONSOLE_18;
        this.setFont(Fonts.BODY_24);
        break;
      case SCALE_200:
        dataFont = Fonts.CONSOLE_24;
        this.setFont(Fonts.BODY_32);
        break;
      case SCALE_300:
        dataFont = Fonts.CONSOLE_36;
        this.setFont(Fonts.BODY_48);
        break;
      case SCALE_75:
        dataFont = Fonts.CONSOLE_12;
        this.setFont(Fonts.BODY_12);
        break;
      default:
        System.out.println(
            "Pie Chart \"" + title + "\" unknown UIscale: " + uiScale);
        break;
    }
  }
}