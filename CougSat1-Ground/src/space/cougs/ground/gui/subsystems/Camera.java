
package space.cougs.ground.gui.subsystems;

import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.GridBagLayout;
import java.awt.LayoutManager;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;

import space.cougs.ground.CougSatGround;
import space.cougs.ground.gui.modules.CISPanel;
import space.cougs.ground.gui.modules.ImageModule;
import space.cougs.ground.gui.modules.ThumbnailGrid;
import space.cougs.ground.gui.modules.TitleLabel;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.GridBagConstraintsWrapper;
import space.cougs.ground.satellites.CougSat;
import space.cougs.ground.utils.FileUtils;

public class Camera extends CISPanel implements SatelliteInfo {
  private static final long serialVersionUID = 1L;

  private final int    border               = 10;
  private final double thumbnailWidthFactor = 0.25;

  private final ThumbnailGrid thumbnailGrid = new ThumbnailGrid(2);

  private final CISPanel photoViewer = new CISPanel();
  private final ImageModule image    = new ImageModule();

  public Camera() {
    super();

    GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();
    gbc.setInsets(5, 5, 5, 5);

    thumbnailGrid.addActionListener(actionListener);

    photoViewer.setBackground(CustomColors.PRIMARY);
    photoViewer.setLayout(new GridBagLayout());
    photoViewer.add(
        new TitleLabel("Current Image"), gbc.setCommon(0, 0, 1, 1, 1.0, 0.0));
    photoViewer.add(image, gbc.setCommon(0, 1, 1, 1, 1.0, 1.0));

    this.setLayout(layoutManager);
    this.setOpaque(false);
    this.add(thumbnailGrid);
    this.add(photoViewer);

    addThumbnailsToGrid();
  }

  private ActionListener actionListener = new ActionListener() {
    @Override
    public void actionPerformed(ActionEvent e) {
      image.setImage(FileUtils.getImage(new File(e.getActionCommand())));
    }
  };

  private LayoutManager layoutManager = new LayoutManager() {
    @Override
    public void removeLayoutComponent(Component comp) {}

    @Override
    public Dimension preferredLayoutSize(Container parent) {
      return null;
    }

    @Override
    public Dimension minimumLayoutSize(Container parent) {
      return null;
    }

    @Override
    public void layoutContainer(Container parent) {
      int x      = border;
      int y      = border;
      int width  = (int)(getWidth() * thumbnailWidthFactor);
      int height = getHeight() - border * 2;

      thumbnailGrid.setBounds(x, y, width, height);

      x += width + border;
      width = getWidth() - x - border;
      photoViewer.setBounds(x, y, width, height);
    }

    @Override
    public void addLayoutComponent(String name, Component comp) {}
  };

  private void addThumbnailsToGrid() {
    File dir = new File(
        CougSatGround.getHomeDir().getAbsolutePath() + "\\packets\\images");
    if (dir.exists()) {
      for (File file : dir.listFiles(FileUtils.imageFilter)) {
        thumbnailGrid.addThumbnail(file);
      }
    } else {
      System.out.println(
          "Image directory does not exist: " + dir.getAbsolutePath());
    }
  }

  @Override
  public void updateSatellite(CougSat satellite) {
    addThumbnailsToGrid();
  }
}
