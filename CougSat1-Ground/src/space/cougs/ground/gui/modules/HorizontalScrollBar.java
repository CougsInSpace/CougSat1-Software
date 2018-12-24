package space.cougs.ground.gui.modules;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Insets;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.event.ActionListener;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.util.ArrayList;
import java.util.List;

import javax.swing.BorderFactory;
import javax.swing.JComponent;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;

public class HorizontalScrollBar extends JComponent implements UIScaling {
  private static final long serialVersionUID = 1L;

  private final double barLength    = 0.2;
  private final Rectangle scrollBar = new Rectangle(0, 0, 0, 0);
  private final List<ActionListener> actionListeners =
      new ArrayList<ActionListener>();

  private String label    = "";
  private Color  barColor = CustomColors.ACCENT;

  private double  scrollPosition = 0.0;
  private int     lastScrollBarX = 0;
  private boolean barClicked     = false;

  private final ComponentListener componentListener = new ComponentListener() {
    @Override
    public void componentShown(ComponentEvent e) {
      componentResized(e);
    }

    @Override
    public void componentResized(ComponentEvent e) {
      FontMetrics fontMetrics = getFontMetrics(getFont());
      Insets      insets      = getInsets();

      int width = (int)(getWidth() * barLength);
      width     = Math.max(
          width, fontMetrics.stringWidth(label) + insets.left + insets.right);

      scrollBar.setSize(width, getHeight());
    }

    @Override
    public void componentMoved(ComponentEvent e) {}

    @Override
    public void componentHidden(ComponentEvent e) {}
  };

  private MouseListener mouseListener = new MouseListener() {
    @Override
    public void mouseClicked(MouseEvent e) {}

    @Override
    public void mouseEntered(MouseEvent e) {}

    @Override
    public void mouseExited(MouseEvent e) {}

    @Override
    public void mousePressed(MouseEvent e) {
      lastScrollBarX = e.getX();
      barClicked     = true;
      if (!scrollBar.contains(e.getPoint())) {
        int x = (int)(lastScrollBarX - scrollBar.getWidth() / 2);
        x     = (int)Math.min(x, getWidth() - scrollBar.getWidth());
        x     = (int)Math.max(0, x);

        scrollPosition = x / (getWidth() - scrollBar.getWidth());
        lastScrollBarX = e.getX();
        repaint();
        for (ActionListener listener : actionListeners) {
          listener.actionPerformed(null);
        }
      }
    }

    @Override
    public void mouseReleased(MouseEvent e) {
      barClicked = false;
    }
  };

  private final MouseMotionListener mouseMotionListener =
      new MouseMotionListener() {
        @Override
        public void mouseDragged(MouseEvent e) {
          if (barClicked) {
            int x = scrollBar.x + e.getX() - lastScrollBarX;
            x     = (int)Math.min(x, getWidth() - scrollBar.getWidth());
            x     = (int)Math.max(0, x);

            scrollPosition = x / (getWidth() - scrollBar.getWidth());
            lastScrollBarX = e.getX();
            repaint();
          }
          for (ActionListener listener : actionListeners) {
            listener.actionPerformed(null);
          }
        }

        @Override
        public void mouseMoved(MouseEvent e) {}
      };

  public HorizontalScrollBar(String label) {
    this.label = label;
    this.setFont(Fonts.BODY_16);
    this.setBackground(CustomColors.PRIMARY);
    this.setForeground(CustomColors.PRIMARY_TEXT);
    this.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));
    this.addComponentListener(componentListener);
    this.addMouseListener(mouseListener);
    this.addMouseMotionListener(mouseMotionListener);
  }

  public void addActionListner(ActionListener listener) {
    if (!actionListeners.contains(listener)) {
      actionListeners.add(listener);
    }
  }

  @Override
  public Dimension getPreferredSize() {
    return getMinimumSize();
  }

  @Override
  public Dimension getMinimumSize() {
    FontMetrics fontMetrics = this.getFontMetrics(this.getFont());
    Insets      insets      = this.getInsets();

    int width  = fontMetrics.stringWidth(label);
    int height = fontMetrics.getHeight();
    width += insets.right + insets.left;
    height += insets.top + insets.bottom;

    width = (int)(width / barLength);

    return new Dimension(width, height);
  }

  @Override
  protected void paintComponent(Graphics g) {
    super.paintComponent(g);
    Graphics2D g2d = (Graphics2D)g;
    g2d.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,
        RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

    FontMetrics fontMetrics = this.getFontMetrics(this.getFont());

    g2d.setColor(this.getBackground());
    g2d.fillRect(0, 0, this.getWidth(), this.getHeight());

    scrollBar.x =
        (int)((this.getWidth() - scrollBar.getWidth()) * scrollPosition);

    g2d.setColor(this.barColor);
    g2d.fill(scrollBar);

    int x = scrollBar.x;
    int y =
        (getHeight() - fontMetrics.getHeight()) / 2 + fontMetrics.getAscent();

    x += (int)((scrollBar.getWidth() - fontMetrics.stringWidth(label)) / 2);
    g2d.setColor(this.getForeground());
    g2d.drawString(label, x, y);
  }

  /**
   * @return the scrollPosition
   */
  public double getScrollPosition() {
    return scrollPosition;
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
        break;
      case SCALE_150:
        this.setFont(Fonts.BODY_24);
        break;
      case SCALE_200:
        this.setFont(Fonts.BODY_32);
        break;
      case SCALE_300:
        this.setFont(Fonts.BODY_48);
        break;
      case SCALE_75:
        this.setFont(Fonts.BODY_12);
        break;
      default:
        System.out.println(
            "Horizontal Scroll Bar unknown UI Scale: " + uiScale);
        break;
    }
  }
}