package space.cougs.ground.gui.subsystems;

import java.awt.Color;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;

import javax.swing.BorderFactory;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.SwingConstants;

import space.cougs.ground.CougSat1Telemetry;
import space.cougs.ground.utils.CustomColors;
import space.cougs.ground.utils.Fonts;

@SuppressWarnings("serial")
public class HealthPanel extends JPanel {

	IHUHealth ihuHealth = new IHUHealth();
	JPanel tempHealth = new TempHealth();
	JPanel adcsHealth = new ADCSHealth();
	RCSHealth rcsHealth = new RCSHealth();
	JPanel powerHealth = new PowerHealth();

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
			this.add(txLabel, gbc);

			gbc.gridx = 1;
			this.add(rxLabel, gbc);

			gbc.gridx = 2;
			this.add(snrLabel, gbc);

		}

		private void updateBars(double tXPower, double rXPower, double rxSNR) {

			int newTXPower = (int) ((tXPower / 4.0) * 100.0);
			int newRXPower = (int) ((rXPower / 0.1) * 100.0);
			int newRXSNR = (int) (((rxSNR + 30.0) / 60.0) * 100.0);

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

		public void updateData(String newMode, String newTime, long newSd, int newResets, int newPayload) {

			String buff = "";
			mode.setText(newMode);
			time.setText(newTime + "");
			if (newSd > 1000000000) {
				buff = (newSd / 1000000000) + " GB";
			} else if (newSd > 1000000) {
				buff = (newSd / 1000000) + " MB";
			} else if (newSd > 1000) {
				buff = (newSd / 1000) + " kB";
			} else {
				buff = (newSd) + " B";
			}
			sdUsed.setText(buff);
			reset.setText(newResets + "");
			payload.setText(newPayload + "");

		}
	}

	private class TempHealth extends JPanel {

		JLabel temp = new JLabel("Temp", SwingConstants.CENTER);

		public TempHealth() {

			temp.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			temp.setForeground(Color.WHITE);
			this.add(temp);
		}
	}

	private class ADCSHealth extends JPanel {

		JLabel adcs = new JLabel("Adcs", SwingConstants.CENTER);

		public ADCSHealth() {

			adcs.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			adcs.setForeground(Color.WHITE);
			this.add(adcs);
		}
	}

	private class PowerHealth extends JPanel {

		JLabel power = new JLabel("Power", SwingConstants.CENTER);

		public PowerHealth() {

			power.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
			power.setForeground(Color.WHITE);
			this.add(power);
		}
	}

	public void updateData(CougSat1Telemetry data) {

		rcsHealth.updateBars(data.getTXPower(), data.getRXPower(), data.getRXSNR());
		ihuHealth.updateData(data.getMode(), data.getTime(), data.getIHUSdCard(), data.getResetCount(),
				data.getPayloadFrames());

	}
}
