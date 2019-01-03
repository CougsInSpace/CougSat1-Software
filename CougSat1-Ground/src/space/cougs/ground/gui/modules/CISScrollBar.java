package space.cougs.ground.gui.modules;

import java.awt.Dimension;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.RenderingHints;

import javax.swing.JButton;
import javax.swing.JComponent;
import javax.swing.JScrollBar;
import javax.swing.plaf.ScrollBarUI;
import javax.swing.plaf.basic.BasicScrollBarUI;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;

public class CISScrollBar extends JScrollBar implements UIScaling {
  private static final long serialVersionUID = 1L;

  private String label = "";
  private int scrollBarSize = 16;

  private final ScrollBarUI ui = new BasicScrollBarUI() {
    @Override
    protected Dimension getMinimumThumbSize() {
      FontMetrics fontMetrics = getFontMetrics(getFont());
      int height = fontMetrics.getHeight() + 4;
      int width = fontMetrics.stringWidth(label) + 4;
      return new Dimension(width, height);
    }

    @Override
    protected JButton createDecreaseButton(int orientation) {
      return new CISArrowButton(orientation);
    }

    @Override
    protected JButton createIncreaseButton(int orientation) {
      return new CISArrowButton(orientation);
    }

    @Override
    protected void configureScrollBarColors() {
      thumbColor = CustomColors.ACCENT.darker();
      trackColor = CustomColors.PRIMARY.darker();
    }

    @Override
    protected void paintThumb(Graphics g, JComponent c, Rectangle thumbBounds) {
      if (thumbBounds.isEmpty() || !scrollbar.isEnabled()) {
        return;
      }

      Graphics2D g2d = (Graphics2D) g;
      g2d.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,
          RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

      g.translate(thumbBounds.x, thumbBounds.y);
      g.setColor(thumbColor);
      g.fillRect(0, 0, thumbBounds.width, thumbBounds.height);
      g.setColor(CustomColors.PRIMARY_TEXT);
      FontMetrics fontMetrics = getFontMetrics(getFont());
      int x = (thumbBounds.width - fontMetrics.stringWidth(label)) / 2;
      int y = (thumbBounds.height - fontMetrics.getHeight()) / 2;
      y = y + fontMetrics.getAscent();
      g.drawString(label, x, y);
      g.translate(-thumbBounds.x, -thumbBounds.y);
    }
  };

  public CISScrollBar() {
    this("", VERTICAL, 0, 10, 0, 100);
  }

  public CISScrollBar(int orientation) {
    this("", orientation, 0, 10, 0, 100);
  }

  public CISScrollBar(String label, int orientation) {
    this(label, orientation, 0, 10, 0, 100);
  }

  public CISScrollBar(
      String label, int orientation, int value, int extent, int min, int max) {
    super(orientation, value, extent, min, max);
    this.setUI(ui);
    this.label = label;
    this.setFont(Fonts.BODY_16);
    this.setBackground(CustomColors.PRIMARY);
    this.setForeground(CustomColors.PRIMARY_TEXT);
  }

  @Override
  public Dimension getPreferredSize() {
    return getMinimumSize();
  }

  @Override
  public Dimension getMinimumSize() {
    if (label.isEmpty()) {
      if (this.getOrientation() == VERTICAL) {
        return new Dimension(scrollBarSize, 48);
      } else {
        return new Dimension(48, scrollBarSize);
      }
    } else {
      FontMetrics fontMetrics = this.getFontMetrics(this.getFont());
      int         textHeight  = fontMetrics.getHeight();
      if (this.getOrientation() == VERTICAL) {
        return new Dimension(textHeight + 4, 48);
      } else {
        return new Dimension(48, textHeight + 4);
      }
    }
  }

  /**
   * @param label the label to set
   */
  public void setLabel(String label) {
    this.label = label;
    this.repaint();
  }

  @Override
  public void updateUIScaling(UIScale uiScale) {
    switch (uiScale) {
      case SCALE_100:
        this.setFont(Fonts.BODY_16);
        scrollBarSize = 16;
        break;
      case SCALE_150:
        this.setFont(Fonts.BODY_24);
        scrollBarSize = 24;
        break;
      case SCALE_200:
        this.setFont(Fonts.BODY_32);
        scrollBarSize = 32;
        break;
      case SCALE_300:
        this.setFont(Fonts.BODY_48);
        scrollBarSize = 48;
        break;
      case SCALE_75:
        this.setFont(Fonts.BODY_12);
        scrollBarSize = 12;
        break;
      default:
        System.out.println(
            "Horizontal Scroll Bar unknown UI Scale: " + uiScale);
        break;
    }
  }
}