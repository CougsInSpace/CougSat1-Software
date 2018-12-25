package space.cougs.ground.gui.subsystems;

import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.List;
import java.util.TimeZone;

import javax.swing.BorderFactory;

import space.cougs.ground.gui.modules.BodyLabel;
import space.cougs.ground.gui.modules.HorizontalValue;
import space.cougs.ground.gui.modules.Map;
import space.cougs.ground.gui.modules.SingleVerticalBarGraph;
import space.cougs.ground.gui.modules.TitleLabel;
import space.cougs.ground.gui.modules.CISPanel;
import space.cougs.ground.gui.utils.GridBagConstraintsWrapper;
import space.cougs.ground.satellites.CougSat;
import space.cougs.ground.utils.Units;

public class Health extends CISPanel implements SatelliteInfo {
  private static final long serialVersionUID = 1L;

  private final CISPanel adcs        = new CISPanel();
  private final CISPanel comms       = new CISPanel();
  private final CISPanel cdh         = new CISPanel();
  private final CISPanel power       = new CISPanel();
  private final CISPanel temperature = new CISPanel();

  private final HorizontalValue mode   = new HorizontalValue("Mode:", 9, 0.5);
  private final HorizontalValue time   = new HorizontalValue("Time:", 9, 0.5);
  private final HorizontalValue SD     = new HorizontalValue("SD:", 9, 0.5);
  private final HorizontalValue reset  = new HorizontalValue("Reset:", 9, 0.5);
  private final HorizontalValue status = new HorizontalValue("Status:", 9, 0.5);

  private final SingleVerticalBarGraph rx =
      new SingleVerticalBarGraph("RX (mW)", 8, 0, 100);
  private final SingleVerticalBarGraph tx = new SingleVerticalBarGraph(
      "TX (mW)", 8, 0, 1500, 1.0, 0.7, 0.0, 0.9, 0.0);
  private final SingleVerticalBarGraph snr = new SingleVerticalBarGraph(
      "SNR (dB)", 8, -30, 30, 1.0, 1.0, 0.5, 1.0, 0.4);

  private final SingleVerticalBarGraph cdhTemp =
      new SingleVerticalBarGraph("C&DH", 6, -50, 80);
  private final SingleVerticalBarGraph adcsTemp =
      new SingleVerticalBarGraph("ADCS", 6, -50, 80);
  private final SingleVerticalBarGraph ifjrTemp =
      new SingleVerticalBarGraph("IFJR", 6, -50, 80);
  private final SingleVerticalBarGraph epsTemp =
      new SingleVerticalBarGraph("EPS", 6, -50, 80);
  private final SingleVerticalBarGraph commsTemp =
      new SingleVerticalBarGraph("Comms", 6, -50, 80);
  private final SingleVerticalBarGraph batteryATemp =
      new SingleVerticalBarGraph("Batt A", 6, -50, 80);
  private final SingleVerticalBarGraph batteryBTemp =
      new SingleVerticalBarGraph("Batt B", 6, -50, 80);
  private final SingleVerticalBarGraph plantTemp =
      new SingleVerticalBarGraph("Plant", 6, -50, 80);

  private final List<SingleVerticalBarGraph> pvTemps =
      new ArrayList<SingleVerticalBarGraph>();

  private final SingleVerticalBarGraph avgPVVoltage =
      new SingleVerticalBarGraph("V", 5, 0, 3);
  private final SingleVerticalBarGraph sumPVCurrent =
      new SingleVerticalBarGraph("I", 5, 0, 2, 1.0, 0.7, 0.0, 0.9, 0.0);
  private final SingleVerticalBarGraph sumInPower =
      new SingleVerticalBarGraph("In", 5, 0, 5, 1.0, 0.7, 0.3, 0.9, 0.1);
  private final SingleVerticalBarGraph sumOutPower =
      new SingleVerticalBarGraph("Out", 5, 0, 5, 1.0, 0.7, 0.0, 0.9, 0.0);
  private final SingleVerticalBarGraph sumPR3V3Power =
      new SingleVerticalBarGraph("3.3V", 5, 0, 8, 1.0, 0.7, 0.0, 0.9, 0.0);
  private final SingleVerticalBarGraph sumPRBattPower =
      new SingleVerticalBarGraph("Batt", 5, 0, 8, 1.0, 0.7, 0.0, 0.9, 0.0);
  private final SingleVerticalBarGraph sumHeatPower =
      new SingleVerticalBarGraph("Heat", 5, 0, 5, 1.0, 0.7, 0.0, 0.9, 0.0);
  private final SingleVerticalBarGraph sumCommsPower =
      new SingleVerticalBarGraph("Comms", 5, 0, 5, 1.0, 0.7, 0.0, 0.9, 0.0);
  private final SingleVerticalBarGraph battAVoltage =
      new SingleVerticalBarGraph("V", 5, 0, 5);
  private final SingleVerticalBarGraph battACurrent =
      new SingleVerticalBarGraph("I", 5, 0, 6, 1.0, 0.7, 0.0, 0.9, 0.0);
  private final SingleVerticalBarGraph battBVoltage =
      new SingleVerticalBarGraph("V", 5, 0, 5);
  private final SingleVerticalBarGraph battBCurrent =
      new SingleVerticalBarGraph("I", 5, 0, 6, 1.0, 0.7, 0.0, 0.9, 0.0);
  private final SingleVerticalBarGraph regulator3V3AVoltage =
      new SingleVerticalBarGraph("V", 5, 0, 4);
  private final SingleVerticalBarGraph regulator3V3ACurrent =
      new SingleVerticalBarGraph("I", 5, 0, 4, 1.0, 0.7, 0.0, 0.9, 0.0);
  private final SingleVerticalBarGraph regulator3V3BVoltage =
      new SingleVerticalBarGraph("V", 5, 0, 4);
  private final SingleVerticalBarGraph regulator3V3BCurrent =
      new SingleVerticalBarGraph("I", 5, 0, 4, 1.0, 0.7, 0.0, 0.9, 0.0);

