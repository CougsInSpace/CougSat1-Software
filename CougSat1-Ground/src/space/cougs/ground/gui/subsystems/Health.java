package space.cougs.ground.gui.subsystems;

import java.awt.Component;
import java.awt.Container;
import java.awt.Font;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.util.ArrayList;
import java.util.List;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingConstants;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.subsystems.modules.HorizontalText;
import space.cougs.ground.gui.subsystems.modules.Map;
import space.cougs.ground.gui.subsystems.modules.SingleVerticalBarGraph;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;
import space.cougs.ground.gui.utils.GridBagConstraintsWrapper;
import space.cougs.ground.satellites.CougSat;

@SuppressWarnings("serial")
public class Health extends JPanel implements UIScaling, SatelliteInfo {

	private final JPanel ihu;
	private final JPanel temperature;
	private final JPanel adcs;
	private final JPanel comms;
	private final JPanel power;

	private final HorizontalText mode = new HorizontalText("Mode:", "Null data", 0.5);
	private final HorizontalText time = new HorizontalText("Time:", "Null data", 0.5);
	private final HorizontalText SD = new HorizontalText("SD:", "Null data", 0.5);
	private final HorizontalText reset = new HorizontalText("Reset:", "Null data", 0.5);
	private final HorizontalText status = new HorizontalText("Status:", "Null data", 0.5);

	private final SingleVerticalBarGraph rx = new SingleVerticalBarGraph("RX (mW)", 0, 100, 10, .5);
	private final SingleVerticalBarGraph tx = new SingleVerticalBarGraph("TX (mW)", 0, 100, 10, .5);
	private final SingleVerticalBarGraph snr = new SingleVerticalBarGraph("SNR(dB)", -30, 30, 10, .5);

	private final SingleVerticalBarGraph ihuTemp = new SingleVerticalBarGraph("IHU ", -50, 80, 10, .4);
	private final SingleVerticalBarGraph adcsTemp = new SingleVerticalBarGraph("ADCS", -50, 80, 10, .4);
	private final SingleVerticalBarGraph ifjrTemp = new SingleVerticalBarGraph("IFJR", -50, 80, 10, .4);
	private final SingleVerticalBarGraph pmicTemp = new SingleVerticalBarGraph("PMIC", -50, 80, 10, .4);
	private final SingleVerticalBarGraph commsTemp = new SingleVerticalBarGraph("COMMS ", -50, 80, 10, .4);
	private final SingleVerticalBarGraph avgTemp = new SingleVerticalBarGraph("AVG ", -50, 80, 10, .4);
	private final SingleVerticalBarGraph batteryATemp = new SingleVerticalBarGraph("BatA", -50, 80, 10, .4);
	private final SingleVerticalBarGraph batteryBTemp = new SingleVerticalBarGraph("BatB", -50, 80, 10, .4);

	private final List<SingleVerticalBarGraph> pvTemps = new ArrayList<SingleVerticalBarGraph>();

