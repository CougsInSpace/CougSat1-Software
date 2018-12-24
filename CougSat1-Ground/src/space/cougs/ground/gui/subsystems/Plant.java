package space.cougs.ground.gui.subsystems;

import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.util.ArrayList;
import java.util.List;

import javax.swing.BorderFactory;

import space.cougs.ground.CougSatGround;
import space.cougs.ground.gui.modules.CISPanel;
import space.cougs.ground.gui.modules.HorizontalScrollBar;
import space.cougs.ground.gui.modules.HorizontalValue;
import space.cougs.ground.gui.modules.ImageModule;
import space.cougs.ground.gui.modules.TitleLabel;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.GridBagConstraintsWrapper;
import space.cougs.ground.satellites.CougSat;
import space.cougs.ground.utils.FileUtils;

public class Plant extends CISPanel implements SatelliteInfo {
  private static final long serialVersionUID = 1L;

  private final ImageModule image = new ImageModule();
  private final HorizontalValue time =
      new HorizontalValue("Data Timestamp: ", "     ", 0.5);
  private final HorizontalScrollBar scroll = new HorizontalScrollBar("Day  0");

  private final List<File> imageFiles = new ArrayList<File>();

  private final ActionListener actionListener = new ActionListener() {
    @Override
    public void actionPerformed(ActionEvent e) {
      int day = (int)(Math.floor(28 * scroll.getScrollPosition() + 0.5));
      scroll.setLabel(String.format("Day %2d", day));
      if (day < imageFiles.size()) {
        image.setImage(FileUtils.getImage(imageFiles.get(day)));
      } else {
        image.setImage(null);
      }
    }
  };

  public Plant() {
    super();

    GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();
    gbc.setInsets(5, 5, 5, 5);

    scroll.addActionListner(actionListener);

    this.setLayout(new GridBagLayout());
    this.add(new TitleLabel("Germination Exeriment"),
        gbc.setCommon(0, 0, 2, 1, 1.0, 0.0));
    this.add(image, gbc.setCommon(0, 1, 1, 1, 1.0, 1.0));
    this.add(time, gbc.setCommon(1, 1, 1, 1, 0.0, 1.0));
    this.add(scroll, gbc.setCommon(0, 2, 2, 1, 1.0, 0.0));

    this.setBackground(CustomColors.SECONDARY);
    this.setBorder(BorderFactory.createLineBorder(CustomColors.PRIMARY, 10));

    addImagesToList();
  }

  private void addImagesToList() {
    File dir = new File(
        CougSatGround.getHomeDir().getAbsolutePath() + "\\packets\\plant");
    if (dir.exists()) {
      for (File file : dir.listFiles(FileUtils.imageFilter)) {
        if (!imageFiles.contains(file)) {
          imageFiles.add(file);
        }
      }
    } else {
      System.out.println(
          "Image directory does not exist: " + dir.getAbsolutePath());
    }
  }

  @Override
  public void updateSatellite(CougSat satellite) {
    addImagesToList();
    if (!imageFiles.isEmpty()) {
      image.setImage(FileUtils.getImage(imageFiles.get(0)));
    }
  }
}