  private final Map map = new Map(1.25);

  public Health() {
    super();

    this.setOpaque(false);
    this.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
    this.setLayout(new GridBagLayout());

    GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();

    cdh.setLayout(new GridLayout(0, 1, 5, 5));
    cdh.add(new TitleLabel("Command"));
    cdh.add(mode);
    cdh.add(time);
    cdh.add(SD);
    cdh.add(reset);
    cdh.add(status);

    gbc.setInsets(5, 5, 5, 5);
    comms.setLayout(new GridBagLayout());
    comms.add(new TitleLabel("Radio"), gbc.setCommon(0, 0, 3, 1, 1.0, 0.0));
    comms.add(rx, gbc.setCommon(0, 1, 1, 1).setWeight(1.0, 1.0));
    comms.add(tx, gbc.setXY(1, 1));
    comms.add(snr, gbc.setXY(2, 1));

    temperature.setLayout(new GridBagLayout());
    temperature.add(new TitleLabel("Temperature (\u00B0C)"),
        gbc.setCommon(0, 0, 8, 1, 1.0, 0.0));
    temperature.add(cdhTemp, gbc.setCommon(0, 1, 1, 1, 1.0, 1.0));
    temperature.add(adcsTemp, gbc.setXY(1, 1));
    temperature.add(ifjrTemp, gbc.setXY(2, 1));
    temperature.add(epsTemp, gbc.setXY(3, 1));
    temperature.add(commsTemp, gbc.setXY(4, 1));
    temperature.add(batteryATemp, gbc.setXY(5, 1));
    temperature.add(batteryBTemp, gbc.setXY(6, 1));
    temperature.add(plantTemp, gbc.setXY(7, 1));

    for (int i = 0; i < 8; i++) {
      pvTemps.add(new SingleVerticalBarGraph("PV " + i, 5, -50, 80));
      temperature.add(pvTemps.get(i), gbc.setXY(i, 2));
    }

    power.setLayout(new GridBagLayout());
    gbc.setInsets(0, 5, 0, 5);
    power.add(new TitleLabel("Power"), gbc.setCommon(0, 0, 8, 1, 1.0, 0.0));
    power.add(avgPVVoltage, gbc.setCommon(0, 1, 1, 1, 1.0, 1.0));
    power.add(sumPVCurrent, gbc.setXY(1, 1));
    power.add(sumInPower, gbc.setXY(2, 1));
    power.add(sumOutPower, gbc.setXY(3, 1));
    power.add(sumPR3V3Power, gbc.setXY(4, 1));
    power.add(sumPRBattPower, gbc.setXY(5, 1));
    power.add(sumHeatPower, gbc.setXY(6, 1));
    power.add(sumCommsPower, gbc.setXY(7, 1));
    power.add(battAVoltage, gbc.setXY(0, 3));
    power.add(battACurrent, gbc.setXY(1, 3));
    power.add(battBVoltage, gbc.setXY(2, 3));
    power.add(battBCurrent, gbc.setXY(3, 3));
    power.add(regulator3V3AVoltage, gbc.setXY(4, 3));
    power.add(regulator3V3ACurrent, gbc.setXY(5, 3));
    power.add(regulator3V3BVoltage, gbc.setXY(6, 3));
    power.add(regulator3V3BCurrent, gbc.setXY(7, 3));

    power.add(new BodyLabel("Solar Cells"),
        gbc.setCommon(0, 2, 2, 1, 0.0, 0.0).setInsets(0, 5, 5, 5));
    power.add(new BodyLabel("System Power"), gbc.setXY(2, 2));
    power.add(new BodyLabel(" Battery A "), gbc.setXY(0, 4));
    power.add(new BodyLabel(" Battery B "), gbc.setXY(2, 4));
    power.add(new BodyLabel("Regulator A"), gbc.setXY(4, 4));
    power.add(new BodyLabel("Regulator B"), gbc.setXY(6, 4));
    power.add(new BodyLabel("Various Rails Power"), gbc.setCommon(4, 2, 4, 1));

    adcs.setLayout(new GridBagLayout());

    adcs.add(new TitleLabel("Attitude"),
        gbc.setCommon(0, 0, 1, 1, 1.0, 0.0).setInsets(5, 5, 5, 5));
    adcs.add(map, gbc.setCommon(0, 1, 1, 1, 1.0, 1.0));

    this.add(cdh, gbc.setCommon(0, 0, 1, 1, 0.0, 0.0).setInsets(0, 0, 5, 5));
    this.add(adcs, gbc.setCommon(0, 1, 2, 2, 0.0, 1.0).setInsets(5, 0, 0, 5));
    this.add(
        temperature, gbc.setCommon(2, 2, 1, 2, 1.0, 1.0).setInsets(5, 5, 0, 0));
    this.add(comms, gbc.setCommon(1, 0, 1, 1, 0.0, 0.0).setInsets(0, 5, 5, 5));
    this.add(power, gbc.setCommon(2, 0, 1, 2, 1.0, 1.0).setInsets(0, 5, 5, 0));

    setToolTips();
  }