	private final SingleVerticalBarGraph avgPVVoltageIn = new SingleVerticalBarGraph("PV V", 0, 3, 10, .4);
	private final SingleVerticalBarGraph sumPVCurrent = new SingleVerticalBarGraph("PV I", 0, 2, 20, .4);
	private final SingleVerticalBarGraph sumPR3V3Current = new SingleVerticalBarGraph("PR-3 I", 0, 8, 10, .4);
	private final SingleVerticalBarGraph sumBatteryPR = new SingleVerticalBarGraph("PR-B I", 0, 8, 10, .4);
	private final SingleVerticalBarGraph sumPV3v3Current = new SingleVerticalBarGraph("PV-3 I", 0, 2, 10, .4);
	private final SingleVerticalBarGraph battHeaterA = new SingleVerticalBarGraph("BH-A I", 0, 5, 10, .4);
	private final SingleVerticalBarGraph battHeaterB = new SingleVerticalBarGraph("BH-B I", 0, 5, 10, .4);
	private final SingleVerticalBarGraph prDeployCurrent = new SingleVerticalBarGraph("PR-D I", 0, 3, 10, .4);
	private final SingleVerticalBarGraph battACurrent = new SingleVerticalBarGraph("BT-A I", 0, 6, 10, .4);
	private final SingleVerticalBarGraph battAVoltage = new SingleVerticalBarGraph("BT-A V", 0, 5, 10, .4);
	private final SingleVerticalBarGraph battBCurrent = new SingleVerticalBarGraph("BT-B I", 0, 6, 10, .4);
	private final SingleVerticalBarGraph battBVoltage = new SingleVerticalBarGraph("BT-B V", 0, 5, 10, .4);
	private final SingleVerticalBarGraph regulator3v3AVoltage = new SingleVerticalBarGraph("REG-A I", 0, 4, 10, .4);
	private final SingleVerticalBarGraph regulator3v3ACurrent = new SingleVerticalBarGraph("REG-A V", 0, 4, 10, .4);
	private final SingleVerticalBarGraph regulator3v3BVoltage = new SingleVerticalBarGraph("REG-B I", 0, 4, 10, .4);
	private final SingleVerticalBarGraph regulator3v3BCurrent = new SingleVerticalBarGraph("REG-B V", 0, 4, 10, .4);

	private final Map map = new Map(46.7304889, -117.1750474);

