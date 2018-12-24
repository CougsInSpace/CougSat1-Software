package space.cougs.ground.gui.subsystems;

import java.awt.BorderLayout;
import java.awt.CardLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.Font;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.AbstractButton;
import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.SwingConstants;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.modules.HorizontalValue;
import space.cougs.ground.gui.modules.Map;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;
import space.cougs.ground.gui.utils.GridBagConstraintsWrapper;
import space.cougs.ground.satellites.CougSat;

public class ADCS extends JPanel implements UIScaling, SatelliteInfo {

	private static final long serialVersionUID = 1L;

	private final JPanel coordinatePanel = new JPanel();
	private final Map map = new Map(46.7304889, -117.1750474, 2.00);
	private final JPanel panelWrapper = new JPanel(new CardLayout());
	private final CardSwitcher cardSwitcher = new CardSwitcher();
	private final JPanel telemetryPanel = new JPanel();
	private final JPanel attitudeIndicator = new JPanel();

	private final JPanel earthPanel = new JPanel();
	private final JPanel attitudePanel = new JPanel();
	private final JPanel celestialPanel = new JPanel();

	private final JTextField lattCoordinates = new JTextField();
	private final JTextField longCoordinates = new JTextField();
	private final JTextField rightAscensionCoordinates = new JTextField();
	private final JTextField leftAscensionCoordinates = new JTextField();
	private final JTextField rollCoordinates = new JTextField();
	private final JTextField yawCoordinates = new JTextField();
	private final JTextField pitchCoordinates = new JTextField();

	private final HorizontalValue adcsTemp = new HorizontalValue("Temp:", "        ", 0.5);
	private final HorizontalValue roll = new HorizontalValue("Roll:", "        ", 0.5);
	private final HorizontalValue pitch = new HorizontalValue("Pitch:", "        ", 0.5);
	private final HorizontalValue yaw = new HorizontalValue("Yaw:", "        ", 0.5);
	private final HorizontalValue xPWMOut = new HorizontalValue("X PWM Out:", "        ", 0.5);
	private final HorizontalValue yPWMOut = new HorizontalValue("Y PWM Out:", "        ", 0.5);
	private final HorizontalValue zPWMOut = new HorizontalValue("Z PWM Out:", "        ", 0.5);
	private final HorizontalValue xCurrent = new HorizontalValue("X Current:", "        ", 0.5);
	private final HorizontalValue yCurrent = new HorizontalValue("Y Current:", "        ", 0.5);
	private final HorizontalValue zCurrent = new HorizontalValue("Z Current:", "        ", 0.5);

