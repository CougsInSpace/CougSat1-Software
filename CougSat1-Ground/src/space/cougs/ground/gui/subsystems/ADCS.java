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
import space.cougs.ground.gui.subsystems.modules.HorizontalText;
import space.cougs.ground.gui.subsystems.modules.Map;
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

	private final HorizontalText adcsTemp = new HorizontalText("Temp:", "        ", 0.5);
	private final HorizontalText roll = new HorizontalText("Roll:", "        ", 0.5);
	private final HorizontalText pitch = new HorizontalText("Pitch:", "        ", 0.5);
	private final HorizontalText yaw = new HorizontalText("Yaw:", "        ", 0.5);
	private final HorizontalText xPWMOut = new HorizontalText("X PWM Out:", "        ", 0.5);
	private final HorizontalText yPWMOut = new HorizontalText("Y PWM Out:", "        ", 0.5);
	private final HorizontalText zPWMOut = new HorizontalText("Z PWM Out:", "        ", 0.5);
	private final HorizontalText xCurrent = new HorizontalText("X Current:", "        ", 0.5);
	private final HorizontalText yCurrent = new HorizontalText("Y Current:", "        ", 0.5);
	private final HorizontalText zCurrent = new HorizontalText("Z Current:", "        ", 0.5);

	public ADCS() {

		super();
		this.setBackground(CustomColors.BACKGROUND2);

		GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();
		gbc.setFill(GridBagConstraintsWrapper.BOTH);

		this.setLayout(new GridBagLayout());

		coordinatePanel.setLayout(new BorderLayout());

		panelWrapper.setOpaque(false);
		panelWrapper.setBackground(CustomColors.BACKGROUND1);

		earthPanel.setLayout(new GridBagLayout());
		earthPanel.add(new JLabel("Lattitude"),
				gbc.setLocation(0, 0).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		earthPanel.add(lattCoordinates, gbc.setLocation(1, 0).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		earthPanel.add(new JLabel("°"), gbc.setLocation(2, 0).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));

		earthPanel.add(new JLabel("Longitude"),
				gbc.setLocation(0, 1).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		earthPanel.add(longCoordinates, gbc.setLocation(1, 1).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		earthPanel.add(new JLabel("°"), gbc.setLocation(2, 1).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));

		earthPanel.add(new JButton("Transmit"),
				gbc.setLocation(0, 2).setSize(3, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		earthPanel.setBackground(CustomColors.BACKGROUND1);
		panelWrapper.add(earthPanel, "Earth");

		attitudePanel.setLayout(new GridBagLayout());
		attitudePanel.add(new JLabel("Right Ascension"),
				gbc.setLocation(0, 0).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		attitudePanel.add(rightAscensionCoordinates,
				gbc.setLocation(1, 0).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		attitudePanel.add(new JLabel("°"),
				gbc.setLocation(2, 0).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));

		attitudePanel.add(new JLabel("Left Ascension"),
				gbc.setLocation(0, 1).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		attitudePanel.add(leftAscensionCoordinates,
				gbc.setLocation(1, 1).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		attitudePanel.add(new JLabel("°"),
				gbc.setLocation(2, 1).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));

		attitudePanel.add(new JButton("Transmit"),
				gbc.setLocation(0, 2).setSize(3, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		attitudePanel.setBackground(CustomColors.BACKGROUND1);
		panelWrapper.add(attitudePanel, "Attitude");

		celestialPanel.setLayout(new GridBagLayout());
		celestialPanel.add(new JLabel("Roll"),
				gbc.setLocation(0, 0).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		celestialPanel.add(rollCoordinates,
				gbc.setLocation(1, 0).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		celestialPanel.add(new JLabel("°"),
				gbc.setLocation(2, 0).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));

		celestialPanel.add(new JLabel("Pitch"),
				gbc.setLocation(0, 1).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		celestialPanel.add(pitchCoordinates,
				gbc.setLocation(1, 1).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		celestialPanel.add(new JLabel("°"),
				gbc.setLocation(2, 1).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));

		celestialPanel.add(new JLabel("Yaw"),
				gbc.setLocation(0, 2).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		celestialPanel.add(yawCoordinates,
				gbc.setLocation(1, 2).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		celestialPanel.add(new JLabel("°"),
				gbc.setLocation(2, 2).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		celestialPanel.add(new JButton("Transmit"),
				gbc.setLocation(0, 3).setSize(3, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		celestialPanel.setBackground(CustomColors.BACKGROUND1);
		panelWrapper.add(celestialPanel, "Celestial");

		coordinatePanel.add(panelWrapper, BorderLayout.CENTER);
		coordinatePanel.add(cardSwitcher, BorderLayout.PAGE_START);
		coordinatePanel.setBackground(CustomColors.BACKGROUND2);
		this.add(coordinatePanel, gbc.setLocation(2, 2).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));

		telemetryPanel.setLayout(new GridBagLayout());
		telemetryPanel.add(adcsTemp, gbc.setLocation(0, 1).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		telemetryPanel.add(roll, gbc.setLocation(0, 2).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		telemetryPanel.add(pitch, gbc.setLocation(0, 3).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		telemetryPanel.add(yaw, gbc.setLocation(0, 4).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		telemetryPanel.add(xPWMOut, gbc.setLocation(0, 5).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		telemetryPanel.add(yPWMOut, gbc.setLocation(1, 1).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		telemetryPanel.add(zPWMOut, gbc.setLocation(1, 2).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		telemetryPanel.add(xCurrent, gbc.setLocation(1, 3).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		telemetryPanel.add(yCurrent, gbc.setLocation(1, 4).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		telemetryPanel.add(zCurrent, gbc.setLocation(1, 5).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		telemetryPanel.setBackground(CustomColors.BACKGROUND1);
		attitudeIndicator.setBackground(CustomColors.BACKGROUND1);

		this.add(telemetryPanel, gbc.setLocation(1, 2).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		this.add(map, gbc.setLocation(0, 0).setSize(3, 2).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		this.add(attitudeIndicator, gbc.setLocation(0, 2).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));

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

			this.add(earthButton, gbc.setLocation(1, 0).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
			this.add(attitudeButton, gbc.setLocation(2, 0).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
			this.add(celestialButton, gbc.setLocation(3, 0).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));

			for (Component component : this.getComponents()) {
				if (component instanceof JButton) {
					((AbstractButton) component).setHorizontalAlignment(SwingConstants.LEFT);
					((JButton) component).addActionListener(this);
					((JButton) component).setBorder(BorderFactory.createEmptyBorder(5, 15, 5, 15));
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
		map.setValue(satellite.getLattitude(), satellite.getLongitude());

		adcsTemp.setValue(String.format(" %d°", satellite.getADCSTemp()));
		roll.setValue(String.format(" %6.2f°", satellite.getRoll()));
		pitch.setValue(String.format(" %6.2f°", satellite.getPitch()));
		yaw.setValue(String.format(" %6.2f°", satellite.getYaw()));
		xPWMOut.setValue(String.format(" %d", satellite.getXPWMOut()));
		yPWMOut.setValue(String.format(" %d", satellite.getYPWMOut()));
		zPWMOut.setValue(String.format(" %d", satellite.getZPWMOut()));
		xCurrent.setValue(String.format(" %f", satellite.getXCurrent()));
		yCurrent.setValue(String.format(" %f", satellite.getYCurrent()));
		zCurrent.setValue(String.format(" %f", satellite.getZCurrent()));
		
		
		
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
	}
}
