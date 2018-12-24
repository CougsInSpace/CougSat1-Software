
package space.cougs.ground.gui.subsystems;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.io.File;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JPanel;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.modules.PhotoViewer;
import space.cougs.ground.gui.modules.ThumbnailGrid;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.satellites.CougSat;

public class Camera extends JPanel implements UIScaling, SatelliteInfo {
  private static final long serialVersionUID = 1L;

  private static final double thumbnailWidthFactor = 0.3;

  private static final ThumbnailGrid thumbnailGrid = new ThumbnailGrid(2);
  private static final PhotoViewer photoViewer = new PhotoViewer();
  private int                      border = 10;

  private List<File>   cameraFolder = new ArrayList<File>();
  // private ListFiles    itr = new ListFiles();
  private static final File imageFile = new File("packets/images");

  public Camera() {
    super();
    this.setLayout(null);
    this.addComponentListener(componentListener);

    // itr.listFilesForFolder(imageFile, cameraFolder);

    thumbnailGrid.setBackground(CustomColors.BACKGROUND22);
    thumbnailGrid.addActionListner(actionListener);

    photoViewer.setBackground(CustomColors.BACKGROUND22);

    for (File file : cameraFolder) {
      thumbnailGrid.addThumbnail(file);
    }

    this.add(thumbnailGrid);
    this.add(photoViewer);

    this.repaint();
    this.setBackground(CustomColors.BACKGROUND12);
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
          getWidth() - thumbnailWidth - border, getHeight() - border * 2);
      repaint();
    }

    @Override
    public void componentShown(ComponentEvent e) {
      this.componentResized(e);
    }
  };

  private ActionListener actionListener = new ActionListener() {
    @Override
    public void actionPerformed(ActionEvent e) {
      photoViewer.setThumbnail(thumbnailGrid.getCurrentThumbnail());
    }
  };

  @Override
  public void updateUIScaling(UIScale uiScale) {
    // Font titleFont = Fonts.BODY_16;

    // switch (uiScale) {
    // case SCALE_100:
    // titleFont = Fonts.BODY_16;
    // break;
    // case SCALE_150:
    // titleFont = Fonts.BODY_24;
    // break;
    // case SCALE_200:
    // titleFont = Fonts.BODY_32;
    // break;
    // case SCALE_300:
    // titleFont = Fonts.BODY_48;
    // break;
    // case SCALE_75:
    // titleFont = Fonts.BODY_12;
    // break;
    // default:
    // break;
    // }
  }

  @Override
  public void updateSatellite(CougSat satellite) {
    // TODO Auto-generated method stub
  }
}
