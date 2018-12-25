package space.cougs.ground.gui.modules;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Insets;
import java.awt.RenderingHints;

import javax.swing.BorderFactory;
import javax.swing.JComponent;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;

public class SingleVerticalBarGraph extends JComponent implements UIScaling {
  private static final long serialVersionUID = 1L;

  private final String label;
  private final int    labelWidth;

  private final boolean printMinMax;
  private final int     min;
  private final int     max;
  private final double  barWidth;

  private double value;
  private double warningHigh;
  private double warningLow;
  private double dangerHigh;
  private double dangerLow;

  private Font  sideFont = Fonts.CONSOLE_12;
  private Color barFill  = CustomColors.ACCENT;

  public SingleVerticalBarGraph(
      String label, int labelWidth, int min, int max) {
    this(label, labelWidth, min, max, 0, 1.0, false, 0.7, 0.3, 0.9, 0.1);
  }

  public SingleVerticalBarGraph(String label, int labelWidth, int min, int max,
      double warningHigh, double warningLow, double dangerHigh,
      double dangerLow) {
    this(label, labelWidth, min, max, 0, 1.0, false, warningHigh, warningLow,
        dangerHigh, dangerLow);
  }

  public SingleVerticalBarGraph(String label, int labelWidth, int min, int max,
      double barWidth, double warningHigh, double warningLow, double dangerHigh,
      double dangerLow) {
    this(label, labelWidth, min, max, 0, barWidth, false, warningHigh,
        warningLow, dangerHigh, dangerLow);
  }

  public SingleVerticalBarGraph(String label, int labelWidth, int min, int max,
      int value, double barWidth, boolean printMinMax, double warningHigh,
      double warningLow, double dangerHigh, double dangerLow) {
    this.label       = label;
    this.labelWidth  = labelWidth;
    this.min         = min;
    this.max         = max;
    this.value       = value;
    this.barWidth    = barWidth;
    this.printMinMax = printMinMax;
    this.warningHigh = warningHigh;
    this.warningLow  = warningLow;
    this.dangerHigh  = dangerHigh;
    this.dangerLow   = dangerLow;

    this.setForeground(CustomColors.SECONDARY_TEXT);
    this.setBackground(CustomColors.SECONDARY.darker());

    this.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, 0));
  }

  @Override
  public Dimension getPreferredSize() {
    return getMinimumSize();
  }

  @Override
  public Dimension getMinimumSize() {
    FontMetrics fontMetrics = this.getFontMetrics(this.getFont());
    Insets      insets      = this.getInsets();
    int         labelWidthPx  = fontMetrics.stringWidth(label);
    if(labelWidth != -1){
      String string = "";
      for (int i = 0; i < labelWidth; i++){
        string += " ";
      }
      labelWidthPx = fontMetrics.stringWidth(string);
    }

    if (barWidth == 1.0 || !printMinMax) {
      int height = fontMetrics.getHeight();
      height += insets.bottom + insets.top;
      labelWidthPx += insets.left + insets.right;
      return new Dimension(labelWidthPx, height);
    } else {
      FontMetrics sideFontMetrics = this.getFontMetrics(sideFont);

      int height     = fontMetrics.getHeight();
      int valueWidth = Math.max(sideFontMetrics.stringWidth(" " + min),
          sideFontMetrics.stringWidth(" " + max));

      valueWidth = (int)(valueWidth / (1 - barWidth));

      int width = Math.max(labelWidthPx, valueWidth);
      height += insets.bottom + insets.top;
      width += insets.left + insets.right;

      return new Dimension(width, height);
    }
  }

  @Override
  protected void paintComponent(Graphics g) {
    super.paintComponent(g);
    Graphics2D g2d = (Graphics2D)g;
    g2d.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,
        RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

    FontMetrics fontMetrics     = this.getFontMetrics(this.getFont());
    FontMetrics sideFontMetrics = this.getFontMetrics(sideFont);

    Insets insets = this.getInsets();

    int adjustedWidth  = this.getWidth() - insets.left - insets.right;
    int adjustedHeight = this.getHeight() - insets.left - insets.right;

    int barHeight = adjustedHeight - fontMetrics.getHeight();
    int barWidth  = (int)(adjustedWidth * this.barWidth);
    int rectX     = (printMinMax) ? insets.left
                              : (adjustedWidth - barWidth) / 2 + insets.left;

    g2d.setColor(this.getBackground());
    g2d.fillRect(rectX, insets.top, barWidth, barHeight);

    int x = (adjustedWidth - fontMetrics.stringWidth(label)) / 2 + insets.left;
    int y = this.getHeight() - insets.bottom - fontMetrics.getDescent();

    g2d.setColor(this.getForeground());
    g2d.drawString(label, x, y);

    g2d.setFont(sideFont);

    if (printMinMax) {
      g2d.drawString(" " + max, insets.left + barWidth,
          insets.top + sideFontMetrics.getAscent());
      g2d.drawString(" " + min, insets.left + barWidth, insets.top + barHeight);
    }

    barHeight = (int)(barHeight * (value - min) / (double)(max - min));
    y = this.getHeight() - fontMetrics.getHeight() - insets.bottom - barHeight;
    g2d.setColor(barFill);
    g2d.fillRect(rectX, y, barWidth, barHeight);

    g2d.setColor(this.getForeground());
    g2d.setFont(sideFont);
    x = (barWidth - sideFontMetrics.stringWidth(String.format("%.1f", value)));
    x = x / 2 + rectX;
    y = y - sideFontMetrics.getDescent();
    y = Math.min(adjustedHeight - fontMetrics.getHeight(),
        Math.max(sideFontMetrics.getAscent(), y));
    g2d.drawString(String.format("%.1f", value), x, y);
  }

  public void setValue(double value, Color barFill) {
    this.value   = value;
    this.barFill = barFill;
    this.repaint();
  }

  public void setValue(double value) {
    this.value = value;

    double percent = ((this.max - this.value) / this.max);

    if ((percent <= dangerLow || ((percent) >= dangerHigh))) {
      this.barFill = CustomColors.STATUS_BAD;
    } else if ((percent) <= warningLow || (percent) >= warningHigh) {
      this.barFill = CustomColors.STATUS_WARN;
    } else {
      this.barFill = CustomColors.STATUS_GOOD;
    }
    this.repaint();
  }

  @Override
  public void updateUIScaling(UIScale uiScale) {
    switch (uiScale) {
      case SCALE_100:
        sideFont = Fonts.CONSOLE_12;
        this.setFont(Fonts.BODY_16);
        break;
      case SCALE_150:
        sideFont = Fonts.CONSOLE_18;
        this.setFont(Fonts.BODY_24);
        break;
      case SCALE_200:
        sideFont = Fonts.CONSOLE_24;
        this.setFont(Fonts.BODY_32);
        break;
      case SCALE_300:
        sideFont = Fonts.CONSOLE_36;
        this.setFont(Fonts.BODY_48);
        break;
      case SCALE_75:
        sideFont = Fonts.CONSOLE_8;
        this.setFont(Fonts.BODY_12);
        break;
      default:
        System.out.println("Single Vertical Bar Graph \"" + label +
                           "\" unknown UIscale: " + uiScale);
        break;
    }
  }
}