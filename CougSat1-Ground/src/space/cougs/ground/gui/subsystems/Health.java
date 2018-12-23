package space.cougs.ground.gui.subsystems;

import java.awt.Component;
import java.awt.Container;
import java.awt.Font;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.List;
import java.util.TimeZone;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingConstants;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.subsystems.modules.HorizontalText;
import space.cougs.ground.gui.subsystems.modules.Map;
import space.cougs.ground.gui.subsystems.modules.SingleVerticalBarGraph;
import space.cougs.ground.gui.subsystems.modules.TitleLabel;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;
import space.cougs.ground.gui.utils.GridBagConstraintsWrapper;
import space.cougs.ground.satellites.CougSat;

@SuppressWarnings("serial")
public class Health extends JPanel implements UIScaling, SatelliteInfo {

	private final JPanel ihu = new JPanel();
	private final JPanel temperature = new JPanel();
	private final JPanel adcs = new JPanel();
	private final JPanel comms = new JPanel();
	private final JPanel power = new JPanel();

	private final HorizontalText mode = new HorizontalText("Mode:", "        ", 0.5);
	private final HorizontalText time = new HorizontalText("Time:", "        ", 0.5);
	private final HorizontalText SD = new HorizontalText("SD:", "        ", 0.5);
	private final HorizontalText reset = new HorizontalText("Reset:", "        ", 0.5);
	private final HorizontalText status = new HorizontalText("Status:", "        ", 0.5);

	private final SingleVerticalBarGraph rx = new SingleVerticalBarGraph("RX (mW)", 0, 100, 10, 0.6, false, 1.0, 0.2,
			1.0, 0.1);
	private final SingleVerticalBarGraph tx = new SingleVerticalBarGraph("TX (mW)", 0, 100, 10, 0.6, false, 0.8, 0.0,
			0.9, 0.0);
	private final SingleVerticalBarGraph snr = new SingleVerticalBarGraph("SNR(dB)", -30, 30, 10, 0.6, false, 1.0, 0.2,
			1.0, 0.1);

	private final SingleVerticalBarGraph ihuTemp = new SingleVerticalBarGraph("IHU ", -50, 80, 10, 0.5, false, 0.7, 0.3,
			0.9, 0.1);
	private final SingleVerticalBarGraph adcsTemp = new SingleVerticalBarGraph("ADCS", -50, 80, 10, 0.5, false, 0.7,
			0.3, 0.9, 0.1);
	private final SingleVerticalBarGraph ifjrTemp = new SingleVerticalBarGraph("IFJR", -50, 80, 10, 0.5, false, 0.7,
			0.3, 0.9, 0.1);
	private final SingleVerticalBarGraph pmicTemp = new SingleVerticalBarGraph("PMIC", -50, 80, 10, 0.5, false, 0.7,
			0.3, 0.9, 0.1);
	private final SingleVerticalBarGraph commsTemp = new SingleVerticalBarGraph("COMMS ", -50, 80, 10, 0.5, false, 0.7,
			0.3, 0.9, 0.1);
	private final SingleVerticalBarGraph batteryATemp = new SingleVerticalBarGraph("BatA", -50, 80, 10, 0.5, false, 0.8,
			0.4, 0.9, 0.3);
	private final SingleVerticalBarGraph batteryBTemp = new SingleVerticalBarGraph("BatB", -50, 80, 10, 0.5, false, 0.8,
			0.4, 0.9, 0.3);

	private final List<SingleVerticalBarGraph> pvTemps = new ArrayList<SingleVerticalBarGraph>();

