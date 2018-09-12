package space.cougs.ground.gui;

import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Font;
import java.awt.GridBagLayout;
import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;

import space.cougs.ground.CougSatGround;
import space.cougs.ground.gui.subsystems.modules.ImageModule;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;
import space.cougs.ground.gui.utils.GridBagConstraintsWrapper;

public class Home extends JPanel implements UIScaling {

	private static final long serialVersionUID = 1L;
	private static final int defaultHeight = 650;
	private static final int defaultWidth = 1200;
	private static final JLabel patchNotesHeaderText = new JLabel(
			"CIS Patch Notes " + CougSatGround.getVersionnumber());
	private static final JTextArea patchNotesBodyText = new JTextArea("");
	private static final JScrollPane scroll = new JScrollPane(patchNotesBodyText);
	private static final JTextArea aboutPanelBody = new JTextArea("");
	private static final JLabel aboutPanelHeader = new JLabel("About GroundStation");
	private static final JPanel optionsPanel = new JPanel();
	private static BufferedImage CISLogo;
	private static ImageModule logoPanel;

	public Home() {
		super();

		GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();
		gbc.setFill(GridBagConstraintsWrapper.BOTH);

		this.setLayout(new GridBagLayout());
		this.setBackground(CustomColors.BACKGROUND4);

		try {
			CISLogo = ImageIO.read(new File("resources/images/CISClubLogo.png"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		logoPanel = new ImageModule(CISLogo);

		// CIS Logo
		logoPanel.setBackground(CustomColors.BACKGROUND1);
		this.add(logoPanel, gbc.setLocation(0, 0).setSize(2, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));

		optionsPanel.setBackground(CustomColors.BACKGROUND1);
		this.add(optionsPanel, gbc.setLocation(3, 0).setSize(1, 5).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));

		// Information Panel

		this.add(aboutPanelHeader, gbc.setLocation(0, 1).setSize(2, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		this.add(aboutPanelBody, gbc.setLocation(0, 2).setSize(2, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));
		aboutPanelBody.setText("-Body-");

		// Patch Notes
		this.add(patchNotesHeaderText, gbc.setLocation(0, 3).setSize(2, 1).setWeight(1.0, 0.0).setInsets(5, 5, 5, 5));

		// patchNotesBodyText.append();

		File file = new File("resources/PatchNotes");

		BufferedReader br = null;
		try {
			br = new BufferedReader(new FileReader(file));
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		String tempLine;
		try {
			while ((tempLine = br.readLine()) != null) {
				patchNotesBodyText.append(tempLine + "\n");
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		patchNotesBodyText.append("test");
		patchNotesBodyText.setEditable(false);
		patchNotesBodyText.setVisible(true);

		scroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);

		this.add(scroll, gbc.setLocation(0, 4).setSize(2, 1).setWeight(1.0, 0.25).setInsets(5, 5, 5, 5));
		this.setBackground(CustomColors.BACKGROUND2);
		this.repaint();

	}

	@Override
	public void updateUIScaling(UIScale uiScale) {
		Font font = Fonts.BODY_24;

		switch (uiScale) {
		default:
		case SCALE_100:
			font = Fonts.BODY_24;
			break;
		case SCALE_150:
			font = Fonts.BODY_36;
			break;
		case SCALE_200:
			font = Fonts.BODY_48;
			break;
		case SCALE_300:
			font = Fonts.BODY_48;
			break;
		case SCALE_75:
			font = Fonts.BODY_16;
			break;
		}

		Font bodyfont = Fonts.BODY_16;

		switch (uiScale) {
		default:
		case SCALE_100:
			bodyfont = Fonts.BODY_16;
			break;
		case SCALE_150:
			bodyfont = Fonts.BODY_24;
			break;
		case SCALE_200:
			bodyfont = Fonts.BODY_32;
			break;
		case SCALE_300:
			bodyfont = Fonts.BODY_48;
			break;
		case SCALE_75:
			bodyfont = Fonts.BODY_12;
			break;
		}

		for (Component component : this.getComponents()) {
			if (component instanceof JLabel) {

				component.setFont(font);
				component.setBackground(CustomColors.BACKGROUND1);
				component.setForeground(Color.white);

			}
			if (component instanceof JTextArea) {
				component.setFont(bodyfont);
				component.setBackground(CustomColors.BACKGROUND1);
//				component.setForeground(Color.white);
			}
			for (Component subComponent : ((Container) component).getComponents()) {

				for (Component subsubComponent : ((Container) subComponent).getComponents()) {
					if (subsubComponent instanceof JTextArea) {

						subsubComponent.setFont(bodyfont);
						subsubComponent.setBackground(CustomColors.BACKGROUND1);
//						subsubComponent.setForeground(Color.white);
					}
				}
			}
		}
	}
}
