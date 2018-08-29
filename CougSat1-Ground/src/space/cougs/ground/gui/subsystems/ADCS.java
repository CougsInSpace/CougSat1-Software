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
import space.cougs.ground.gui.subsystems.modules.Map;
import space.cougs.ground.gui.subsystems.modules.TitleLabel;
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

	private final JTextField earthLattCoordinates = new JTextField();
	private final JTextField earthLongCoordinates = new JTextField();

	public ADCS() {

		super();
		this.setBackground(CustomColors.BACKGROUND2);

		GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();
		gbc.setFill(GridBagConstraintsWrapper.BOTH);

		this.setLayout(new GridBagLayout());

		coordinatePanel.setLayout(new BorderLayout());

		panelWrapper.setOpaque(false);

		earthPanel.setLayout(new GridBagLayout());
		earthPanel.add(new JLabel("Lattitude"),
				gbc.setLocation(0, 0).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		earthPanel.add(earthLattCoordinates,
				gbc.setLocation(1, 0).setSize(2, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		earthPanel.add(new JLabel("°"), gbc.setLocation(2, 0).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));

		earthPanel.add(new JLabel("Longitude"),
				gbc.setLocation(0, 1).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		earthPanel.add(earthLongCoordinates,
				gbc.setLocation(1, 1).setSize(2, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		earthPanel.add(new JLabel("°"), gbc.setLocation(2, 1).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));

		earthPanel.add(new JButton("Transmit"),
				gbc.setLocation(0, 2).setSize(3, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		earthPanel.setBackground(CustomColors.BACKGROUND1);
		panelWrapper.add(earthPanel, "Earth");

		attitudePanel.setLayout(new GridBagLayout());
		attitudePanel.add(new JLabel("Lattitude"),
				gbc.setLocation(0, 0).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		attitudePanel.add(earthLattCoordinates,
				gbc.setLocation(1, 0).setSize(2, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		attitudePanel.add(new JLabel("°"),
				gbc.setLocation(2, 0).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));

		attitudePanel.add(new JLabel("Longitude"),
				gbc.setLocation(0, 1).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		attitudePanel.add(earthLongCoordinates,
				gbc.setLocation(1, 1).setSize(2, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		attitudePanel.add(new JLabel("°"),
				gbc.setLocation(2, 1).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));

		attitudePanel.add(new JButton("Transmit"),
				gbc.setLocation(0, 2).setSize(3, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		attitudePanel.setBackground(CustomColors.BACKGROUND1);
		panelWrapper.add(attitudePanel, "Attitude");

		celestialPanel.setLayout(new GridBagLayout());
		celestialPanel.add(new JLabel("Lattitude"),
				gbc.setLocation(0, 0).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		celestialPanel.add(earthLattCoordinates,
				gbc.setLocation(1, 0).setSize(2, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		celestialPanel.add(new JLabel("°"),
				gbc.setLocation(2, 0).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));

		celestialPanel.add(new JLabel("Longitude"),
				gbc.setLocation(0, 1).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
		celestialPanel.add(earthLongCoordinates,
				gbc.setLocation(1, 1).setSize(2, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		celestialPanel.add(new JLabel("°"),
				gbc.setLocation(2, 1).setSize(1, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));

		celestialPanel.add(new JButton("Transmit"),
				gbc.setLocation(0, 2).setSize(3, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		celestialPanel.setBackground(CustomColors.BACKGROUND1);
		panelWrapper.add(celestialPanel, "Celestial");

		coordinatePanel.add(panelWrapper, BorderLayout.CENTER);
		coordinatePanel.add(cardSwitcher, BorderLayout.PAGE_START);

		this.add(map, gbc.setLocation(0, 0).setSize(3, 2).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		this.add(attitudeIndicator, gbc.setLocation(0, 2).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		this.add(telemetryPanel, gbc.setLocation(1, 2).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
		this.add(coordinatePanel, gbc.setLocation(2, 2).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));

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
					component.setFont(Fonts.BODY_24);
					((JButton) component).setIconTextGap(15);
					((JButton) component).setFocusable(false);
				}
			}
			earthButton.doClick();
		}

		@Override
		public void updateUIScaling(UIScale uiScale) {
			// TODO Auto-generated method stub

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
}
