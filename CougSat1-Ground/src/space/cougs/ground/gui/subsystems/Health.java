package space.cougs.ground.gui.subsystems;

import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Font;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.util.ArrayList;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.SwingConstants;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.subsystems.modules.HorizontalText;
import space.cougs.ground.gui.subsystems.modules.Map;
import space.cougs.ground.gui.subsystems.modules.SingleVerticalBarGraph;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;
import space.cougs.ground.gui.utils.GridBagConstraintsWrapper;
import space.cougs.ground.satellites.CougSat1Telemetry;

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

		temperature.add(new JLabel("Temperature (�C)", SwingConstants.CENTER),
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

	private class RCSHealth extends JPanel {

		private JProgressBar txBar = new JProgressBar(SwingConstants.VERTICAL, 0, 100);
		private JProgressBar rxBar = new JProgressBar(SwingConstants.VERTICAL, 0, 100);
		private JProgressBar snrBar = new JProgressBar(SwingConstants.VERTICAL, 0, 100);
		private JLabel commsLabel = new JLabel("Communications", SwingConstants.CENTER);
		private JLabel rxLabel = new JLabel("RX", SwingConstants.CENTER);
		private JLabel txLabel = new JLabel("TX", SwingConstants.CENTER);
		private JLabel snrLabel = new JLabel("SNR", SwingConstants.CENTER);
		private JLabel txData = new JLabel("", SwingConstants.CENTER);
		private JLabel rxData = new JLabel("", SwingConstants.CENTER);
		private JLabel snrxData = new JLabel("", SwingConstants.CENTER);

		ArrayList<JLabel> labels = new ArrayList<JLabel>();
		ArrayList<JProgressBar> data = new ArrayList<JProgressBar>();

		public RCSHealth() { // Rewrites appearance of communications in the
								// health panel
			super();

			labels.add(commsLabel);
			labels.add(rxLabel);
			labels.add(txLabel);
			labels.add(snrLabel);
			labels.add(rxData);
			labels.add(txData);
			labels.add(snrxData);

			for (JLabel label : labels) {
				// label.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().PLAIN_32]);
				label.setForeground(Color.WHITE);
			}

			data.add(txBar);
			data.add(rxBar);
			data.add(snrBar);

			for (JProgressBar dataPoint : data) {
				dataPoint.setBackground(CustomColors.BACKGROUND1);
				dataPoint.setForeground(CustomColors.BACKGROUND3);
			}

			this.setLayout(new GridBagLayout());
			GridBagConstraints gbc = new GridBagConstraints();
			gbc.fill = GridBagConstraints.BOTH;
			gbc.insets = new Insets(0, 5, 0, 5);
			gbc.gridx = 0;
			gbc.gridy = 0;
			gbc.weightx = 1.0;
			gbc.weighty = 1.0;
			gbc.gridheight = 1;
			gbc.gridwidth = 3;
			this.add(commsLabel, gbc);

			gbc.gridy = 1;
			gbc.gridwidth = 1;
			gbc.weighty = 15.0;
			gbc.insets = new Insets(0, 20, 0, 20);
			this.add(txBar, gbc);

			gbc.gridx = 1;
			this.add(rxBar, gbc);

			gbc.gridx = 2;
			this.add(snrBar, gbc);

			gbc.gridx = 0;
			gbc.gridy = 2;
			gbc.weighty = 1.0;
			gbc.insets = new Insets(0, 0, 0, 0);
			this.add(txData, gbc);

			gbc.gridy = 3;
			this.add(txLabel, gbc);

			gbc.gridy = 2;
			gbc.gridx = 1;
			this.add(rxData, gbc);

			gbc.gridy = 3;
			this.add(rxLabel, gbc);

			gbc.gridy = 2;
			gbc.gridx = 2;
			this.add(snrxData, gbc);

			gbc.gridy = 3;
			this.add(snrLabel, gbc);

		}

		private void updateData(CougSat1Telemetry data) {

			int newTXPower = (int) ((data.getTXPower() / 4.0) * 100.0);
			int newRXPower = (int) ((data.getRXPower() / 0.1) * 100.0);
			int newRXSNR = (int) ((data.getRXSNR() / 30.0) * 100.0);

			txData.setText(String.format("%4.3f W", data.getTXPower()));
			rxData.setText(String.format("%4.0f mW", data.getRXPower() * 1000));
			snrxData.setText(String.format("%4.1f dB", data.getRXSNR()));

			txBar.setValue(newTXPower);
			rxBar.setValue(newRXPower);
			snrBar.setValue(newRXSNR);

		}

	}

	private class IHUHealth extends JPanel {

		JLabel computer = new JLabel("Computer", SwingConstants.CENTER);
		JLabel modeLabel = new JLabel("Mode:", SwingConstants.LEFT);
		JLabel mode = new JLabel("Normal", SwingConstants.LEFT);
		JLabel timeLabel = new JLabel("Time:", SwingConstants.LEFT);
		JLabel time = new JLabel("23:59:59", SwingConstants.LEFT);
		JLabel sdPieLabel = new JLabel("SD", SwingConstants.LEFT);
		JLabel sdUsed = new JLabel("32.00 GB", SwingConstants.LEFT);
		JLabel resetLabel = new JLabel("Times Reset:", SwingConstants.LEFT);
		JLabel reset = new JLabel("1", SwingConstants.LEFT);
		JLabel payloadLabel = new JLabel("Payload-Packets Ready:", SwingConstants.LEFT);
		JLabel payload = new JLabel("100", SwingConstants.LEFT);

		public IHUHealth() {

			this.setLayout(new GridBagLayout());
			GridBagConstraints gbc = new GridBagConstraints();
			gbc.insets = new Insets(5, 5, 5, 5);
			gbc.fill = GridBagConstraints.HORIZONTAL;
			gbc.gridx = 0;
			gbc.gridy = 0;
			gbc.weightx = 1.0;
			gbc.weighty = 1.0;
			gbc.gridheight = 1;
			gbc.gridwidth = 2;
			computer.setFont(Fonts.BODY_24);
			computer.setForeground(Color.WHITE);
			this.add(computer, gbc);

			gbc.gridwidth = 1;
			gbc.gridx = 0;
			gbc.gridy = 1;
			// modeLabel.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().PLAIN_32]);
			modeLabel.setForeground(Color.WHITE);
			mode.setBackground(CustomColors.BACKGROUND1);
			mode.setOpaque(true);
			this.add(modeLabel, gbc);

			gbc.gridx = 1;

			// mode.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().PLAIN_32]);
			mode.setForeground(Color.WHITE);
			this.add(mode, gbc);

			gbc.gridx = 0;
			gbc.gridy = 2;
			// timeLabel.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().PLAIN_32]);
			timeLabel.setForeground(Color.WHITE);
			this.add(timeLabel, gbc);

			gbc.gridx = 1;

			// time.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().PLAIN_32]);
			time.setForeground(Color.WHITE);
			this.add(time, gbc);

			gbc.gridx = 0;
			gbc.gridy = 3;
			// sdPieLabel.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().PLAIN_32]);
			sdPieLabel.setForeground(Color.WHITE);
			this.add(sdPieLabel, gbc);

			gbc.gridx = 1;

			// sdUsed.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().PLAIN_32]);
			sdUsed.setForeground(Color.WHITE);
			this.add(sdUsed, gbc);

			gbc.gridx = 0;
			gbc.gridy = 4;
			// resetLabel.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().PLAIN_32]);
			resetLabel.setForeground(Color.WHITE);
			this.add(resetLabel, gbc);

			gbc.gridx = 1;

			// reset.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().PLAIN_32]);
			reset.setForeground(Color.WHITE);
			this.add(reset, gbc);

			gbc.gridx = 0;
			gbc.gridy = 5;
			// payloadLabel.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().PLAIN_32]);
			payloadLabel.setForeground(Color.WHITE);
			this.add(payloadLabel, gbc);

			gbc.gridx = 1;

			// payload.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().PLAIN_32]);
			payload.setForeground(Color.WHITE);
			this.add(payload, gbc);

		}

		public void updateData(CougSat1Telemetry data) {

			String buff = "";
			mode.setText(data.getMode());
			time.setText(data.getTime() + "");
			if (data.getIHUSdCard() > 1000000000) {
				buff = (data.getIHUSdCard() / 1000000000) + " GB";
			} else if (data.getIHUSdCard() > 1000000) {
				buff = (data.getIHUSdCard() / 1000000) + " MB";
			} else if (data.getIHUSdCard() > 1000) {
				buff = (data.getIHUSdCard() / 1000) + " kB";
			} else {
				buff = (data.getIHUSdCard()) + " B";
			}
			sdUsed.setText(buff);
			reset.setText(data.getResetCount() + "");
			payload.setText(data.getPayloadFrames() + "");

		}
	}

	private class PowerHealth extends JPanel {

		JLabel power = new JLabel("Power", SwingConstants.CENTER);
		private JProgressBar pInBar = new JProgressBar(SwingConstants.HORIZONTAL, 0, 100);
		private JProgressBar pOutBar = new JProgressBar(SwingConstants.HORIZONTAL, 0, 100);
		private JProgressBar batt0VBar = new JProgressBar(SwingConstants.HORIZONTAL, 0, 100);
		private JProgressBar batt0CBar = new JProgressBar(SwingConstants.HORIZONTAL, 0, 100);
		private JProgressBar batt1VBar = new JProgressBar(SwingConstants.HORIZONTAL, 0, 100);
		private JProgressBar batt1CBar = new JProgressBar(SwingConstants.HORIZONTAL, 0, 100);
		private JProgressBar batt0HeatBar = new JProgressBar(SwingConstants.HORIZONTAL, 0, 100);
		private JProgressBar batt1HeatBar = new JProgressBar(SwingConstants.HORIZONTAL, 0, 100);
		private JProgressBar sp0VBar = new JProgressBar(SwingConstants.HORIZONTAL, 0, 100);
		private JProgressBar sp0CBar = new JProgressBar(SwingConstants.HORIZONTAL, 0, 100);
		private JProgressBar sp1VBar = new JProgressBar(SwingConstants.HORIZONTAL, 0, 100);
		private JProgressBar sp1CBar = new JProgressBar(SwingConstants.HORIZONTAL, 0, 100);
		private JProgressBar sp2VBar = new JProgressBar(SwingConstants.HORIZONTAL, 0, 100);
		private JProgressBar sp2CBar = new JProgressBar(SwingConstants.HORIZONTAL, 0, 100);
		private JProgressBar sp3VBar = new JProgressBar(SwingConstants.HORIZONTAL, 0, 100);
		private JProgressBar sp3CBar = new JProgressBar(SwingConstants.HORIZONTAL, 0, 100);
		private JProgressBar v3v3Bar = new JProgressBar(SwingConstants.HORIZONTAL, 0, 100);
		private JProgressBar c3v3Bar = new JProgressBar(SwingConstants.HORIZONTAL, 0, 100);
		private JProgressBar v5v0Bar = new JProgressBar(SwingConstants.HORIZONTAL, 0, 100);
		private JProgressBar c5v0Bar = new JProgressBar(SwingConstants.HORIZONTAL, 0, 100);

		private JLabel pInLabel = new JLabel("In: 12W", SwingConstants.CENTER);
		private JLabel pOutLabel = new JLabel("Out: 12W", SwingConstants.CENTER);
		private JLabel battery0 = new JLabel("Battery 0", SwingConstants.CENTER);
		private JLabel battery0V = new JLabel("12V", SwingConstants.CENTER);
		private JLabel battery0C = new JLabel("12A", SwingConstants.CENTER);
		private JLabel battery1 = new JLabel("Battery 1", SwingConstants.CENTER);
		private JLabel battery1V = new JLabel("12V", SwingConstants.CENTER);
		private JLabel battery1C = new JLabel("12A", SwingConstants.CENTER);
		private JLabel battHeat = new JLabel("Battery Heaters", SwingConstants.CENTER);
		private JLabel batt0Heat = new JLabel("12W", SwingConstants.CENTER);
		private JLabel batt1Heat = new JLabel("12W", SwingConstants.CENTER);
		private JLabel sp0 = new JLabel("Solar Panel 0", SwingConstants.CENTER);
		private JLabel sp1 = new JLabel("Solar Panel 1", SwingConstants.CENTER);
		private JLabel sp2 = new JLabel("Solar Panel 2", SwingConstants.CENTER);
		private JLabel sp3 = new JLabel("Solar Panel 3", SwingConstants.CENTER);
		private JLabel sp0V = new JLabel("12V", SwingConstants.CENTER);
		private JLabel sp0C = new JLabel("12A", SwingConstants.CENTER);
		private JLabel sp1V = new JLabel("12V", SwingConstants.CENTER);
		private JLabel sp1C = new JLabel("12A", SwingConstants.CENTER);
		private JLabel sp2V = new JLabel("12V", SwingConstants.CENTER);
		private JLabel sp2C = new JLabel("12A", SwingConstants.CENTER);
		private JLabel sp3V = new JLabel("12V", SwingConstants.CENTER);
		private JLabel sp3C = new JLabel("12A", SwingConstants.CENTER);
		private JLabel label3v3 = new JLabel("3.3V Rail", SwingConstants.CENTER);
		private JLabel v3v3 = new JLabel("12V", SwingConstants.CENTER);
		private JLabel c3v3 = new JLabel("12A", SwingConstants.CENTER);
		private JLabel label5v0 = new JLabel("5.0V Rail", SwingConstants.CENTER);
		private JLabel v5v0 = new JLabel("12V", SwingConstants.CENTER);
		private JLabel c5v0 = new JLabel("12A", SwingConstants.CENTER);
		private JLabel channels = new JLabel("Channels", SwingConstants.CENTER);

		private JLabel ch0 = new JLabel("0", SwingConstants.CENTER);
		private JLabel ch1 = new JLabel("1", SwingConstants.CENTER);
		private JLabel ch2 = new JLabel("2", SwingConstants.CENTER);
		private JLabel ch3 = new JLabel("3", SwingConstants.CENTER);
		private JLabel ch4 = new JLabel("4", SwingConstants.CENTER);
		private JLabel ch5 = new JLabel("5", SwingConstants.CENTER);
		private JLabel ch6 = new JLabel("6", SwingConstants.CENTER);
		private JLabel ch7 = new JLabel("7", SwingConstants.CENTER);
		private JLabel ch8 = new JLabel("8", SwingConstants.CENTER);
		private JLabel ch9 = new JLabel("9", SwingConstants.CENTER);
		private JLabel ch10 = new JLabel("10", SwingConstants.CENTER);
		private JLabel ch11 = new JLabel("11", SwingConstants.CENTER);
		private JLabel ch12 = new JLabel("12", SwingConstants.CENTER);
		private JLabel ch13 = new JLabel("13", SwingConstants.CENTER);
		private JLabel ch14 = new JLabel("14", SwingConstants.CENTER);
		private JLabel ch15 = new JLabel("15", SwingConstants.CENTER);
		private JLabel ch16 = new JLabel("16", SwingConstants.CENTER);
		private JLabel ch17 = new JLabel("17", SwingConstants.CENTER);
		private JLabel ch18 = new JLabel("18", SwingConstants.CENTER);
		private JLabel ch19 = new JLabel("19", SwingConstants.CENTER);

		private JPanel channelWrapper = new JPanel();

		ArrayList<JLabel> labels = new ArrayList<JLabel>();
		ArrayList<JLabel> channelLabels = new ArrayList<JLabel>();
		ArrayList<JProgressBar> bars = new ArrayList<JProgressBar>();

		public PowerHealth() {

			bars.add(pInBar);
			bars.add(pOutBar);
			bars.add(batt0VBar);
			bars.add(batt0CBar);
			bars.add(batt1VBar);
			bars.add(batt1CBar);
			bars.add(batt0HeatBar);
			bars.add(batt1HeatBar);
			bars.add(sp0VBar);
			bars.add(sp0CBar);
			bars.add(sp1VBar);
			bars.add(sp1CBar);
			bars.add(sp2VBar);
			bars.add(sp2CBar);
			bars.add(sp3VBar);
			bars.add(sp3CBar);
			bars.add(v3v3Bar);
			bars.add(c3v3Bar);
			bars.add(v5v0Bar);
			bars.add(c5v0Bar);

			for (JProgressBar bar : bars) {

				bar.setBackground(CustomColors.BACKGROUND1);
				bar.setForeground(CustomColors.BACKGROUND3);

			}

			labels.add(power);
			labels.add(pInLabel);
			labels.add(pOutLabel);
			labels.add(battery0);
			labels.add(battery0V);
			labels.add(battery0C);
			labels.add(battery1);
			labels.add(battery1V);
			labels.add(battery1C);
			labels.add(battHeat);
			labels.add(batt0Heat);
			labels.add(batt1Heat);
			labels.add(sp0);
			labels.add(sp0V);
			labels.add(sp0C);
			labels.add(sp1);
			labels.add(sp1V);
			labels.add(sp1C);
			labels.add(sp2);
			labels.add(sp2V);
			labels.add(sp2C);
			labels.add(sp3);
			labels.add(sp3V);
			labels.add(sp3C);
			labels.add(v3v3);
			labels.add(c3v3);
			labels.add(v5v0);
			labels.add(c5v0);
			labels.add(label3v3);
			labels.add(label5v0);
			labels.add(channels);

			for (JLabel label : labels) {

				label.setForeground(Color.WHITE);
				// label.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().PLAIN_32]);
			}

			channelLabels.add(ch0);
			channelLabels.add(ch1);
			channelLabels.add(ch2);
			channelLabels.add(ch3);
			channelLabels.add(ch4);
			channelLabels.add(ch5);
			channelLabels.add(ch6);
			channelLabels.add(ch7);
			channelLabels.add(ch8);
			channelLabels.add(ch9);
			channelLabels.add(ch10);
			channelLabels.add(ch11);
			channelLabels.add(ch12);
			channelLabels.add(ch13);
			channelLabels.add(ch14);
			channelLabels.add(ch15);
			channelLabels.add(ch16);
			channelLabels.add(ch17);
			channelLabels.add(ch18);
			channelLabels.add(ch19);

			for (JLabel label : channelLabels) {

				label.setForeground(Color.WHITE);
				label.setOpaque(true);
				label.setBackground(CustomColors.BACKGROUND3);
				// label.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().PLAIN_32]);
			}

			this.setLayout(new GridBagLayout());
			GridBagConstraints gbc = new GridBagConstraints();
			gbc.insets = new Insets(5, 5, 5, 5);
			gbc.fill = GridBagConstraints.BOTH;
			gbc.gridx = 0;
			gbc.gridy = 0;
			gbc.weightx = 1.0;
			gbc.weighty = 1.0;
			gbc.gridheight = 1;
			gbc.gridwidth = 2;
			this.add(power, gbc);// power label at top of screen

			gbc.gridx = 0;
			gbc.gridy = 1;
			gbc.gridwidth = 1;
			gbc.weightx = 0.1;
			this.add(pInLabel, gbc);// power in

			gbc.gridx = 1;
			gbc.weightx = 0.9;
			this.add(pInBar, gbc);

			gbc.gridy = 2;
			gbc.gridx = 0;
			gbc.weightx = 0.1;
			this.add(pOutLabel, gbc);// power out

			gbc.gridx = 1;
			gbc.weightx = 0.9;
			this.add(pOutBar, gbc);

			gbc.gridwidth = 2;// battery 0
			gbc.gridx = 0;
			gbc.gridy = 3;
			gbc.weightx = 1.0;
			this.add(battery0, gbc);

			gbc.gridwidth = 1;
			gbc.gridx = 0;
			gbc.gridy = 4;
			gbc.weightx = 0.1;
			this.add(battery0V, gbc);

			gbc.gridx = 1;
			gbc.weightx = 0.9;
			this.add(batt0VBar, gbc);

			gbc.gridx = 0;
			gbc.gridy = 5;
			gbc.weightx = 0.1;
			this.add(battery0C, gbc);

			gbc.gridx = 1;
			gbc.weightx = 0.9;
			this.add(batt0CBar, gbc);

			gbc.gridwidth = 2;// battery 1
			gbc.gridx = 0;
			gbc.gridy = 6;
			gbc.weightx = 1.0;
			this.add(battery1, gbc);

			gbc.gridwidth = 1;
			gbc.gridx = 0;
			gbc.gridy = 7;
			gbc.weightx = 0.1;
			this.add(battery1V, gbc);

			gbc.gridx = 1;
			gbc.weightx = 0.9;
			this.add(batt1VBar, gbc);

			gbc.gridx = 0;
			gbc.gridy = 8;
			gbc.weightx = 0.1;
			this.add(battery1C, gbc);

			gbc.gridx = 1;
			gbc.weightx = 0.9;
			this.add(batt1CBar, gbc);

			gbc.gridwidth = 2;// battery Heat
			gbc.gridx = 0;
			gbc.gridy = 9;
			gbc.weightx = 1.0;
			this.add(battHeat, gbc);

			gbc.gridwidth = 1;
			gbc.gridx = 0;
			gbc.gridy = 10;
			gbc.weightx = 0.1;
			this.add(batt0Heat, gbc);

			gbc.gridx = 1;
			gbc.weightx = 0.9;
			this.add(batt0HeatBar, gbc);

			gbc.gridx = 0;
			gbc.gridy = 11;
			gbc.weightx = 0.1;
			this.add(batt1Heat, gbc);

			gbc.gridx = 1;
			gbc.weightx = 0.9;
			this.add(batt1HeatBar, gbc);

			gbc.gridwidth = 2;// solar panel 0 input
			gbc.gridx = 0;
			gbc.gridy = 12;
			gbc.weightx = 1.0;
			this.add(sp0, gbc);

			gbc.gridwidth = 1;
			gbc.gridx = 0;
			gbc.gridy = 13;
			gbc.weightx = 0.1;
			this.add(sp0V, gbc);

			gbc.gridx = 1;
			gbc.weightx = 0.9;
			this.add(sp0VBar, gbc);

			gbc.gridx = 0;
			gbc.gridy = 14;
			gbc.weightx = 0.1;
			this.add(sp0C, gbc);

			gbc.gridx = 1;
			gbc.weightx = 0.9;
			this.add(sp0CBar, gbc);

			gbc.gridwidth = 2;// solar panel 1 input
			gbc.gridx = 0;
			gbc.gridy = 15;
			gbc.weightx = 1.0;
			this.add(sp1, gbc);

			gbc.gridwidth = 1;
			gbc.gridx = 0;
			gbc.gridy = 16;
			gbc.weightx = 0.1;
			this.add(sp1V, gbc);

			gbc.gridx = 1;
			gbc.weightx = 0.9;
			this.add(sp1VBar, gbc);

			gbc.gridx = 0;
			gbc.gridy = 17;
			gbc.weightx = 0.1;
			this.add(sp1C, gbc);

			gbc.gridx = 1;
			gbc.weightx = 0.9;
			this.add(sp1CBar, gbc);

			gbc.gridwidth = 2;// solar panel 2 input
			gbc.gridx = 0;
			gbc.gridy = 18;
			gbc.weightx = 1.0;
			this.add(sp2, gbc);

			gbc.gridwidth = 1;
			gbc.gridx = 0;
			gbc.gridy = 19;
			gbc.weightx = 0.1;
			this.add(sp2V, gbc);

			gbc.gridx = 1;
			gbc.weightx = 0.9;
			this.add(sp2VBar, gbc);

			gbc.gridx = 0;
			gbc.gridy = 20;
			gbc.weightx = 0.1;
			this.add(sp2C, gbc);

			gbc.gridx = 1;
			gbc.weightx = 0.9;
			this.add(sp2CBar, gbc);

			gbc.gridwidth = 2;// solar panel 3 input
			gbc.gridx = 0;
			gbc.gridy = 21;
			gbc.weightx = 1.0;
			this.add(sp3, gbc);

			gbc.gridwidth = 1;
			gbc.gridx = 0;
			gbc.gridy = 22;
			gbc.weightx = 0.1;
			this.add(sp3V, gbc);

			gbc.gridx = 1;
			gbc.weightx = 0.9;
			this.add(sp3VBar, gbc);

			gbc.gridx = 0;
			gbc.gridy = 23;
			gbc.weightx = 0.1;
			this.add(sp3C, gbc);

			gbc.gridx = 1;
			gbc.weightx = 0.9;
			this.add(sp3CBar, gbc);

			gbc.gridwidth = 2;// 3.3V rail input
			gbc.gridx = 0;
			gbc.gridy = 24;
			gbc.weightx = 1.0;
			this.add(label3v3, gbc);

			gbc.gridwidth = 1;
			gbc.gridx = 0;
			gbc.gridy = 25;
			gbc.weightx = 0.1;
			this.add(v3v3, gbc);

			gbc.gridx = 1;
			gbc.weightx = 0.9;
			this.add(v3v3Bar, gbc);

			gbc.gridx = 0;
			gbc.gridy = 26;
			gbc.weightx = 0.1;
			this.add(c3v3, gbc);

			gbc.gridx = 1;
			gbc.weightx = 0.9;
			this.add(c3v3Bar, gbc);

			gbc.gridwidth = 2;// 5.0V rail input
			gbc.gridx = 0;
			gbc.gridy = 27;
			gbc.weightx = 1.0;
			this.add(label5v0, gbc);

			gbc.gridwidth = 1;
			gbc.gridx = 0;
			gbc.gridy = 28;
			gbc.weightx = 0.1;
			this.add(v5v0, gbc);

			gbc.gridx = 1;
			gbc.weightx = 0.9;
			this.add(v5v0Bar, gbc);

			gbc.gridx = 0;
			gbc.gridy = 29;
			gbc.weightx = 0.1;
			this.add(c5v0, gbc);

			gbc.gridx = 1;
			gbc.weightx = 0.9;
			this.add(c5v0Bar, gbc);

			gbc.gridwidth = 2;// channels
			gbc.gridx = 0;
			gbc.gridy = 30;
			gbc.weightx = 1.0;
			this.add(channels, gbc);

			gbc.gridwidth = 2;
			// --------------------

			GridLayout gl = new GridLayout(2, 7, 5, 5);

			channelWrapper.setLayout(gl);
			channelWrapper.setBackground(CustomColors.BACKGROUND2);

			for (JLabel ch : channelLabels) {
				channelWrapper.add(ch);
			}

			gbc.gridy = 31;
			this.add(channelWrapper, gbc);

		}

		public void updateData(CougSat1Telemetry data) {

			int channels = data.getEPSChannels();

			for (int i = 0; i < channelLabels.size(); i++) {

				if ((channels & (1 << i)) != 0) {

					channelLabels.get(i).setBackground(Color.GREEN);
					channelLabels.get(i).setForeground(Color.BLACK);

				} else {

					channelLabels.get(i).setBackground(CustomColors.BACKGROUND3);
					channelLabels.get(i).setForeground(Color.WHITE);

				}
			}

			pInLabel.setText(String.format("In: %.3f W", data.getPowerIn()));
			pOutLabel.setText(String.format("Out: %.3f W", data.getPowerOut()));
			battery0V.setText(String.format("%.3f V", data.getBattery0Voltage()));
			battery0C.setText(String.format("%.3f A", data.getBattery0Current()));
			battery1V.setText(String.format("%.3f V", data.getBattery1Voltage()));
			battery1C.setText(String.format("%.3f A", data.getBattery1Current()));

			if (data.getBattery0Heat() < 1000) {
				batt0Heat.setText(String.format("%.1f mW", data.getBattery0Heat()));
			} else {
				batt0Heat.setText(String.format("%1.3f W", data.getBattery0Heat() * 0.001));
			}
			if (data.getBattery0Heat() < 1000) {
				batt1Heat.setText(String.format("%.1f mW", data.getBattery1Heat()));
			} else {
				batt1Heat.setText(String.format("%1.3f W", data.getBattery1Heat() * 0.001));
			}

			sp0V.setText(String.format("%.3f V", data.getSP0Voltage()));
			sp0C.setText(String.format("%.3f A", data.getSP0Current()));
			sp1V.setText(String.format("%.3f V", data.getSP1Voltage()));
			sp1C.setText(String.format("%.3f A", data.getSP1Current()));
			sp2V.setText(String.format("%.3f V", data.getSP2Voltage()));
			sp2C.setText(String.format("%.3f A", data.getSP2Current()));
			sp3V.setText(String.format("%.3f V", data.getSP3Voltage()));
			sp3C.setText(String.format("%.3f A", data.getSP3Current()));

			v3v3.setText(String.format("%.3f V", data.get3v3RailVoltage()));
			c3v3.setText(String.format("%.3f A", data.get3v3RailCurrent()));

			v5v0.setText(String.format("%.3f V", data.get5vRailVoltage()));
			c5v0.setText(String.format("%.3f A", data.get5vRailCurrent()));

			pInBar.setValue((int) (data.getPowerIn() / 10.0 * 100));
			pOutBar.setValue((int) (data.getPowerOut() / 20.0 * 100));

			batt0VBar.setValue((int) (data.getBattery0Voltage() / 4.2 * 100));
			batt0CBar.setValue((int) (data.getBattery0Current() / 3.0 * 100));

			batt1VBar.setValue((int) (data.getBattery1Voltage() / 4.2 * 100));
			batt1CBar.setValue((int) (data.getBattery1Current() / 3.0 * 100));

			batt0HeatBar.setValue((int) (data.getBattery0Heat() / 1500.0 * 100));
			batt1HeatBar.setValue((int) (data.getBattery1Heat() / 1500.0 * 100));

			sp0VBar.setValue((int) (data.getSP0Voltage() / 5.5 * 100));
			sp0CBar.setValue((int) (data.getSP0Current() / 0.6 * 100));
			sp1VBar.setValue((int) (data.getSP1Voltage() / 5.5 * 100));
			sp1CBar.setValue((int) (data.getSP1Current() / 0.6 * 100));
			sp2VBar.setValue((int) (data.getSP2Voltage() / 5.5 * 100));
			sp2CBar.setValue((int) (data.getSP2Current() / 0.6 * 100));
			sp3VBar.setValue((int) (data.getSP3Voltage() / 5.5 * 100));
			sp3CBar.setValue((int) (data.getSP3Current() / 0.6 * 100));
			v3v3Bar.setValue((int) (data.get3v3RailVoltage() / 3.5 * 100));
			c3v3Bar.setValue((int) (data.get3v3RailCurrent() / 4.0 * 100));

			v5v0Bar.setValue((int) (data.get5vRailVoltage() / 5.2 * 100));
			c5v0Bar.setValue((int) (data.get5vRailCurrent() / 2.0 * 100));

		}

	}

	private class TempHealth extends JPanel {

		private JProgressBar IHUTemp = new JProgressBar(SwingConstants.VERTICAL, 0, 100);
		private JProgressBar ADCSTemp = new JProgressBar(SwingConstants.VERTICAL, 0, 100);
		private JProgressBar IFJRTemp = new JProgressBar(SwingConstants.VERTICAL, 0, 100);
		private JProgressBar PMICTemp = new JProgressBar(SwingConstants.VERTICAL, 0, 100);
		private JProgressBar battery0Temp = new JProgressBar(SwingConstants.VERTICAL, 0, 100);
		private JProgressBar battery1Temp = new JProgressBar(SwingConstants.VERTICAL, 0, 100);
		private JProgressBar RCSTemp = new JProgressBar(SwingConstants.VERTICAL, 0, 100);

		JLabel temp = new JLabel("Temp", SwingConstants.CENTER);
		JLabel IHUTempLabel = new JLabel(" IHU  ", SwingConstants.CENTER);
		JLabel ADCSTempLabel = new JLabel(" ADCS ", SwingConstants.CENTER);
		JLabel IFJRTempLabel = new JLabel(" IFJR ", SwingConstants.CENTER);
		JLabel PMICTempLabel = new JLabel(" PMIC ", SwingConstants.CENTER);
		JLabel battery0TempLabel = new JLabel("Batt 0", SwingConstants.CENTER);
		JLabel battery1TempLabel = new JLabel("Batt 1", SwingConstants.CENTER);
		JLabel RCSTempLabel = new JLabel(" RCS  ", SwingConstants.CENTER);

		JLabel IHUValue = new JLabel("", SwingConstants.CENTER);
		JLabel ADCSValue = new JLabel("", SwingConstants.CENTER);
		JLabel IFJRValue = new JLabel("", SwingConstants.CENTER);
		JLabel PMICValue = new JLabel("", SwingConstants.CENTER);
		JLabel Battery0Value = new JLabel("", SwingConstants.CENTER);
		JLabel Battery1Value = new JLabel("", SwingConstants.CENTER);
		JLabel RCSValue = new JLabel("", SwingConstants.CENTER);

		ArrayList<JLabel> labels = new ArrayList<JLabel>();
		ArrayList<JProgressBar> bars = new ArrayList<JProgressBar>();

		public TempHealth() {

			labels.add(temp);
			labels.add(IHUTempLabel);
			labels.add(ADCSTempLabel);
			labels.add(IFJRTempLabel);
			labels.add(PMICTempLabel);
			labels.add(battery0TempLabel);
			labels.add(battery1TempLabel);
			labels.add(RCSTempLabel);

			labels.add(IHUValue);
			labels.add(ADCSValue);
			labels.add(IFJRValue);
			labels.add(PMICValue);
			labels.add(Battery0Value);
			labels.add(Battery1Value);
			labels.add(RCSValue);

			for (JLabel label : labels) {

				// label.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().PLAIN_32]);
				label.setForeground(Color.WHITE);

			}

			bars.add(IHUTemp);
			bars.add(ADCSTemp);
			bars.add(IFJRTemp);
			bars.add(PMICTemp);
			bars.add(battery0Temp);
			bars.add(battery1Temp);
			bars.add(RCSTemp);

			for (JProgressBar bar : bars) {

				bar.setBackground(CustomColors.BACKGROUND1);
				bar.setForeground(CustomColors.BACKGROUND3);

			}

			this.setLayout(new GridBagLayout());
			GridBagConstraints gbc = new GridBagConstraints();
			gbc.insets = new Insets(5, 5, 5, 5);
			gbc.fill = GridBagConstraints.VERTICAL;
			gbc.gridx = 0;
			gbc.gridy = 0;
			gbc.weightx = 1.0;
			gbc.weighty = 1.0;
			gbc.gridheight = 1;
			gbc.gridwidth = 21;
			this.add(temp, gbc);

			gbc.gridwidth = 3;
			gbc.gridy = 1;
			gbc.gridx = 0;
			this.add(IHUValue, gbc);

			gbc.gridy = 3;
			this.add(IHUTempLabel, gbc);

			gbc.gridx = 3;
			gbc.gridy = 1;
			this.add(ADCSValue, gbc);

			gbc.gridy = 3;
			this.add(ADCSTempLabel, gbc);

			gbc.gridx = 6;
			gbc.gridy = 1;
			this.add(IFJRValue, gbc);

			gbc.gridy = 3;
			this.add(IFJRTempLabel, gbc);

			gbc.gridx = 9;
			gbc.gridy = 1;
			this.add(RCSValue, gbc);

			gbc.gridy = 3;
			this.add(RCSTempLabel, gbc);

			gbc.gridx = 12;
			gbc.gridy = 1;
			this.add(PMICValue, gbc);

			gbc.gridy = 3;
			this.add(PMICTempLabel, gbc);

			gbc.gridx = 15;
			gbc.gridy = 1;
			this.add(Battery0Value, gbc);

			gbc.gridy = 3;
			this.add(battery0TempLabel, gbc);

			gbc.gridx = 18;
			gbc.gridy = 1;
			this.add(Battery1Value, gbc);

			gbc.gridy = 3;
			this.add(battery1TempLabel, gbc);

			gbc.fill = GridBagConstraints.BOTH;
			gbc.insets = new Insets(0, 50, 0, 50);
			gbc.gridy = 2;
			gbc.gridx = 1;
			gbc.gridwidth = 1;
			this.add(IHUTemp, gbc);

			gbc.gridx = 4;
			this.add(ADCSTemp, gbc);

			gbc.gridx = 7;
			this.add(IFJRTemp, gbc);

			gbc.gridx = 10;
			this.add(RCSTemp, gbc);

			gbc.gridx = 13;
			this.add(PMICTemp, gbc);

			gbc.gridx = 16;
			this.add(battery0Temp, gbc);

			gbc.gridx = 19;
			this.add(battery1Temp, gbc);

		}

		public void updateData(CougSat1Telemetry data) {

			IHUValue.setText(String.format("%d C", data.getIHUTemp()));
			ADCSValue.setText(String.format("%d C", data.getADCSTemp()));
			IFJRValue.setText(String.format("%d C", data.getIFJRTemp()));
			PMICValue.setText(String.format("%d C", data.getPMICTemp()));
			Battery0Value.setText(String.format("%d C", data.getBattery0Temp()));
			Battery1Value.setText(String.format("%d C", data.getBattery1Temp()));
			RCSValue.setText(String.format("%d C", data.getRCSTemp()));

			IHUTemp.setValue((int) ((data.getIHUTemp() + 80) / 160.0 * 100));// -80
																				// -
																				// 80
			ADCSTemp.setValue((int) ((data.getADCSTemp() + 80) / 160.0 * 100));
			IFJRTemp.setValue((int) ((data.getIFJRTemp() + 80) / 160.0 * 100));
			PMICTemp.setValue((int) ((data.getPMICTemp() + 80) / 160.0 * 100));
			battery0Temp.setValue((int) ((data.getBattery0Temp() + 80) / 160.0 * 100));
			battery1Temp.setValue((int) ((data.getBattery1Temp() + 80) / 160.0 * 100));
			RCSTemp.setValue((int) ((data.getRCSTemp() + 80) / 160.0 * 100));

		}
	}

	private class ADCSHealth extends JPanel {

		ArrayList<JLabel> labels = new ArrayList<JLabel>();
		JLabel adcs = new JLabel("ADCS", SwingConstants.CENTER);

		public ADCSHealth() {

			labels.add(adcs);

			for (JLabel label : labels) {
				// label.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().PLAIN_32]);
				label.setForeground(Color.WHITE);
			}

			this.add(adcs);

		}

		public void updateData(CougSat1Telemetry data) {

		}
	}

	public void updateData(CougSat1Telemetry data) {// updates all other
													// branches

		// rcsHealth.updateData(data);
		// ihuHealth.updateData(data);
		// powerHealth.updateData(data);
		// adcsHealth.updateData(data);
		// tempHealth.updateData(data);

	}
}