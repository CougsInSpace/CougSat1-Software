package space.cougs.ground.gui.modules;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Rectangle;
import java.awt.RenderingHints;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JComponent;

import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.utils.FileUtils;

public class ThumbnailGrid extends JComponent {
  private static final long serialVersionUID = 1L;

  private final List<Image> thumbnails    = new ArrayList<Image>();
  private final List<File> thumbnailFiles = new ArrayList<File>();
  private final List<ActionListener> actionListeners =
      new ArrayList<ActionListener>();

  private final Rectangle scrollBar = new Rectangle(0, 0, 0, 0);

  private double  scrollPosition = 0.0;
  private int     columns        = 1;
  private int     barWidth       = 20;
  private int     gridHeight     = 0;
  private int     squareLength   = 0;
  private int     lastScrollBarY = 0;
  private boolean barClicked     = false;

  public ThumbnailGrid(int columns) {
    this.columns = columns;
    this.addMouseMotionListener(mouseMotionListener);
    this.addMouseListener(mouseListener);
    this.setBackground(CustomColors.SECONDARY);
  }

  public void addActionListner(ActionListener listener) {
    if (!actionListeners.contains(listener)) {
      actionListeners.add(listener);
    }
  }

  private MouseListener mouseListener = new MouseListener() {
    @Override
    public void mouseClicked(MouseEvent e) {}

    @Override
    public void mouseEntered(MouseEvent e) {}

    @Override
    public void mouseExited(MouseEvent e) {}

    @Override
    public void mousePressed(MouseEvent e) {
      if (scrollBar.contains(e.getPoint())) {
        lastScrollBarY = e.getY();
        barClicked     = true;
      }
    }

    @Override
    public void mouseReleased(MouseEvent e) {
      if (!barClicked && e.getX() < squareLength * 2) {
        int hiddenHeight  = (int)(scrollPosition * (gridHeight - getHeight()));
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
      barClicked = false;
    }
  };

  private final MouseMotionListener mouseMotionListener =
      new MouseMotionListener() {
        @Override
        public void mouseDragged(MouseEvent e) {
          if (barClicked) {
            int y = scrollBar.y + e.getY() - lastScrollBarY;
            y     = (int)Math.min(y, getHeight() - scrollBar.getHeight());
            y     = (int)Math.max(0, y);

            scrollBar.y    = y;
            scrollPosition = y / (getHeight() - scrollBar.getHeight());
            lastScrollBarY = e.getY();
            repaint();
          }
        }

        @Override
        public void mouseMoved(MouseEvent e) {}
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
    }
  }

  @Override
  protected void paintComponent(Graphics g) {
    super.paintComponent(g);
    Graphics2D g2d = (Graphics2D)g;
    g2d.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,
        RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

    squareLength = (getWidth() - barWidth) / columns;

    int rowCount =
        (int)Math.max(1, Math.ceil((double)thumbnails.size() / columns));
    gridHeight = squareLength * rowCount;
    scrollBar.setSize(barWidth, getHeight() * getHeight() / gridHeight);
    scrollBar.x = squareLength * 2;
    g2d.setColor(this.getBackground());
    g2d.fillRect(0, 0, squareLength * 2 + scrollBar.width, this.getHeight());

    int y = (int)(scrollPosition * Math.min(0, this.getHeight() - gridHeight));
    for (int i = 0; i < thumbnails.size(); i++) {
      int x = squareLength * (i % columns);
      if (y > -squareLength && y < getHeight()) {
        // Only draw visible images
        g2d.drawImage(
            thumbnails.get(i), x, y, squareLength, squareLength, null);
      }
      if (i % columns == columns - 1) {
        y += squareLength;
      }
    }

    g2d.setColor(CustomColors.ACCENT);
    g2d.fill(scrollBar);
  }
}
