package space.cougs.ground.gui.modules;

import java.awt.BorderLayout;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.RenderingHints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.AdjustmentEvent;
import java.awt.event.AdjustmentListener;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseWheelEvent;
import java.awt.event.MouseWheelListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JComponent;
import javax.swing.plaf.basic.BasicScrollBarUI;

import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.utils.FileUtils;

public class ThumbnailGrid extends JComponent {
  private static final long serialVersionUID = 1L;

  private final CISScrollBar scroll = new CISScrollBar();

  private final List<Image> thumbnails    = new ArrayList<Image>();
  private final List<File> thumbnailFiles = new ArrayList<File>();
  private final List<ActionListener> actionListeners =
      new ArrayList<ActionListener>();

  private int columns      = 1;
  private int gridHeight   = 0;
  private int squareLength = 0;

  public ThumbnailGrid(int columns) {
    scroll.addAdjustmentListener(adjustmentListener);

    this.columns = columns;
    this.addMouseListener(mouseListener);
    this.addComponentListener(componentListener);
    this.addMouseWheelListener(mouseWheelListener);
    this.setBackground(CustomColors.PRIMARY);
    this.setLayout(new BorderLayout());
    this.add(scroll, BorderLayout.EAST);
  }

  public void addActionListener(ActionListener actionListener) {
    actionListeners.add(actionListener);
  }

  private MouseListener mouseListener = new MouseListener() {
    @Override
    public void mouseClicked(MouseEvent e) {}

    @Override
    public void mouseEntered(MouseEvent e) {}

    @Override
    public void mouseExited(MouseEvent e) {}

    @Override
    public void mousePressed(MouseEvent e) {}

    @Override
    public void mouseReleased(MouseEvent e) {
      int hiddenHeight  = scroll.getValue();
      int currentColumn = e.getX() / squareLength;
      int currentRow    = (e.getY() + hiddenHeight) / squareLength;
      int i             = currentRow * 2 + currentColumn;

      if (i < thumbnailFiles.size()) {
        for (ActionListener listener : actionListeners) {
          ActionEvent event = new ActionEvent(
              e.getSource(), 0, thumbnailFiles.get(i).getAbsolutePath());
          listener.actionPerformed(event);
        }
      }
    }
  };

  private final ComponentListener componentListener = new ComponentListener() {
    @Override
    public void componentShown(ComponentEvent e) {
      this.componentResized(e);
    }

    @Override
    public void componentResized(ComponentEvent e) {
      squareLength = (getWidth() - scroll.getWidth()) / columns;

      int rowCount =
          (int)Math.max(1, Math.ceil((double)thumbnails.size() / columns));
      gridHeight = squareLength * rowCount;
      scroll.setMaximum(gridHeight);
      scroll.setVisibleAmount(getHeight());
      scroll.setUnitIncrement(squareLength / 20);
    }

    @Override
    public void componentMoved(ComponentEvent e) {}

    @Override
    public void componentHidden(ComponentEvent e) {}
  };

  private final AdjustmentListener adjustmentListener =
      new AdjustmentListener() {
        @Override
        public void adjustmentValueChanged(AdjustmentEvent e) {
          repaint();
        }
      };

  private final MouseWheelListener mouseWheelListener =
      new MouseWheelListener() {
        @Override
        public void mouseWheelMoved(MouseWheelEvent e) {
          if (e.getWheelRotation() == 0) {
            return;
          }
          int direction = e.getWheelRotation() < 0 ? -1 : 1;
          if (e.getScrollType() == MouseWheelEvent.WHEEL_UNIT_SCROLL) {
            int units = e.getUnitsToScroll();
            int value = scroll.getValue();
            value += scroll.getUnitIncrement() * units;
            scroll.setValue(value);
          } else if (e.getScrollType() == MouseWheelEvent.WHEEL_BLOCK_SCROLL) {
            int value = scroll.getValue();
            value += scroll.getBlockIncrement(direction);
            scroll.setValue(value);
          }
        }
      };

  public void addThumbnail(File thumbnail) {
    // Only add new images
    if (!thumbnailFiles.contains(thumbnail)) {
      BufferedImage image = FileUtils.getThumbnail(thumbnail);
      if (image == null) {
        return;
      }
      thumbnails.add(image);
      thumbnailFiles.add(thumbnail);
      gridHeight =
          (int)(squareLength * Math.ceil((double)thumbnails.size() / columns));
      this.repaint();
    }
  }

  @Override
  protected void paintComponent(Graphics g) {
    super.paintComponent(g);
    Graphics2D g2d = (Graphics2D)g;
    g2d.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,
        RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

    int xOffset = this.getWidth() - scroll.getWidth() - squareLength * 2;

    g2d.setColor(this.getBackground());
    g2d.fillRect(
        xOffset, 0, this.getWidth() - scroll.getWidth(), this.getHeight());

    int y = -scroll.getValue();
    for (int i = 0; i < thumbnails.size(); i++) {
      int x = squareLength * (i % columns) + xOffset;
      if (y > -squareLength && y < getHeight()) {
        // Only draw visible images
        g2d.drawImage(
            thumbnails.get(i), x, y, squareLength, squareLength, null);
      }
      if (i % columns == columns - 1) {
        y += squareLength;
      }
    }
  }
}
