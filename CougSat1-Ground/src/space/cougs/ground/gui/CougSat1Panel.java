package space.cougs.ground.gui;

import java.awt.CardLayout;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.Toolkit;
import java.util.ArrayList;

import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.SwingConstants;

import space.cougs.ground.gui.subsystems.ADCSPanel;
import space.cougs.ground.gui.subsystems.CameraPanel;
import space.cougs.ground.gui.subsystems.EPSPanel;
import space.cougs.ground.gui.subsystems.HealthPanel;
import space.cougs.ground.gui.subsystems.IFJRPanel;
import space.cougs.ground.gui.subsystems.IHUPanel;
import space.cougs.ground.gui.subsystems.PlantPanel;
import space.cougs.ground.gui.subsystems.RCSPanel;
import space.cougs.ground.satellites.CougSat1Telemetry;
import space.cougs.ground.utils.CustomColors;
import space.cougs.ground.utils.Fonts;

class CougSat1Panel extends JPanel {

	HealthPanel healthPanel = new HealthPanel();// all telemetry data
	ADCSPanel adcsPanel = new ADCSPanel();
	IFJRPanel ifjrPanel = new IFJRPanel();
	IHUPanel ihuPanel = new IHUPanel();// in house upkeep
	RCSPanel rcsPanel = new RCSPanel();
	CameraPanel cameraPanel = new CameraPanel();// camera data
	PlantPanel plantPanel = new PlantPanel();// images of plant
	EPSPanel epsPanel = new EPSPanel();

	ArrayList<JButton> buttons = new ArrayList<JButton>();

	JButton healthButton = new JButton("Health", new ImageIcon("resources/images/128/heart.png"));
	JButton adcsButton = new JButton("Attitude", new ImageIcon("resources/images/128/globe.png"));
	JButton ifjrButton = new JButton("IFJR", new ImageIcon("resources/images/128/terminal.png"));
	JButton ihuButton = new JButton("Computer", new ImageIcon("resources/images/128/keyboard.png"));
	JButton rcsButton = new JButton("Communication", new ImageIcon("resources/images/128/communications.png"));
	JButton cameraButton = new JButton("Camera", new ImageIcon("resources/images/128/camera.png"));
	JButton plantButton = new JButton("Plant", new ImageIcon("resources/images/128/plant.png"));
	JButton epsButton = new JButton("Power", new ImageIcon("resources/images/128/battery.png"));

	JPanel subSystemWrapper = new JPanel(new CardLayout());

	CardSwitcher cardSwitcher = new CardSwitcher(subSystemWrapper, (CardLayout) subSystemWrapper.getLayout(), buttons);

	public CougSat1Panel() {
		super();
		
		// subSystemWrapper is used to switch between panels
		subSystemWrapper.setOpaque(false);
		subSystemWrapper.add(healthPanel, "Health");
		subSystemWrapper.add(adcsPanel, "Attitude");
		subSystemWrapper.add(ifjrPanel, "IFJR");
		subSystemWrapper.add(ihuPanel, "Computer");
		subSystemWrapper.add(rcsPanel, "Communication");
		subSystemWrapper.add(cameraPanel, "Camera");
		subSystemWrapper.add(plantPanel, "Plant");
		subSystemWrapper.add(epsPanel, "Power");

		this.setBackground(CustomColors.WSU_GRAY);
		this.setLayout(new GridBagLayout());

		buttons.add(healthButton);
		buttons.add(adcsButton);
		buttons.add(ifjrButton);
		buttons.add(ihuButton);
		buttons.add(rcsButton);
		buttons.add(cameraButton);
		buttons.add(plantButton);
		buttons.add(epsButton);

		for (JButton button : buttons) {
			button.setHorizontalAlignment(SwingConstants.LEFT);
			button.setBackground(CustomColors.CRIMSON);
			button.setForeground(Color.WHITE);
			button.setFocusPainted(false);
			button.addActionListener(cardSwitcher);
			button.setFont(Fonts.getInstance().codeNewRoman[Fonts.getInstance().MEDIUM]);
		}

		healthButton.doClick();// starts with the health button clicked

		GridBagConstraints gbc = new GridBagConstraints();
		gbc.fill = GridBagConstraints.HORIZONTAL;
		gbc.gridx = 0;
		gbc.gridy = 0;
		gbc.weightx = 1;
		gbc.weighty = 1;
		gbc.gridheight = 1;
		gbc.gridwidth = 1;
		this.add(healthButton, gbc);

		gbc.gridy = 1;
		this.add(adcsButton, gbc);

		gbc.gridy = 2;
		this.add(rcsButton, gbc);

		gbc.gridy = 3;
		this.add(ifjrButton, gbc);

		gbc.gridy = 4;
		this.add(ihuButton, gbc);

		gbc.gridy = 5;
		this.add(epsButton, gbc);

		gbc.gridy = 6;
		this.add(cameraButton, gbc);

		gbc.gridy = 7;
		this.add(plantButton, gbc);

		gbc.fill = GridBagConstraints.BOTH;
		gbc.gridy = 0;
		gbc.gridx = 1;
		gbc.gridheight = 8;
		gbc.weightx = 9;

		gbc.insets = new Insets(0, 20, 0, 0);
		this.add(subSystemWrapper, gbc);

		this.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));

	}

	public void upgdateData(CougSat1Telemetry data) {

		healthPanel.updateData(data);

	}
}
