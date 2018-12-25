package space.cougs.ground.gui.subsystems;

import java.awt.GridBagLayout;

import javax.swing.BorderFactory;

import space.cougs.ground.gui.modules.CISPanel;
import space.cougs.ground.gui.modules.PieChart;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.GridBagConstraintsWrapper;
import space.cougs.ground.satellites.CougSat;
import space.cougs.ground.utils.Units;

public class CDH extends CISPanel implements SatelliteInfo {
  private static final long serialVersionUID = 1L;

  private final PieChart pieChart = new PieChart("C&DH Storage Usage");

  public CDH() {
    super();

    GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();

    this.setLayout(new GridBagLayout());
    this.add(pieChart, gbc.setCommon(0, 0, 1, 1, 1.0, 1.0));

    this.setOpaque(false);
    this.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
  }

  @Override
  public void updateSatellite(CougSat satellite) {
    pieChart.clear();
    long used = satellite.getCDH().getSDCardUsed();
    long free = satellite.getCDH().getSDCardFree();
    pieChart.setValue("SD Used: " + Units.toBytes(used), used, CustomColors.STATUS_GOOD);
    pieChart.setValue("SD Free: " + Units.toBytes(free), free, CustomColors.STATUS_WARN);
  }
}
