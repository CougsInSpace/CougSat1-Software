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
	private final JPanel rcs;
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
	private final SingleVerticalBarGraph rcsTemp = new SingleVerticalBarGraph("RCS ", -50, 80, 10, .4);
	private final SingleVerticalBarGraph avgTemp = new SingleVerticalBarGraph("AVG ", -50, 80, 10, .4);
	private final SingleVerticalBarGraph batteryATemp = new SingleVerticalBarGraph("BatA", -50, 80, 10, .4);
	private final SingleVerticalBarGraph batteryBTemp = new SingleVerticalBarGraph("BatB", -50, 80, 10, .4);

	private final List<SingleVerticalBarGraph> pvTemps = new ArrayList<SingleVerticalBarGraph>();

	private final Map map;

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

		rcs = new JPanel();
		rcs.setLayout(new GridBagLayout());

		rcs.add(new JLabel("Radio", SwingConstants.CENTER),
				gbc.setLocation(0, 0).setSize(3, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		rcs.add(rx, gbc.setLocation(0, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		rcs.add(tx, gbc.setLocation(1, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		rcs.add(snr, gbc.setLocation(2, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));

		temperature = new JPanel();
		temperature.setLayout(new GridBagLayout());

		temperature.add(new JLabel("Temperature (°C)", SwingConstants.CENTER),
				gbc.setLocation(0, 0).setSize(14, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		temperature.add(ihuTemp, gbc.setLocation(1, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		temperature.add(adcsTemp, gbc.setLocation(2, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		temperature.add(ifjrTemp, gbc.setLocation(3, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		temperature.add(pmicTemp, gbc.setLocation(4, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		temperature.add(rcsTemp, gbc.setLocation(5, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		temperature.add(batteryATemp, gbc.setLocation(6, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		temperature.add(batteryBTemp, gbc.setLocation(7, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		temperature.add(avgTemp, gbc.setLocation(8, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));

		gbc.setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5);

		for (int i = 0; i < 8; i++) {

			temperature.add(pvTemps.get(i), gbc.setLocation(i + 1, 2));
		}

		power = new JPanel();
		power.add(new JLabel("Power", SwingConstants.CENTER));

		adcs = new JPanel();
		adcs.setLayout(new GridBagLayout());

		map = new Map(46.7304889, -117.1750474);

		adcs.add(new JLabel("Attitude", SwingConstants.CENTER),
				gbc.setLocation(0, 0).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		adcs.add(map, gbc.setLocation(0, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));

		this.add(ihu, gbc.setLocation(0, 0).setSize(1, 1).setWeight(0.0, 0.0).setInsets(0, 0, 5, 5));
		this.add(adcs, gbc.setLocation(0, 1).setSize(2, 2).setWeight(0.0, 1.0).setInsets(5, 0, 0, 5));
		this.add(temperature, gbc.setLocation(2, 2).setSize(1, 2).setWeight(1.0, 1.0).setInsets(5, 5, 0, 0));
		this.add(rcs, gbc.setLocation(1, 0).setSize(1, 1).setWeight(0.0, 0.0).setInsets(0, 5, 5, 5));
		this.add(power, gbc.setLocation(2, 0).setSize(1, 2).setWeight(1.0, 1.0).setInsets(0, 5, 5, 0));

		ihu.setBackground(CustomColors.BACKGROUND2);
		temperature.setBackground(CustomColors.BACKGROUND2);
		adcs.setBackground(CustomColors.BACKGROUND2);
		rcs.setBackground(CustomColors.BACKGROUND2);
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
		rcsTemp.setValue(satellite.getRCSTemp(), CustomColors.BAR_DEFAULT);
		batteryATemp.setValue(satellite.getBatteryATemp(), CustomColors.BAR_DEFAULT);
		batteryBTemp.setValue(satellite.getBatteryBTemp(), CustomColors.BAR_DEFAULT);
		for (int i = 0; i < 8; i++) {
			pvTemps.get(i).setValue(satellite.getPVTemp(i), CustomColors.BAR_DEFAULT);
		}
		// avg temp

		map.setValue(satellite.getLattitude(), satellite.getLongitude());

	}
}