	public Health() {
		super();

		for (int i = 0; i < 8; i++) {
			pvTemps.add(new SingleVerticalBarGraph("PV " + i, -50, 80, 10, .4));
		}

		this.setBackground(CustomColors.BACKGROUND1);
		this.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
		this.setLayout(new GridBagLayout());

		GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();
		gbc.setFill(GridBagConstraintsWrapper.BOTH);

		ihu = new JPanel();
		ihu.setLayout(new GridLayout(0, 1, 5, 5));

		ihu.add(new JLabel("Computer", SwingConstants.CENTER));
		ihu.add(mode);
		ihu.add(time);
		ihu.add(SD);
		ihu.add(reset);
		ihu.add(status);

		comms = new JPanel();
		comms.setLayout(new GridBagLayout());

		comms.add(new JLabel("Radio", SwingConstants.CENTER),
				gbc.setLocation(0, 0).setSize(3, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		comms.add(rx, gbc.setLocation(0, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		comms.add(tx, gbc.setLocation(1, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		comms.add(snr, gbc.setLocation(2, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));

		temperature = new JPanel();
		temperature.setLayout(new GridBagLayout());

		temperature.add(new JLabel("Temperature (°C)", SwingConstants.CENTER),
				gbc.setLocation(0, 0).setSize(14, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		temperature.add(ihuTemp, gbc.setLocation(1, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		temperature.add(adcsTemp, gbc.setLocation(2, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		temperature.add(ifjrTemp, gbc.setLocation(3, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		temperature.add(pmicTemp, gbc.setLocation(4, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		temperature.add(commsTemp, gbc.setLocation(5, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		temperature.add(batteryATemp, gbc.setLocation(6, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		temperature.add(batteryBTemp, gbc.setLocation(7, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		temperature.add(avgTemp, gbc.setLocation(8, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));

		gbc.setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5);

		for (int i = 0; i < 8; i++) {

			temperature.add(pvTemps.get(i), gbc.setLocation(i + 1, 2));
		}

		power = new JPanel();
		power.setLayout(new GridBagLayout());

		power.add(new JLabel("Power", SwingConstants.CENTER),
				gbc.setLocation(0, 0).setSize(14, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		power.add(avgPVVoltageIn, gbc.setLocation(1, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		power.add(sumPVCurrent, gbc.setLocation(2, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		power.add(sumPR3V3Current, gbc.setLocation(3, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		power.add(sumBatteryPR, gbc.setLocation(4, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		power.add(sumPV3v3Current, gbc.setLocation(5, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		power.add(battHeaterA, gbc.setLocation(6, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		power.add(battHeaterB, gbc.setLocation(7, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		power.add(prDeployCurrent, gbc.setLocation(8, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		power.add(battACurrent, gbc.setLocation(1, 2).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		power.add(battAVoltage, gbc.setLocation(2, 2).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		power.add(battBCurrent, gbc.setLocation(3, 2).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		power.add(battBVoltage, gbc.setLocation(4, 2).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		power.add(regulator3v3AVoltage, gbc.setLocation(5, 2).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		power.add(regulator3v3ACurrent, gbc.setLocation(6, 2).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		power.add(regulator3v3BVoltage, gbc.setLocation(7, 2).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		power.add(regulator3v3BCurrent, gbc.setLocation(8, 2).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));

		avgPVVoltageIn.setToolTipText("Solar Panel Average Voltage Im");
		sumPVCurrent.setToolTipText("Sum of Solar Panel Current");
		sumPR3V3Current.setToolTipText("Sum of Current for Power Rail 3.3v, Deployables, and battery");
		sumBatteryPR.setToolTipText("Sum of Power Rail Battery currents");
		sumPV3v3Current.setToolTipText("Sum of Solar Panel 3.3V Current");
		battHeaterA.setToolTipText("Current of Battery Heater A");
		battHeaterB.setToolTipText("Current of Battery Heater B");
		prDeployCurrent.setToolTipText("Power Rail Deployables current");
		battACurrent.setToolTipText("Battery A Current");
		battAVoltage.setToolTipText("Battery A Voltage");
		battBCurrent.setToolTipText("Battery B Current");
		battBVoltage.setToolTipText("Battery B Voltage");
		regulator3v3AVoltage.setToolTipText("3.3V Regulator A Voltage");
		regulator3v3ACurrent.setToolTipText("3.3V Regulator A Current");
		regulator3v3BVoltage.setToolTipText("3.3V Regulator B Voltage");
		regulator3v3BCurrent.setToolTipText("3.3V Regulator B Current");

		for (int i = 0; i < 2; i++) {
			power.add(prDeployCurrent, gbc.setLocation(8, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		}

		adcs = new JPanel();
		adcs.setLayout(new GridBagLayout());

		adcs.add(new JLabel("Attitude", SwingConstants.CENTER),
				gbc.setLocation(0, 0).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		adcs.add(map, gbc.setLocation(0, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));

		this.add(ihu, gbc.setLocation(0, 0).setSize(1, 1).setWeight(0.0, 0.0).setInsets(0, 0, 5, 5));
		this.add(adcs, gbc.setLocation(0, 1).setSize(2, 2).setWeight(0.0, 1.0).setInsets(5, 0, 0, 5));
		this.add(temperature, gbc.setLocation(2, 2).setSize(1, 2).setWeight(1.0, 1.0).setInsets(5, 5, 0, 0));
		this.add(comms, gbc.setLocation(1, 0).setSize(1, 1).setWeight(0.0, 0.0).setInsets(0, 5, 5, 5));
		this.add(power, gbc.setLocation(2, 0).setSize(1, 2).setWeight(1.0, 1.0).setInsets(0, 5, 5, 0));

		ihu.setBackground(CustomColors.BACKGROUND2);
		temperature.setBackground(CustomColors.BACKGROUND2);
		adcs.setBackground(CustomColors.BACKGROUND2);
		comms.setBackground(CustomColors.BACKGROUND2);
		power.setBackground(CustomColors.BACKGROUND2);

	}

	@Override
	public void updateUIScaling(UIScale uiScale) {

		Font titleFont = Fonts.TITLE_16;

		switch (uiScale) {
		case SCALE_100:
			titleFont = Fonts.TITLE_16;
			break;
		case SCALE_150:
			titleFont = Fonts.TITLE_24;
			break;
		case SCALE_200:
			titleFont = Fonts.TITLE_32;
			break;
		case SCALE_300:
			titleFont = Fonts.TITLE_48;
			break;
		case SCALE_75:
			titleFont = Fonts.TITLE_12;
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

					} else if (subComponent instanceof JLabel) {

						subComponent.setFont(titleFont);
						subComponent.setForeground(CustomColors.TEXT1);
					}
				}
			}
		}
	}

	@Override
	public void updateSatellite(CougSat satellite) {

		mode.setValue(satellite.getMode());
		time.setValue(satellite.getTime());
		SD.setValue(String.valueOf(satellite.getSDCard()));
		reset.setValue(String.valueOf(satellite.getResetCount()));
		status.setValue(satellite.getErrorStatus().toString());

		rx.setValue((int) satellite.getRXPower() * 1000, CustomColors.BAR_DEFAULT);
		tx.setValue((int) satellite.getTxPower() * 1000, CustomColors.BAR_DEFAULT);
		snr.setValue((int) satellite.getRXSNR(), CustomColors.BAR_DEFAULT);

		ihuTemp.setValue(satellite.getIHUTemp(), CustomColors.BAR_DEFAULT);
		adcsTemp.setValue(satellite.getADCSTemp(), CustomColors.BAR_DEFAULT);
		ifjrTemp.setValue(satellite.getIFJRTemp(), CustomColors.BAR_DEFAULT);
		pmicTemp.setValue(satellite.getPMICTemp(), CustomColors.BAR_DEFAULT);
		commsTemp.setValue(satellite.getCommsTemp(), CustomColors.BAR_DEFAULT);
		batteryATemp.setValue(satellite.getBatteryATemp(), CustomColors.BAR_DEFAULT);
		batteryBTemp.setValue(satellite.getBatteryBTemp(), CustomColors.BAR_DEFAULT);

		for (int i = 0; i < 8; i++) {
			pvTemps.get(i).setValue(satellite.getPVTemp(i), CustomColors.BAR_DEFAULT);
		}
		// avg temp

		// avgPVVoltageIn.setValue(satellite, CustomColors.BAR_DEFAULT);
		// sumPVCurrent.setValue(satellite, CustomColors.BAR_DEFAULT);
		// sumPR3V3Current.setValue(value, barFill);
		// sumBatteryPR.setValue(value, barFill);
		// sumPV3v3Current.setValue(value, barFill);
		battHeaterA.setValue(satellite.getPRBatteryHeaterACurrent(), CustomColors.BAR_DEFAULT);
		System.out.println(satellite.getPRBatteryHeaterACurrent());
		battHeaterB.setValue(satellite.getPRBatteryHeaterBCurrent(), CustomColors.BAR_DEFAULT);
		prDeployCurrent.setValue(satellite.getPRDeployablesCurrent(), CustomColors.BAR_DEFAULT);
		battACurrent.setValue(satellite.getBatteryACurrent(), CustomColors.BAR_DEFAULT);
		battAVoltage.setValue(satellite.getBatteryAVoltage(), CustomColors.BAR_DEFAULT);
		battBCurrent.setValue(satellite.getBatteryBCurrent(), CustomColors.BAR_DEFAULT);
		battBVoltage.setValue(satellite.getBatteryBVoltage(), CustomColors.BAR_DEFAULT);
		regulator3v3AVoltage.setValue(satellite.getReg3V3AVoltage(), CustomColors.BAR_DEFAULT);
		regulator3v3ACurrent.setValue(satellite.getReg3V3ACurrent(), CustomColors.BAR_DEFAULT);
		regulator3v3BVoltage.setValue(satellite.getReg3V3BVoltage(), CustomColors.BAR_DEFAULT);
		regulator3v3BCurrent.setValue(satellite.getReg3V3BCurrent(), CustomColors.BAR_DEFAULT);

		map.setValue(satellite.getLattitude(), satellite.getLongitude());

	}
}
