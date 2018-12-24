package space.cougs.ground.gui.subsystems;

import java.awt.GridBagLayout;

import javax.swing.BorderFactory;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.data.general.DefaultPieDataset;

import space.cougs.ground.gui.modules.CISPanel;
import space.cougs.ground.gui.modules.TitleLabel;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.GridBagConstraintsWrapper;
import space.cougs.ground.satellites.CougSat;
import space.cougs.ground.utils.Units;

public class CDH extends CISPanel implements SatelliteInfo {
  private static final long serialVersionUID = 1L;

  private final DefaultPieDataset sdUsageData = new DefaultPieDataset();
  private final JFreeChart chart =
      ChartFactory.createPieChart(null, sdUsageData);
  private final ChartPanel chartWrapper = new ChartPanel(chart);

  public CDH() {
    super();

    GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();

    chart.getPlot().setOutlineVisible(false);
    chart.getPlot().setBackgroundPaint(CustomColors.SECONDARY);
    chart.setBackgroundPaint(CustomColors.SECONDARY);

    this.setLayout(new GridBagLayout());
    this.add(chartWrapper, gbc.setCommon(0, 1, 1, 1, 1.0, 1.0));

    this.add(new TitleLabel("C&DH Storage"),
        gbc.setCommon(0, 0, 1, 1, 1.0, 0.0).setInsets(0, 0, 10, 0));
    this.setBorder(BorderFactory.createLineBorder(CustomColors.PRIMARY, 10));
  }

  @Override
  public void updateSatellite(CougSat satellite) {
    sdUsageData.clear();
    long used = satellite.getCDH().getSDCardUsed();
    long free = satellite.getCDH().getSDCardFree();
    sdUsageData.setValue("SD Used: " + Units.toBytes(used), used);
    sdUsageData.setValue("SD Free: " + Units.toBytes(free), free);
  }
}
