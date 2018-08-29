package space.cougs.ground.gui;

import java.awt.BorderLayout;
import java.awt.CardLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.AbstractButton;
import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.SwingConstants;

import space.cougs.ground.gui.subsystems.ADCS;
import space.cougs.ground.gui.subsystems.CDH;
import space.cougs.ground.gui.subsystems.Camera;
import space.cougs.ground.gui.subsystems.Comms;
import space.cougs.ground.gui.subsystems.EPS;
import space.cougs.ground.gui.subsystems.Health;
import space.cougs.ground.gui.subsystems.IFJR;
import space.cougs.ground.gui.subsystems.Plant;
import space.cougs.ground.gui.subsystems.SatelliteInfo;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;
import space.cougs.ground.satellites.CougSat;

class CougSat1GUI extends JPanel implements UIScaling, SatelliteInfo {

	private static final long serialVersionUID = 1L;

	private final Health health = new Health(); // all telemetry data
	private final ADCS adcs = new ADCS();
	private final IFJR ifjr = new IFJR();
	private final CDH cdh = new CDH(); // Internal houskeeping unit shhhh
	private final Comms comms = new Comms();
	private final Camera camera = new Camera(); // camera data
	private final Plant plant = new Plant(); // images of plant
	private final EPS eps = new EPS(); // power?

	private final JPanel subSystemWrapper = new JPanel(new CardLayout());

	private final CardSwitcher cardSwitcher = new CardSwitcher();

	CougSat1GUI() {
		super();

		this.setLayout(new BorderLayout());

		subSystemWrapper.add(health, "Health");
		subSystemWrapper.add(adcs, "Attitude");
		subSystemWrapper.add(ifjr, "IFJR");
		subSystemWrapper.add(cdh, "Computer");
		subSystemWrapper.add(comms, "Radio");
		subSystemWrapper.add(camera, "Camera");
		subSystemWrapper.add(plant, "Plant");
		subSystemWrapper.add(eps, "Power");
		subSystemWrapper.setOpaque(false);

		this.add(subSystemWrapper, BorderLayout.CENTER);
		this.add(cardSwitcher, BorderLayout.LINE_START);

		this.setBackground(CustomColors.BACKGROUND1);

	}

	private class CardSwitcher extends JPanel implements ActionListener, UIScaling {

		private static final long serialVersionUID = 1L;

		private final JButton healthButton = new JButton("Health");
		private final JButton adcsButton = new JButton("Attitude");
		private final JButton ifjrButton = new JButton("IFJR");
		private final JButton cdhButton = new JButton("Computer");
		private final JButton commsButton = new JButton("Radio");
		private final JButton cameraButton = new JButton("Camera");
		private final JButton plantButton = new JButton("Plant");
		private final JButton epsButton = new JButton("Power");

		private CardSwitcher() {

			this.setOpaque(false);
			this.setLayout(new GridLayout(0, 1, 10, 10));
			this.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 0));
			this.add(healthButton);
			this.add(adcsButton);
			this.add(epsButton);
			this.add(ifjrButton);
			this.add(cdhButton);
			this.add(commsButton);
			this.add(cameraButton);
			this.add(plantButton);

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
			healthButton.doClick();
		}

		@Override
		public void updateUIScaling(UIScale uiScale) {

			Font font = Fonts.BODY_24;
			String iconFolderPath = "resources/images/";

			switch (uiScale) {
			default:
			case SCALE_100:
				font = Fonts.BODY_24;
				iconFolderPath += "48/";
				break;
			case SCALE_150:
				font = Fonts.BODY_36;
				iconFolderPath += "64/";
				break;
			case SCALE_200:
				font = Fonts.BODY_48;
				iconFolderPath += "128/";
				break;
			case SCALE_300:
				font = Fonts.BODY_48;
				iconFolderPath += "128/";
				break;
			case SCALE_75:
				font = Fonts.BODY_16;
				iconFolderPath += "32/";
				break;
			}

			for (Component component : this.getComponents()) {
				if (component instanceof JButton) {

					String iconFilePath = iconFolderPath + ((AbstractButton) component).getText() + ".png";

					component.setFont(font);
					((JButton) component).setIcon(new ImageIcon(iconFilePath));

				}
			}
		}

		@Override
		public void actionPerformed(ActionEvent evt) {

			((CardLayout) subSystemWrapper.getLayout()).show(subSystemWrapper, evt.getActionCommand());

			for (Component component : this.getComponents()) {

				if (component instanceof JButton) {

					component.setBackground(CustomColors.BUTTON_INACTIVE);
				}
			}

			((JButton) evt.getSource()).setBackground(CustomColors.BUTTON_ACTIVE);
		}

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

	@Override
	public void updateSatellite(CougSat satellite) {

		for (Component component : subSystemWrapper.getComponents()) {

			if (component instanceof SatelliteInfo) {

				((SatelliteInfo) component).updateSatellite(satellite);

			}
		}
		this.repaint();
	}
}