	public ADCS() {

		super();
		this.setBackground(CustomColors.BACKGROUND22);
		this.setBorder(BorderFactory.createLineBorder(CustomColors.BACKGROUND12, 10));
		
		GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();
		gbc.setFill(GridBagConstraintsWrapper.BOTH);

		this.setLayout(new GridBagLayout());

		coordinatePanel.setLayout(new BorderLayout());

		panelWrapper.setOpaque(false);
		panelWrapper.setBackground(CustomColors.BACKGROUND12);

		earthPanel.setLayout(new GridBagLayout());
		earthPanel.add(new JLabel("Latitude"),
				gbc.setXY(0, 0).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		earthPanel.add(lattCoordinates, gbc.setXY(1, 0).setSize(1, 1).setWeight(1.0, 0.0));
		earthPanel.add(new JLabel("�N"), gbc.setXY(2, 0).setSize(1, 1).setWeight(0.0, 0.0));

		earthPanel.add(new JLabel("Longitude"),
				gbc.setXY(0, 1).setSize(1, 1).setWeight(0.0, 0.0));
		earthPanel.add(longCoordinates, gbc.setXY(1, 1).setSize(1, 1));
		earthPanel.add(new JLabel("�E"), gbc.setXY(2, 1).setSize(1, 1));

		earthPanel.add(new JButton("Transmit"),
				gbc.setXY(0, 2).setSize(3, 1).setWeight(1.0, 0.0));
		earthPanel.setBackground(CustomColors.BACKGROUND12);
		panelWrapper.add(earthPanel, "Earth");

		celestialPanel.setLayout(new GridBagLayout());
		celestialPanel.add(new JLabel("Right Ascension"),
				gbc.setXY(0, 0).setSize(1, 1).setWeight(0.0, 0.0));
		celestialPanel.add(rightAscensionCoordinates,
				gbc.setXY(1, 0).setSize(1, 1).setWeight(1.0, 0.0));
		celestialPanel.add(new JLabel("�"),
				gbc.setXY(2, 0).setSize(1, 1).setWeight(0.0, 0.0));

		celestialPanel.add(new JLabel("Left Ascension"),
				gbc.setXY(0, 1).setSize(1, 1).setWeight(0.0, 0.0));
		celestialPanel.add(leftAscensionCoordinates,
				gbc.setXY(1, 1).setSize(1, 1).setWeight(1.0, 0.0));
		celestialPanel.add(new JLabel("�"),
				gbc.setXY(2, 1).setSize(1, 1).setWeight(0.0, 0.0));

		celestialPanel.add(new JButton("Transmit"),
				gbc.setXY(0, 2).setSize(3, 1).setWeight(1.0, 0.0));
		celestialPanel.setBackground(CustomColors.BACKGROUND12);
		panelWrapper.add(celestialPanel, "Celestial");

		attitudePanel.setLayout(new GridBagLayout());
		attitudePanel.add(new JLabel("Roll"),
				gbc.setXY(0, 0).setSize(1, 1).setWeight(0.0, 0.0));
		attitudePanel.add(rollCoordinates,
				gbc.setXY(1, 0).setSize(1, 1).setWeight(1.0, 0.0));
		attitudePanel.add(new JLabel("�"),
				gbc.setXY(2, 0).setSize(1, 1).setWeight(0.0, 0.0));

		attitudePanel.add(new JLabel("Pitch"),
				gbc.setXY(0, 1).setSize(1, 1).setWeight(0.0, 0.0));
		attitudePanel.add(pitchCoordinates,
				gbc.setXY(1, 1).setSize(1, 1).setWeight(1.0, 0.0));
		attitudePanel.add(new JLabel("�"),
				gbc.setXY(2, 1).setSize(1, 1).setWeight(0.0, 0.0));

		attitudePanel.add(new JLabel("Yaw"),
				gbc.setXY(0, 2).setSize(1, 1).setWeight(0.0, 0.0));
		attitudePanel.add(yawCoordinates,
				gbc.setXY(1, 2).setSize(1, 1).setWeight(1.0, 0.0));
		attitudePanel.add(new JLabel("�"),
				gbc.setXY(2, 2).setSize(1, 1).setWeight(0.0, 0.0));
		attitudePanel.add(new JButton("Transmit"),
				gbc.setXY(0, 3).setSize(3, 1).setWeight(1.0, 0.0));
		attitudePanel.setBackground(CustomColors.BACKGROUND12);
		panelWrapper.add(attitudePanel, "Attitude");

		coordinatePanel.add(panelWrapper, BorderLayout.CENTER);
		coordinatePanel.add(cardSwitcher, BorderLayout.PAGE_START);
		coordinatePanel.setBackground(CustomColors.BACKGROUND22);
		this.add(coordinatePanel, gbc.setXY(2, 2).setSize(1, 1).setWeight(1.0, 0.0));

		telemetryPanel.setLayout(new GridBagLayout());
		telemetryPanel.add(adcsTemp, gbc.setXY(0, 1).setSize(1, 1).setWeight(1.0, 0.0));
		telemetryPanel.add(roll, gbc.setXY(0, 2).setSize(1, 1).setWeight(1.0, 0.0));
		telemetryPanel.add(pitch, gbc.setXY(0, 3).setSize(1, 1).setWeight(1.0, 0.0));
		telemetryPanel.add(yaw, gbc.setXY(0, 4).setSize(1, 1).setWeight(1.0, 0.0));
		telemetryPanel.add(xPWMOut, gbc.setXY(0, 5).setSize(1, 1).setWeight(1.0, 0.0));
		telemetryPanel.add(yPWMOut, gbc.setXY(1, 1).setSize(1, 1).setWeight(1.0, 0.0));
		telemetryPanel.add(zPWMOut, gbc.setXY(1, 2).setSize(1, 1).setWeight(1.0, 0.0));
		telemetryPanel.add(xCurrent, gbc.setXY(1, 3).setSize(1, 1).setWeight(1.0, 0.0));
		telemetryPanel.add(yCurrent, gbc.setXY(1, 4).setSize(1, 1).setWeight(1.0, 0.0));
		telemetryPanel.add(zCurrent, gbc.setXY(1, 5).setSize(1, 1).setWeight(1.0, 0.0));
		telemetryPanel.setBackground(CustomColors.BACKGROUND12);
		attitudeIndicator.setBackground(CustomColors.BACKGROUND12);

		this.add(telemetryPanel, gbc.setXY(1, 2).setSize(1, 1).setWeight(0.0, 0.0));
		this.add(map, gbc.setXY(0, 0).setSize(3, 2).setWeight(1.0, 1.0));
		this.add(attitudeIndicator, gbc.setXY(0, 2).setSize(1, 1).setWeight(1.0, 0.0));

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

	private class CardSwitcher extends JPanel implements ActionListener, UIScaling {

		private static final long serialVersionUID = 1L;

		private final JButton earthButton = new JButton("Earth");
		private final JButton attitudeButton = new JButton("Attitude");
		private final JButton celestialButton = new JButton("Celestial");

		private CardSwitcher() {

			GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();
			gbc.setFill(GridBagConstraintsWrapper.BOTH);

			this.setOpaque(false);
			this.setLayout(new GridBagLayout());

			earthButton.setHorizontalAlignment(SwingConstants.CENTER);
			attitudeButton.setHorizontalAlignment(SwingConstants.CENTER);
			celestialButton.setHorizontalAlignment(SwingConstants.CENTER);

			this.add(earthButton, gbc.setXY(1, 0).setSize(1, 1).setWeight(1.0, 1.0).setInsets(0, 0, 10, 5));
			this.add(attitudeButton, gbc.setXY(2, 0).setSize(1, 1).setWeight(1.0, 1.0).setInsets(0, 5, 10, 5));
			this.add(celestialButton, gbc.setXY(3, 0).setSize(1, 1).setWeight(1.0, 1.0).setInsets(0, 5, 10, 0));

			for (Component component : this.getComponents()) {
				if (component instanceof JButton) {
					((AbstractButton) component).setHorizontalAlignment(SwingConstants.LEFT);
					((JButton) component).addActionListener(this);
					((JButton) component).setBorder(BorderFactory.createEmptyBorder(0, 5, 5, 5));
					component.setForeground(CustomColors.TEXT1);
					component.setFont(Fonts.BODY_16);
					((JButton) component).setIconTextGap(15);
					((JButton) component).setFocusable(false);
				}
			}
			earthButton.doClick();
		}

		@Override
		public void updateUIScaling(UIScale uiScale) {
			Font font = Fonts.BODY_16;

			switch (uiScale) {
			default:
			case SCALE_100:
				font = Fonts.BODY_16;
				break;
			case SCALE_150:
				font = Fonts.BODY_24;
				break;
			case SCALE_200:
				font = Fonts.BODY_32;
				break;
			case SCALE_300:
				font = Fonts.BODY_48;
				break;
			case SCALE_75:
				font = Fonts.BODY_12;
				break;
			}

			for (Component component : this.getComponents()) {
				if (component instanceof JButton) {

					component.setFont(font);
					component.setForeground(CustomColors.TEXT1);

				}
			}
		}

		@Override
		public void actionPerformed(ActionEvent evt) {

			((CardLayout) panelWrapper.getLayout()).show(panelWrapper, evt.getActionCommand());

			for (Component component : this.getComponents()) {

				if (component instanceof JButton) {

					component.setBackground(CustomColors.BUTTON_INACTIVE);
				}
			}

			((JButton) evt.getSource()).setBackground(CustomColors.BUTTON_ACTIVE);
		}
	}

	@Override
	public void updateSatellite(CougSat satellite) {
		map.setValue(satellite.getADCS().getLatitude(), satellite.getADCS().getLongitude());

		adcsTemp.setValue(String.format(" %d�C", satellite.getECS().getADCSTemp()));
		roll.setValue(String.format(" %6.2f�", satellite.getADCS().getRoll()));
		pitch.setValue(String.format(" %6.2f�", satellite.getADCS().getPitch()));
		yaw.setValue(String.format(" %6.2f�", satellite.getADCS().getYaw()));
		xPWMOut.setValue(String.format(" %d", satellite.getADCS().getXPWMOut()));
		yPWMOut.setValue(String.format(" %d", satellite.getADCS().getYPWMOut()));
		zPWMOut.setValue(String.format(" %d", satellite.getADCS().getZPWMOut()));
		xCurrent.setValue(String.format(" %fA", satellite.getADCS().getXCurrent()));
		yCurrent.setValue(String.format(" %fA", satellite.getADCS().getYCurrent()));
		zCurrent.setValue(String.format(" %fA", satellite.getADCS().getZCurrent()));

	}

	@Override
	public void updateUIScaling(UIScale uiScale) {

		for (Component component : this.getComponents()) {

			if (component instanceof UIScaling) {

				((UIScaling) component).updateUIScaling(uiScale);

			}
			if (component instanceof JPanel) {
				for (Component subComponent : ((Container) component).getComponents()) {
					if (subComponent instanceof UIScaling) {

						((UIScaling) subComponent).updateUIScaling(uiScale);

					}
				}
			}
		}

		Font font = Fonts.BODY_16;

		switch (uiScale) {
		default:
		case SCALE_100:
			font = Fonts.BODY_16;
			break;
		case SCALE_150:
			font = Fonts.BODY_24;
			break;
		case SCALE_200:
			font = Fonts.BODY_32;
			break;
		case SCALE_300:
			font = Fonts.BODY_48;
			break;
		case SCALE_75:
			font = Fonts.BODY_12;
			break;
		}

		for (Component component : panelWrapper.getComponents()) {
			if (component instanceof JPanel) {

				for (Component subComponent : ((Container) component).getComponents()) {
					subComponent.setForeground(CustomColors.TEXT1);
					subComponent.setFont(font);
					
					if (subComponent instanceof JButton) {
						subComponent.setBackground(CustomColors.BACKGROUND22);
					}
				}
			}
		}
	}
}
