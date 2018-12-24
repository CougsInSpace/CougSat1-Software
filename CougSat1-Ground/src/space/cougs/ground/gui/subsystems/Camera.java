
package space.cougs.ground.gui.subsystems;

import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
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
  private final double thumbnailWidthFactor = 0.3;

  private final ThumbnailGrid thumbnailGrid = new ThumbnailGrid(2);

  private final CISPanel photoViewer = new CISPanel();
  private final ImageModule image    = new ImageModule();

  public Camera() {
    super();

    thumbnailGrid.addActionListner(actionListener);

    GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();
    gbc.setInsets(5, 5, 5, 5);

    photoViewer.setBackground(CustomColors.SECONDARY);
    photoViewer.setLayout(new GridBagLayout());
    photoViewer.add(
        new TitleLabel("Current Image"), gbc.setCommon(0, 0, 1, 1, 1.0, 0.0));
    photoViewer.add(image, gbc.setCommon(0, 1, 1, 1, 1.0, 1.0));

    this.setLayout(null);
    this.addComponentListener(componentListener);
    this.setOpaque(false);
    this.add(thumbnailGrid);
    this.add(photoViewer);

    addThumbnailsToGrid();
  }

  private final ComponentListener componentListener = new ComponentListener() {
    @Override
    public void componentHidden(ComponentEvent e) {}

    @Override
    public void componentMoved(ComponentEvent e) {}

    @Override
    public void componentResized(ComponentEvent e) {
      int thumbnailWidth = (int)(getWidth() * thumbnailWidthFactor);
      thumbnailGrid.setBounds(border, border, thumbnailWidth - border * 2,
          getHeight() - border * 2);
      photoViewer.setBounds(thumbnailWidth, border,
          getWidth() - thumbnailWidth - border * 2, getHeight() - border * 2);
    }

    @Override
    public void componentShown(ComponentEvent e) {
      this.componentResized(e);
    }
  };

  private ActionListener actionListener = new ActionListener() {
    @Override
    public void actionPerformed(ActionEvent e) {
      image.setImage(FileUtils.getImage(new File(e.getActionCommand())));
    }
  };

  private void addThumbnailsToGrid(){
    File dir = new File(CougSatGround.getHomeDir().getAbsolutePath() + "\\packets\\images");
    if(dir.exists()){
      for(File file : dir.listFiles(FileUtils.imageFilter)){
        thumbnailGrid.addThumbnail(file);
      }
    }else {
      System.out.println("Image directory does not exist: " + dir.getAbsolutePath());
    }
  }

  @Override
  public void updateSatellite(CougSat satellite) {
    addThumbnailsToGrid();
  }
}