	private final SingleVerticalBarGraph avgPVVoltage = new SingleVerticalBarGraph(" PV V ", 0, 3, 10, 0.5, false,
			0.7, 0.3, 0.9, 0.1);
	private final SingleVerticalBarGraph sumPVCurrent = new SingleVerticalBarGraph(" PV I ", 0, 2, 20, 0.5, false, 0.7,
			0.0, 0.9, 0.0);
	private final SingleVerticalBarGraph sumPR3V3Current = new SingleVerticalBarGraph("PR-3 I", 0, 8, 10, 0.5, false,
			0.7, 0.0, 0.9, 0.0);
	private final SingleVerticalBarGraph sumPRBattCurrent = new SingleVerticalBarGraph("PR-B I", 0, 8, 10, 0.5, false, 0.7,
			0.0, 0.9, 0.0);
	private final SingleVerticalBarGraph sumPV3V3Current = new SingleVerticalBarGraph("PV-3 I", 0, 2, 10, 0.5, false,
			0.7, 0.0, 0.9, 0.0);
	private final SingleVerticalBarGraph battHeaterA = new SingleVerticalBarGraph("BH-A I", 0, 5, 10, 0.5, false, 0.7,
			0.0, 0.9, 0.0);
	private final SingleVerticalBarGraph battHeaterB = new SingleVerticalBarGraph("BH-B I", 0, 5, 10, 0.5, false, 0.7,
			0.0, 0.9, 0.0);
	private final SingleVerticalBarGraph prDeployCurrent = new SingleVerticalBarGraph("PR-D I", 0, 3, 10, 0.5, false,
			0.7, 0.0, 0.9, 0.0);
	private final SingleVerticalBarGraph battACurrent = new SingleVerticalBarGraph("I", 0, 6, 10, 0.5, false, 0.7, 0.0,
			0.9, 0.0);
	private final SingleVerticalBarGraph battAVoltage = new SingleVerticalBarGraph("V", 0, 5, 10, 0.5, false, 0.7, 0.3,
			0.9, 0.1);
	private final SingleVerticalBarGraph battBCurrent = new SingleVerticalBarGraph("I", 0, 6, 10, 0.5, false, 0.7, 0.0,
			0.9, 0.0);
	private final SingleVerticalBarGraph battBVoltage = new SingleVerticalBarGraph("V", 0, 5, 10, 0.5, false, 0.7, 0.3,
			0.9, 0.1);
	private final SingleVerticalBarGraph regulator3V3AVoltage = new SingleVerticalBarGraph("V", 0, 4, 10, 0.5, false,
			0.7, 0.3, 0.9, 0.1);
	private final SingleVerticalBarGraph regulator3V3ACurrent = new SingleVerticalBarGraph("I", 0, 4, 10, 0.5, false,
			0.7, 0.0, 0.9, 0.0);
	private final SingleVerticalBarGraph regulator3V3BVoltage = new SingleVerticalBarGraph("V", 0, 4, 10, 0.5, false,
			0.7, 0.3, 0.9, 0.1);
	private final SingleVerticalBarGraph regulator3V3BCurrent = new SingleVerticalBarGraph("I", 0, 4, 10, 0.5, false,
			0.7, 0.0, 0.9, 0.0);

	private final Map map = new Map(46.7304889, -117.1750474, 1.25);

