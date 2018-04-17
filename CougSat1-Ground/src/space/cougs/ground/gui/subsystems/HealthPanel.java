package space.cougs.ground.gui.subsystems;

import java.awt.Color;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.awt.Insets;
import java.util.ArrayList;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.SwingConstants;

import space.cougs.ground.CougSat1Telemetry;
import space.cougs.ground.utils.CustomColors;
import space.cougs.ground.utils.Fonts;
import space.cougs.ground.utils.RoundButton;

@SuppressWarnings("serial")
public class HealthPanel extends JPanel {

	IHUHealth ihuHealth = new IHUHealth();
	TempHealth tempHealth = new TempHealth();
	ADCSHealth adcsHealth = new ADCSHealth();
	RCSHealth rcsHealth = new RCSHealth();
	PowerHealth powerHealth = new PowerHealth();

	public HealthPanel() {
		super();
		this.setBackground(CustomColors.WSU_GRAY);
		this.setLayout(new GridBagLayout());
		this.setBorder(BorderFactory.createLineBorder(CustomColors.WSU_GRAY, 5));

		ihuHealth.setBackground(CustomColors.NAVY);
		tempHealth.setBackground(CustomColors.NAVY);
		adcsHealth.setBackground(CustomColors.NAVY);
		rcsHealth.setBackground(CustomColors.NAVY);
		powerHealth.setBackground(CustomColors.NAVY);

		GridBagConstraints gbc = new GridBagConstraints();
		gbc.insets = new Insets(5, 5, 5, 5);
		gbc.fill = GridBagConstraints.BOTH;
		gbc.gridx = 0;
		gbc.gridy = 0;
		gbc.weightx = 1.0;
		gbc.weighty = 1.0;
		gbc.gridheight = 1;
		gbc.gridwidth = 1;
		this.add(ihuHealth, gbc);

		gbc.gridy = 1;
		gbc.weighty = 0.6;
		gbc.gridwidth = 2;
		this.add(tempHealth, gbc);

		gbc.gridy = 2;
		gbc.weighty = 1.0;
		this.add(adcsHealth, gbc);

		gbc.gridx = 1;
		gbc.gridy = 0;
		gbc.weightx = 0.6;
		gbc.gridwidth = 1;
		this.add(rcsHealth, gbc);

		gbc.weightx = 1.0;
		gbc.gridx = 2;
		gbc.gridheight = 3;
		this.add(powerHealth, gbc);

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

		public RCSHealth() {
			super();

			commsLabel.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			commsLabel.setForeground(Color.WHITE);

			rxLabel.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			rxLabel.setForeground(Color.WHITE);

			txLabel.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			txLabel.setForeground(Color.WHITE);

			snrLabel.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			snrLabel.setForeground(Color.WHITE);

			rxData.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			rxData.setForeground(Color.WHITE);

			txData.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			txData.setForeground(Color.WHITE);

			snrxData.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			snrxData.setForeground(Color.WHITE);

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
			txBar.setBackground(CustomColors.LIGHT_GRAY);
			txBar.setForeground(CustomColors.CRIMSON);
			this.add(txBar, gbc);

			gbc.gridx = 1;
			rxBar.setBackground(CustomColors.LIGHT_GRAY);
			rxBar.setForeground(CustomColors.CRIMSON);
			this.add(rxBar, gbc);

			gbc.gridx = 2;
			snrBar.setBackground(CustomColors.LIGHT_GRAY);
			snrBar.setForeground(CustomColors.CRIMSON);
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
			computer.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			computer.setForeground(Color.WHITE);
			this.add(computer, gbc);

			gbc.gridwidth = 1;
			gbc.gridx = 0;
			gbc.gridy = 1;
			modeLabel.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			modeLabel.setForeground(Color.WHITE);
			mode.setBackground(CustomColors.WSU_GRAY);
			mode.setOpaque(true);
			this.add(modeLabel, gbc);

			gbc.gridx = 1;
			mode.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			mode.setForeground(Color.WHITE);
			this.add(mode, gbc);

			gbc.gridx = 0;
			gbc.gridy = 2;
			timeLabel.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			timeLabel.setForeground(Color.WHITE);
			this.add(timeLabel, gbc);

			gbc.gridx = 1;
			time.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			time.setForeground(Color.WHITE);
			this.add(time, gbc);

			gbc.gridx = 0;
			gbc.gridy = 3;
			sdPieLabel.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			sdPieLabel.setForeground(Color.WHITE);
			this.add(sdPieLabel, gbc);

			gbc.gridx = 1;
			sdUsed.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			sdUsed.setForeground(Color.WHITE);
			this.add(sdUsed, gbc);

			gbc.gridx = 0;
			gbc.gridy = 4;
			resetLabel.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			resetLabel.setForeground(Color.WHITE);
			this.add(resetLabel, gbc);

			gbc.gridx = 1;
			reset.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			reset.setForeground(Color.WHITE);
			this.add(reset, gbc);

			gbc.gridx = 0;
			gbc.gridy = 5;
			payloadLabel.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			payloadLabel.setForeground(Color.WHITE);
			this.add(payloadLabel, gbc);

			gbc.gridx = 1;
			payload.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
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

	private class TempHealth extends JPanel {

		JLabel temp = new JLabel("Temp", SwingConstants.CENTER);

		public TempHealth() {

			temp.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			temp.setForeground(Color.WHITE);
			this.add(temp);

		}

		public void updateData(CougSat1Telemetry data) {

		}
	}

	private class ADCSHealth extends JPanel {

		JLabel adcs = new JLabel("Adcs", SwingConstants.CENTER);

		public ADCSHealth() {

			adcs.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			adcs.setForeground(Color.WHITE);
			this.add(adcs);

		}

		public void updateData(CougSat1Telemetry data) {

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

		private JPanel channelWrapper = new JPanel();

		ArrayList<JLabel> labels = new ArrayList<JLabel>();
		ArrayList<JLabel> channelLabels = new ArrayList<JLabel>();

		public PowerHealth() {

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
				label.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
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

			for (JLabel label : channelLabels) {

				label.setForeground(Color.WHITE);
				label.setOpaque(true);
				label.setBackground(CustomColors.CRIMSON);
				label.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			}

			// for (JButton button : buttons) {
			//
			// button.setForeground(Color.WHITE);
			// button.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			// }

			this.setLayout(new GridBagLayout());
			GridBagConstraints gbc = new GridBagConstraints();
			gbc.insets = new Insets(5, 5, 5, 5);
			gbc.fill = GridBagConstraints.HORIZONTAL;
			gbc.gridx = 0;
			gbc.gridy = 0;
			gbc.weightx = 1.0;
			gbc.weighty = 1.0;
			gbc.gridheight = 1;
			gbc.gridwidth = 8;
			this.add(power, gbc);// power label at top of screen

			gbc.gridx = 0;
			gbc.gridy = 1;
			gbc.gridwidth = 2;
			this.add(pInLabel, gbc);// power in

			gbc.gridwidth = 6;
			gbc.gridx = 2;
			this.add(pInBar, gbc);

			gbc.gridy = 2;
			gbc.gridx = 0;
			gbc.gridwidth = 2;
			this.add(pOutLabel, gbc);// power out

			gbc.gridwidth = 6;
			gbc.gridx = 2;
			this.add(pOutBar, gbc);

			gbc.gridwidth = 8;// battery 0
			gbc.gridx = 0;
			gbc.gridy = 3;
			this.add(battery0, gbc);

			gbc.gridwidth = 2;
			gbc.gridx = 0;
			gbc.gridy = 4;
			this.add(battery0V, gbc);

			gbc.gridwidth = 6;
			gbc.gridx = 2;
			this.add(batt0VBar, gbc);

			gbc.gridwidth = 2;
			gbc.gridx = 0;
			gbc.gridy = 5;
			this.add(battery0C, gbc);

			gbc.gridwidth = 6;
			gbc.gridx = 2;
			this.add(batt0CBar, gbc);

			gbc.gridwidth = 8;// battery 1
			gbc.gridx = 0;
			gbc.gridy = 6;
			this.add(battery1, gbc);

			gbc.gridwidth = 2;
			gbc.gridx = 0;
			gbc.gridy = 7;
			this.add(battery1V, gbc);

			gbc.gridwidth = 6;
			gbc.gridx = 2;
			this.add(batt1VBar, gbc);

			gbc.gridwidth = 2;
			gbc.gridx = 0;
			gbc.gridy = 8;
			this.add(battery1C, gbc);

			gbc.gridwidth = 6;
			gbc.gridx = 2;
			this.add(batt1CBar, gbc);

			gbc.gridwidth = 8;// battery Heat
			gbc.gridx = 0;
			gbc.gridy = 9;
			this.add(battHeat, gbc);

			gbc.gridwidth = 2;
			gbc.gridx = 0;
			gbc.gridy = 10;
			this.add(batt0Heat, gbc);

			gbc.gridwidth = 6;
			gbc.gridx = 2;
			this.add(batt0HeatBar, gbc);

			gbc.gridwidth = 2;
			gbc.gridx = 0;
			gbc.gridy = 11;
			this.add(batt1Heat, gbc);

			gbc.gridwidth = 6;
			gbc.gridx = 2;
			this.add(batt1HeatBar, gbc);

			gbc.gridwidth = 8;// solar panel 0 input
			gbc.gridx = 0;
			gbc.gridy = 12;
			this.add(sp0, gbc);

			gbc.gridwidth = 2;
			gbc.gridx = 0;
			gbc.gridy = 13;
			this.add(sp0V, gbc);

			gbc.gridwidth = 6;
			gbc.gridx = 2;
			this.add(sp0VBar, gbc);

			gbc.gridwidth = 2;
			gbc.gridx = 0;
			gbc.gridy = 14;
			this.add(sp0C, gbc);

			gbc.gridwidth = 6;
			gbc.gridx = 2;
			this.add(sp0CBar, gbc);

			gbc.gridwidth = 8;// solar panel 1 input
			gbc.gridx = 0;
			gbc.gridy = 15;
			this.add(sp1, gbc);

			gbc.gridwidth = 2;
			gbc.gridx = 0;
			gbc.gridy = 16;
			this.add(sp1V, gbc);

			gbc.gridwidth = 6;
			gbc.gridx = 2;
			this.add(sp1VBar, gbc);

			gbc.gridwidth = 2;
			gbc.gridx = 0;
			gbc.gridy = 17;
			this.add(sp1C, gbc);

			gbc.gridwidth = 6;
			gbc.gridx = 2;
			this.add(sp1CBar, gbc);

			gbc.gridwidth = 8;// solar panel 2 input
			gbc.gridx = 0;
			gbc.gridy = 18;
			this.add(sp2, gbc);

			gbc.gridwidth = 2;
			gbc.gridx = 0;
			gbc.gridy = 19;
			this.add(sp2V, gbc);

			gbc.gridwidth = 6;
			gbc.gridx = 2;
			this.add(sp2VBar, gbc);

			gbc.gridwidth = 2;
			gbc.gridx = 0;
			gbc.gridy = 20;
			this.add(sp2C, gbc);

			gbc.gridwidth = 6;
			gbc.gridx = 2;
			this.add(sp2CBar, gbc);

			gbc.gridwidth = 8;// solar panel 3 input
			gbc.gridx = 0;
			gbc.gridy = 21;
			this.add(sp3, gbc);

			gbc.gridwidth = 2;
			gbc.gridx = 0;
			gbc.gridy = 22;
			this.add(sp3V, gbc);

			gbc.gridwidth = 6;
			gbc.gridx = 2;
			this.add(sp3VBar, gbc);

			gbc.gridwidth = 2;
			gbc.gridx = 0;
			gbc.gridy = 23;
			this.add(sp3C, gbc);

			gbc.gridwidth = 6;
			gbc.gridx = 2;
			this.add(sp3CBar, gbc);

			gbc.gridwidth = 8;// 3.3V rail input
			gbc.gridx = 0;
			gbc.gridy = 24;
			this.add(label3v3, gbc);

			gbc.gridwidth = 2;
			gbc.gridx = 0;
			gbc.gridy = 25;
			this.add(v3v3, gbc);

			gbc.gridwidth = 6;
			gbc.gridx = 2;
			this.add(v3v3Bar, gbc);

			gbc.gridwidth = 2;
			gbc.gridx = 0;
			gbc.gridy = 26;
			this.add(c3v3, gbc);

			gbc.gridwidth = 6;
			gbc.gridx = 2;
			this.add(c3v3Bar, gbc);

			gbc.gridwidth = 8;// 5.0V rail input
			gbc.gridx = 0;
			gbc.gridy = 27;
			this.add(label5v0, gbc);

			gbc.gridwidth = 2;
			gbc.gridx = 0;
			gbc.gridy = 28;
			this.add(v5v0, gbc);

			gbc.gridwidth = 6;
			gbc.gridx = 2;
			this.add(v5v0Bar, gbc);

			gbc.gridwidth = 2;
			gbc.gridx = 0;
			gbc.gridy = 29;
			this.add(c5v0, gbc);

			gbc.gridwidth = 6;
			gbc.gridx = 2;
			this.add(c5v0Bar, gbc);

			gbc.gridwidth = 8;// channels
			gbc.gridx = 0;
			gbc.gridy = 30;
			this.add(channels, gbc);

			// --------------------

			GridLayout gl = new GridLayout(2, 7, 5, 5);

			channelWrapper.setLayout(gl);
			channelWrapper.setBackground(CustomColors.NAVY);

			channelWrapper.add(ch0);
			channelWrapper.add(ch1);
			channelWrapper.add(ch2);
			channelWrapper.add(ch3);
			channelWrapper.add(ch4);
			channelWrapper.add(ch5);
			channelWrapper.add(ch6);
			channelWrapper.add(ch7);
			channelWrapper.add(ch8);
			channelWrapper.add(ch9);
			channelWrapper.add(ch10);
			channelWrapper.add(ch11);
			channelWrapper.add(ch12);
			channelWrapper.add(ch13);

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

					channelLabels.get(i).setBackground(CustomColors.CRIMSON);
					channelLabels.get(i).setForeground(Color.WHITE);

				}

			}

		}

	}

	public void updateData(CougSat1Telemetry data) {

		rcsHealth.updateData(data);
		ihuHealth.updateData(data);
		powerHealth.updateData(data);
		adcsHealth.updateData(data);
		tempHealth.updateData(data);

	}
}
