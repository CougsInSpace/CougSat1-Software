package space.cougs.ground.gui;

import java.awt.BorderLayout;
import java.awt.CardLayout;
import java.awt.Component;
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
import space.cougs.ground.gui.subsystems.Camera;
import space.cougs.ground.gui.subsystems.EPS;
import space.cougs.ground.gui.subsystems.Health;
import space.cougs.ground.gui.subsystems.IFJR;
import space.cougs.ground.gui.subsystems.IHU;
import space.cougs.ground.gui.subsystems.Plant;
import space.cougs.ground.gui.subsystems.RCS;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;

class CougSat1 extends JPanel implements UIScaling {

	private static final long serialVersionUID = 1L;

	private final Health health; // all telemetry data
	private final ADCS adcs;
	private final IFJR ifjr;
	private final IHU ihu; // Internal houskeeping unit shhhh
	private final RCS rcs;
	private final Camera camera; // camera data
	private final Plant plant; // images of plant
	private final EPS eps; // power?

	JPanel subSystemWrapper = new JPanel(new CardLayout());

	CardSwitcher cardSwitcher;

	public CougSat1() {
		super();

		this.setLayout(new BorderLayout());

		health = new Health();
		adcs = new ADCS();
		ifjr = new IFJR();
		ihu = new IHU();
		rcs = new RCS();
		camera = new Camera();
		plant = new Plant();
		eps = new EPS();

		subSystemWrapper.add(health, "Health");
		subSystemWrapper.add(adcs, "Attitude");
		subSystemWrapper.add(ifjr, "IFJR");
		subSystemWrapper.add(ihu, "Computer");
		subSystemWrapper.add(rcs, "Communication");
		subSystemWrapper.add(camera, "Camera");
		subSystemWrapper.add(plant, "Plant");
		subSystemWrapper.add(eps, "Power");
		subSystemWrapper.setOpaque(false);
		this.add(subSystemWrapper, BorderLayout.CENTER);

		cardSwitcher = new CardSwitcher();
		this.add(cardSwitcher, BorderLayout.LINE_START);

		this.setBackground(CustomColors.BACKGROUND1);
		this.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

	}

	private class CardSwitcher extends JPanel implements ActionListener, UIScaling {

		private static final long serialVersionUID = 1L;

		private final JButton healthButton;
		private final JButton adcsButton;
		private final JButton ifjrButton;
		private final JButton ihuButton;
		private final JButton rcsButton;
		private final JButton cameraButton;
		private final JButton plantButton;
		private final JButton epsButton;

		CardSwitcher() {

			healthButton = new JButton("Health");
			adcsButton = new JButton("Attitude");
			ifjrButton = new JButton("IFJR");
			ihuButton = new JButton("Computer");
			rcsButton = new JButton("Communication");
			cameraButton = new JButton("Camera");
			plantButton = new JButton("Plant");
			epsButton = new JButton("Power");

			this.setOpaque(false);
			this.setLayout(new GridLayout(0, 1, 10, 10));
			this.add(healthButton);
			this.add(adcsButton);
			this.add(epsButton);
			this.add(ifjrButton);
			this.add(ihuButton);
			this.add(rcsButton);
			this.add(cameraButton);
			this.add(plantButton);

			for (Component component : this.getComponents()) {
				if (component instanceof JButton) {
					((AbstractButton) component).setHorizontalAlignment(SwingConstants.LEFT);
					((JButton) component).addActionListener(this);
					((JButton) component).setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));
					component.setForeground(CustomColors.TEXT1);
					component.setFont(Fonts.BODY_16);
				}
			}
			healthButton.doClick();
		}

		@Override
		public void updateUIScaling(UIScale uiScale) {

			Font font = Fonts.BODY_16;
			String iconFolderPath = "resources/images/";

			switch (uiScale) {
			case SCALE_100:
				font = Fonts.BODY_16;
				iconFolderPath += "48/";
				break;
			case SCALE_150:
				font = Fonts.BODY_24;
				iconFolderPath += "64/";
				break;
			case SCALE_200:
				font = Fonts.BODY_32;
				iconFolderPath += "128/";
				break;
			case SCALE_300:
				font = Fonts.BODY_48;
				iconFolderPath += "128/";
				break;
			case SCALE_75:
				font = Fonts.BODY_12;
				iconFolderPath += "32/";
				break;
			default:
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

		}

	}
}
