package space.cougs.ground.gui.subsystems;

import java.awt.Component;
import java.awt.Container;
import java.awt.Font;
import java.awt.GridBagLayout;
import java.awt.GridLayout;

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

@SuppressWarnings("serial")
public class Health extends JPanel implements UIScaling {

	private final JPanel ihu;
	private final JPanel temperature;
	private final JPanel adcs;
	private final JPanel rcs;
	private final JPanel power;

	private final HorizontalText mode;
	private final HorizontalText time;
	private final HorizontalText SD;
	private final HorizontalText reset;
	private final HorizontalText packets;

	private final SingleVerticalBarGraph rx;
	private final SingleVerticalBarGraph tx;
	private final SingleVerticalBarGraph snr;

	private final SingleVerticalBarGraph ihuTemp;
	private final SingleVerticalBarGraph adcsTemp;
	private final SingleVerticalBarGraph ifjrTemp;
	private final SingleVerticalBarGraph pmicTemp;
	private final SingleVerticalBarGraph rcsTemp;
	private final SingleVerticalBarGraph avgTemp;
	private final SingleVerticalBarGraph pv0Temp;
	private final SingleVerticalBarGraph pv1Temp;
	private final SingleVerticalBarGraph pv2Temp;
	private final SingleVerticalBarGraph pv3Temp;
	private final SingleVerticalBarGraph pv4Temp;
	private final SingleVerticalBarGraph pv5Temp;
	private final SingleVerticalBarGraph pv6Temp;
	private final SingleVerticalBarGraph pv7Temp;
	private final SingleVerticalBarGraph batteryATemp;
	private final SingleVerticalBarGraph batteryBTemp;

	private final Map map;

	public Health() {
		super();

		this.setBackground(CustomColors.BACKGROUND1);
		this.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
		this.setLayout(new GridBagLayout());

		GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();
		gbc.setFill(GridBagConstraintsWrapper.BOTH);

		ihu = new JPanel();
		ihu.setLayout(new GridLayout(0, 1, 5, 5));

		mode = new HorizontalText("Mode:", "Null data", 0.5);
		time = new HorizontalText("Time:", "Null data", 0.5);
		SD = new HorizontalText("SD:", "Null data", 0.5);
		reset = new HorizontalText("Reset:", "Null data", 0.5);
		packets = new HorizontalText("Packets:", "Null data", 0.5);

		ihu.add(new JLabel("Computer", SwingConstants.CENTER));
		ihu.add(mode);
		ihu.add(time);
		ihu.add(SD);
		ihu.add(reset);
		ihu.add(packets);

		rcs = new JPanel();
		rcs.setLayout(new GridBagLayout());

		rx = new SingleVerticalBarGraph("RX (mW)", 0, 100, 10, .5);
		tx = new SingleVerticalBarGraph("TX (mW)", 0, 100, 10, .5);
		snr = new SingleVerticalBarGraph("SNR(dB)", -30, 30, 10, .5);

		rcs.add(new JLabel("Radio", SwingConstants.CENTER),
				gbc.setLocation(0, 0).setSize(3, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		rcs.add(rx, gbc.setLocation(0, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		rcs.add(tx, gbc.setLocation(1, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		rcs.add(snr, gbc.setLocation(2, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));

		temperature = new JPanel();
		temperature.setLayout(new GridBagLayout());

		ihuTemp = new SingleVerticalBarGraph("IHU ", -50, 80, 10, .4);
		adcsTemp = new SingleVerticalBarGraph("ADCS", -50, 80, 10, .4);
		ifjrTemp = new SingleVerticalBarGraph("IFJR", -50, 80, 10, .4);
		pmicTemp = new SingleVerticalBarGraph("PMIC", -50, 80, 10, .4);
		rcsTemp = new SingleVerticalBarGraph("RCS ", -50, 80, 10, .4);
		avgTemp = new SingleVerticalBarGraph("AVG ", -50, 80, 10, .4);
		batteryATemp = new SingleVerticalBarGraph("BatA", -50, 80, 10, .4);
		batteryBTemp = new SingleVerticalBarGraph("BatB", -50, 80, 10, .4);
		pv0Temp = new SingleVerticalBarGraph("PV 0", -50, 80, 10, .4);
		pv1Temp = new SingleVerticalBarGraph("PV 1", -50, 80, 10, .4);
		pv2Temp = new SingleVerticalBarGraph("PV 2", -50, 80, 10, .4);
		pv3Temp = new SingleVerticalBarGraph("PV 3", -50, 80, 10, .4);
		pv4Temp = new SingleVerticalBarGraph("PV 4", -50, 80, 10, .4);
		pv5Temp = new SingleVerticalBarGraph("PV 5", -50, 80, 10, .4);
		pv6Temp = new SingleVerticalBarGraph("PV 6", -50, 80, 10, .4);
		pv7Temp = new SingleVerticalBarGraph("PV 7", -50, 80, 10, .4);

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

		temperature.add(pv0Temp, gbc.setLocation(1, 2).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		temperature.add(pv1Temp, gbc.setLocation(2, 2).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		temperature.add(pv2Temp, gbc.setLocation(3, 2).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		temperature.add(pv3Temp, gbc.setLocation(4, 2).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		temperature.add(pv4Temp, gbc.setLocation(5, 2).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		temperature.add(pv5Temp, gbc.setLocation(6, 2).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		temperature.add(pv6Temp, gbc.setLocation(7, 2).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		temperature.add(pv7Temp, gbc.setLocation(8, 2).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));

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

}