	public Health() {
		super();

		this.setBackground(CustomColors.BACKGROUND12);
		this.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
		this.setLayout(new GridBagLayout());

		GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();
		gbc.setFill(GridBagConstraintsWrapper.BOTH);

		ihu.setLayout(new GridLayout(0, 1, 5, 5));

		ihu.add(new TitleLabel("Command", SwingConstants.CENTER));
		ihu.add(mode);
		ihu.add(time);
		ihu.add(SD);
		ihu.add(reset);
		ihu.add(status);

		comms.setLayout(new GridBagLayout());

		comms.add(new TitleLabel("Radio", SwingConstants.CENTER),
				gbc.setXY(0, 0).setSize(3, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		comms.add(rx, gbc.setXY(0, 1).setSize(1, 1).setWeight(1.0, 1.0));
		comms.add(tx, gbc.setXY(1, 1).setSize(1, 1).setWeight(1.0, 1.0));
		comms.add(snr, gbc.setXY(2, 1).setSize(1, 1).setWeight(1.0, 1.0));

		temperature.setLayout(new GridBagLayout());

		temperature.add(new TitleLabel("Temperature (�C)", SwingConstants.CENTER),
				gbc.setXY(0, 0).setSize(14, 1).setWeight(0.0, 0.0));
		temperature.add(ihuTemp, gbc.setXY(1, 1).setSize(1, 1).setWeight(1.0, 1.0));
		temperature.add(adcsTemp, gbc.setXY(2, 1).setSize(1, 1).setWeight(1.0, 1.0));
		temperature.add(ifjrTemp, gbc.setXY(3, 1).setSize(1, 1).setWeight(1.0, 1.0));
		temperature.add(pmicTemp, gbc.setXY(4, 1).setSize(1, 1).setWeight(1.0, 1.0));
		temperature.add(commsTemp, gbc.setXY(5, 1).setSize(1, 1).setWeight(1.0, 1.0));
		temperature.add(batteryATemp, gbc.setXY(6, 1).setSize(1, 1).setWeight(1.0, 1.0));
		temperature.add(batteryBTemp, gbc.setXY(7, 1).setSize(1, 1).setWeight(1.0, 1.0));

		for (int i = 0; i < 8; i++) {

			pvTemps.add(new SingleVerticalBarGraph("PV " + i, -50, 80, 10, 0.5, false, 0.7, 0.3, 0.9, 0.1));
			temperature.add(pvTemps.get(i), gbc.setXY(i + 1, 2));
		}

		power.setLayout(new GridBagLayout());

		power.add(new TitleLabel("Power", SwingConstants.CENTER),
				gbc.setXY(0, 0).setSize(14, 1).setWeight(0.0, 0.0));
		power.add(avgPVVoltage, gbc.setXY(1, 1).setSize(1, 1).setWeight(1.0, 1.0));
		power.add(sumPVCurrent, gbc.setXY(2, 1).setSize(1, 1).setWeight(1.0, 1.0));
		power.add(sumPR3V3Current, gbc.setXY(3, 1).setSize(1, 1).setWeight(1.0, 1.0));
		power.add(sumPRBattCurrent, gbc.setXY(4, 1).setSize(1, 1).setWeight(1.0, 1.0));
		power.add(sumPV3V3Current, gbc.setXY(5, 1).setSize(1, 1).setWeight(1.0, 1.0));
		power.add(battHeaterA, gbc.setXY(6, 1).setSize(1, 1).setWeight(1.0, 1.0));
		power.add(battHeaterB, gbc.setXY(7, 1).setSize(1, 1).setWeight(1.0, 1.0));
		power.add(prDeployCurrent, gbc.setXY(8, 1).setSize(1, 1).setWeight(1.0, 1.0));
		power.add(battAVoltage, gbc.setXY(1, 2).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 0, 5));
		power.add(battACurrent, gbc.setXY(2, 2).setSize(1, 1).setWeight(1.0, 1.0));
		power.add(battBVoltage, gbc.setXY(3, 2).setSize(1, 1).setWeight(1.0, 1.0));
		power.add(battBCurrent, gbc.setXY(4, 2).setSize(1, 1).setWeight(1.0, 1.0));
		power.add(regulator3V3AVoltage, gbc.setXY(5, 2).setSize(1, 1).setWeight(1.0, 1.0));
		power.add(regulator3V3ACurrent, gbc.setXY(6, 2).setSize(1, 1).setWeight(1.0, 1.0));
		power.add(regulator3V3BVoltage, gbc.setXY(7, 2).setSize(1, 1).setWeight(1.0, 1.0));
		power.add(regulator3V3BCurrent, gbc.setXY(8, 2).setSize(1, 1).setWeight(1.0, 1.0));
		power.add(prDeployCurrent, gbc.setXY(8, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));

		power.add(new JLabel(" Battery A ", SwingConstants.CENTER),
				gbc.setXY(1, 4).setSize(2, 1).setWeight(0.0, 0.0).setInsets(0, 5, 5, 5));
		power.add(new JLabel(" Battery B ", SwingConstants.CENTER),
				gbc.setXY(3, 4).setSize(2, 1).setWeight(0.0, 0.0));
		power.add(new JLabel("Regulator A", SwingConstants.CENTER),
				gbc.setXY(5, 4).setSize(2, 1).setWeight(0.0, 0.0));
		power.add(new JLabel("Regulator B", SwingConstants.CENTER),
				gbc.setXY(7, 4).setSize(2, 1).setWeight(0.0, 0.0));

		avgPVVoltage.setToolTipText("Solar Panel Average Voltage In");
		sumPVCurrent.setToolTipText("Sum of Solar Panel Current");
		sumPR3V3Current.setToolTipText("Sum of 3.3V Power Rail Currents, Deployables, and Batteries");
		sumPRBattCurrent.setToolTipText("Sum of Unregulated Battery Power Rail Currents");
		sumPV3V3Current.setToolTipText("Sum of Solar Panel 3.3V Current");
		battHeaterA.setToolTipText("Battery Heater A Current");
		battHeaterB.setToolTipText("Battery Heater B Current");
		prDeployCurrent.setToolTipText("Deployables Current");
		battACurrent.setToolTipText("Battery A Current");
		battAVoltage.setToolTipText("Battery A Voltage");
		battBCurrent.setToolTipText("Battery B Current");
		battBVoltage.setToolTipText("Battery B Voltage");
		regulator3V3AVoltage.setToolTipText("3.3V Regulator A Voltage");
		regulator3V3ACurrent.setToolTipText("3.3V Regulator A Current");
		regulator3V3BVoltage.setToolTipText("3.3V Regulator B Voltage");
		regulator3V3BCurrent.setToolTipText("3.3V Regulator B Current");

		adcs.setLayout(new GridBagLayout());

		adcs.add(new TitleLabel("Attitude", SwingConstants.CENTER),
				gbc.setXY(0, 0).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		adcs.add(map, gbc.setXY(0, 1).setSize(1, 1).setWeight(1.0, 1.0));

		this.add(ihu, gbc.setXY(0, 0).setSize(1, 1).setWeight(0.0, 0.0).setInsets(0, 0, 5, 5));
		this.add(adcs, gbc.setXY(0, 1).setSize(2, 2).setWeight(0.0, 1.0).setInsets(5, 0, 0, 5));
		this.add(temperature, gbc.setXY(2, 2).setSize(1, 2).setWeight(1.0, 1.0).setInsets(5, 5, 0, 0));
		this.add(comms, gbc.setXY(1, 0).setSize(1, 1).setWeight(0.0, 0.0).setInsets(0, 5, 5, 5));
		this.add(power, gbc.setXY(2, 0).setSize(1, 2).setWeight(1.0, 1.0).setInsets(0, 5, 5, 0));

		ihu.setBackground(CustomColors.BACKGROUND22);
		temperature.setBackground(CustomColors.BACKGROUND22);
		adcs.setBackground(CustomColors.BACKGROUND22);
		comms.setBackground(CustomColors.BACKGROUND22);
		power.setBackground(CustomColors.BACKGROUND22);

		for (Component component : this.getComponents()) {

			if (component instanceof JPanel) {

				for (Component subComponent : ((Container) component).getComponents()) {

					if (subComponent instanceof JLabel) {

						subComponent.setForeground(CustomColors.TEXT1);
					}
				}
			}
		}
	}

	@Override
	public void updateUIScaling(UIScale uiScale) {

		Font titleFont = Fonts.BODY_16;

		switch (uiScale) {
		case SCALE_100:
			titleFont = Fonts.BODY_16;
			break;
		case SCALE_150:
			titleFont = Fonts.BODY_24;
			break;
		case SCALE_200:
			titleFont = Fonts.BODY_32;
			break;
		case SCALE_300:
			titleFont = Fonts.BODY_48;
			break;
		case SCALE_75:
			titleFont = Fonts.BODY_12;
			break;
		default:
			break;
		}

		for (Component component : this.getComponents()) {

			if (component instanceof UIScaling) {

				((UIScaling) component).updateUIScaling(uiScale);

			}
			if (component instanceof JPanel) {

				for (Component subComponent : ((Container) component).getComponents()) {

					if (subComponent instanceof UIScaling) {

						((UIScaling) subComponent).updateUIScaling(uiScale);

					} else if (subComponent instanceof JLabel && !(subComponent instanceof TitleLabel)) {

						subComponent.setFont(titleFont);
					}
				}
			}
		}
	}

	@Override
	public void updateSatellite(CougSat satellite) {

		mode.setValue(satellite.getCDH().getMode().name());
		SimpleDateFormat format = new SimpleDateFormat("HH:mm:ss");
		format.setTimeZone(TimeZone.getTimeZone("UTC"));
		time.setValue(format.format(satellite.getCDH().getTime()));
		SD.setValue(String.valueOf(satellite.getCDH().getSDCard()));
		reset.setValue(String.valueOf(satellite.getCDH().getResetCount()));
		status.setValue(satellite.getCDH().getErrorStatus().toString());

		rx.setValue(satellite.getComms().getRXPower() * 1000, CustomColors.BAR_DEFAULT);
		tx.setValue(satellite.getComms().getTX700Power() * 1000, CustomColors.BAR_DEFAULT);
		snr.setValue(satellite.getComms().getRXSNR(), CustomColors.BAR_DEFAULT);

		ihuTemp.setValue(satellite.getECS().getIHUTemp());
		adcsTemp.setValue(satellite.getECS().getADCSTemp());
		ifjrTemp.setValue(satellite.getECS().getIFJRTemp());
		pmicTemp.setValue(satellite.getECS().getPMICTemp());
		commsTemp.setValue(satellite.getECS().getCommsTemp());
		batteryATemp.setValue(satellite.getECS().getBatteryTemp(0));
		batteryBTemp.setValue(satellite.getECS().getBatteryTemp(1));

		for (int i = 0; i < 8; i++) {
			pvTemps.get(i).setValue(satellite.getECS().getPVTemp(i));
		}

		avgPVVoltage.setValue(satellite.getEPS().getPVVoltageAvg());
		sumPVCurrent.setValue(satellite.getEPS().getPVCurrentSum());
		sumPR3V3Current.setValue(satellite.getEPS().getPR3V3CurrentSum());
		sumPRBattCurrent.setValue(satellite.getEPS().getPRBattCurrentSum());
		sumPV3V3Current.setValue(satellite.getEPS().getPV3V3CurrentSum());

		battHeaterA.setValue(satellite.getEPS().getPRBHCurrent(0));
		battHeaterB.setValue(satellite.getEPS().getPRBHCurrent(1));
		prDeployCurrent.setValue(satellite.getEPS().getPRDeployablesCurrent());
		battACurrent.setValue(satellite.getEPS().getBatteryCurrent(0));
		battAVoltage.setValue(satellite.getEPS().getBatteryVoltage(0));
		battBCurrent.setValue(satellite.getEPS().getBatteryCurrent(1));
		battBVoltage.setValue(satellite.getEPS().getBatteryVoltage(1));
		regulator3V3AVoltage.setValue(satellite.getEPS().getReg3V3Voltage(0));
		regulator3V3ACurrent.setValue(satellite.getEPS().getReg3V3Current(0));
		regulator3V3BVoltage.setValue(satellite.getEPS().getReg3V3Voltage(1));
		regulator3V3BCurrent.setValue(satellite.getEPS().getReg3V3Current(1));

		map.setValue(satellite.getADCS().getLatitude(), satellite.getADCS().getLongitude());
	}
}