  private void setToolTips() {
    avgPVVoltage.setToolTipText("Solar Panel Average Voltage");
    sumPVCurrent.setToolTipText("Sum of Solar Panel Current");
    sumInPower.setToolTipText("Total Input Power");
    sumOutPower.setToolTipText("Total Output Power");
    sumPR3V3Power.setToolTipText("Total 3.3V Rail Power");
    sumPRBattPower.setToolTipText("Total Battery Rail Power");
    sumHeatPower.setToolTipText("Total Battery Heater Power");
    sumCommsPower.setToolTipText("Total Comms Power");
    battACurrent.setToolTipText("Battery A Current");
    battAVoltage.setToolTipText("Battery A Voltage");
    battBCurrent.setToolTipText("Battery B Current");
    battBVoltage.setToolTipText("Battery B Voltage");
    regulator3V3AVoltage.setToolTipText("3.3V Regulator A Voltage");
    regulator3V3ACurrent.setToolTipText("3.3V Regulator A Current");
    regulator3V3BVoltage.setToolTipText("3.3V Regulator B Voltage");
    regulator3V3BCurrent.setToolTipText("3.3V Regulator B Current");
  }

  @Override
  public void updateSatellite(CougSat satellite) {
    mode.setValue(satellite.getCDH().getMode().name());
    SD.setValue(Units.toBytes(satellite.getCDH().getSDCardUsed()));
    reset.setValue(String.valueOf(satellite.getCDH().getResetCount()));
    status.setValue(satellite.getCDH().getErrorStatus().toString());

    SimpleDateFormat format = new SimpleDateFormat("HH:mm:ss");
    format.setTimeZone(TimeZone.getTimeZone("UTC"));
    time.setValue(format.format(satellite.getCDH().getTime() * 1000));

    rx.setValue(satellite.getComms().getRXPower() * 1000);
    tx.setValue(satellite.getComms().getTXPower() * 1000);
    snr.setValue(satellite.getComms().getRXSNR());

    cdhTemp.setValue(satellite.getECS().getCDHTemp());
    adcsTemp.setValue(satellite.getECS().getADCSTemp());
    ifjrTemp.setValue(satellite.getECS().getIFJRTemp());
    epsTemp.setValue(satellite.getECS().getEPSTemp());
    commsTemp.setValue(satellite.getECS().getCommsTemp());
    batteryATemp.setValue(satellite.getECS().getBatteryTemp(0));
    batteryBTemp.setValue(satellite.getECS().getBatteryTemp(1));

    for (int i = 0; i < 8; i++) {
      pvTemps.get(i).setValue(satellite.getECS().getPVTemp(i));
    }

    avgPVVoltage.setValue(satellite.getEPS().getPVVoltageAvg());
    sumPVCurrent.setValue(satellite.getEPS().getPVCurrentSum());
    sumInPower.setValue(satellite.getEPS().getInPowerSum());
    sumOutPower.setValue(satellite.getEPS().getOutPowerSum());
    sumPR3V3Power.setValue(satellite.getEPS().getPR3V3PowerSum());
    sumPRBattPower.setValue(satellite.getEPS().getPRBattPowerSum());
    sumHeatPower.setValue(satellite.getEPS().getHeaterPowerSum());
    sumCommsPower.setValue(satellite.getEPS().getCommsPowerSum());

    battACurrent.setValue(satellite.getEPS().getBatteryCurrent(0));
    battAVoltage.setValue(satellite.getEPS().getBatteryVoltage(0));
    battBCurrent.setValue(satellite.getEPS().getBatteryCurrent(1));
    battBVoltage.setValue(satellite.getEPS().getBatteryVoltage(1));
    regulator3V3AVoltage.setValue(satellite.getEPS().getReg3V3Voltage(0));
    regulator3V3ACurrent.setValue(satellite.getEPS().getReg3V3Current(0));
    regulator3V3BVoltage.setValue(satellite.getEPS().getReg3V3Voltage(1));
    regulator3V3BCurrent.setValue(satellite.getEPS().getReg3V3Current(1));

    map.setValue(
        satellite.getADCS().getLatitude(), satellite.getADCS().getLongitude());
  }
}
