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
import javax.swing.JCheckBox;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.SwingConstants;
import javax.swing.UIManager;
import javax.swing.text.JTextComponent;

import space.cougs.ground.CougSatGround;
import space.cougs.ground.gui.subsystems.modules.ImageModule;
import space.cougs.ground.gui.subsystems.modules.TitleLabel;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;
import space.cougs.ground.gui.utils.GridBagConstraintsWrapper;

public class Home extends JPanel implements UIScaling {

	private static final long serialVersionUID = 1L;

	private static final JPanel patchNotesPanel = new JPanel();
	private static final TitleLabel patchNotesHeaderText = new TitleLabel(
			"CIS Patch Notes " + CougSatGround.getVersionnumber());
	private static final JTextArea patchNotesBodyText = new JTextArea("");
	private static final JScrollPane patchNotesScroll = new JScrollPane(patchNotesBodyText);

	private static final JPanel aboutPanel = new JPanel();
	private static final TitleLabel aboutPanelHeader = new TitleLabel("About GroundStation", SwingConstants.CENTER);
	private static final JTextArea aboutPanelBody = new JTextArea("");
	private static final JScrollPane aboutPanelScroll = new JScrollPane(aboutPanelBody);

	private static final JPanel optionsPanel = new JPanel();
	private static final JPanel filesAndDirectories = new JPanel();
	private static final JTextField homeDirectory = new JTextField(System.getProperty("user.dir"));
	private static final JPanel groundStationParams = new JPanel();
	private static final JTextField groundstationName = new JTextField("Enter GroundStation Name");
	private static final JTextField latittude = new JTextField();
	private static final JTextField longitude = new JTextField();
	private static final JTextField latLongLocator = new JTextField();
	private static final JTextField altitude = new JTextField();
	private static final JTextField rfRecieverDescription = new JTextField();

	private static final JPanel decoderPanel = new JPanel();
	private static final JCheckBox uploadToServer = new JCheckBox("Upload to Server");
	private static final JCheckBox trackDoppler = new JCheckBox("Track Doppler");
	private static final JCheckBox storePayload = new JCheckBox("Store Payload");
	private static final JCheckBox leftSteroChannel = new JCheckBox("Use Left Stero Channel");
	private static final JCheckBox swapIQ = new JCheckBox("Swap Iq");
	private static final JCheckBox fixDroppedBits = new JCheckBox("Fix Dropped Bits");

	private static final JPanel debugPanel = new JPanel();
	private static final JCheckBox enableLogging = new JCheckBox("Enable Logging");
	private static final JCheckBox debugFrames = new JCheckBox("Debug Frames");
	private static final JCheckBox debugFields = new JCheckBox("Debug Fields");
	private static final JCheckBox debugValues = new JCheckBox("Debug Values");
	private static final JCheckBox debugClock = new JCheckBox("Debug Clock");
	private static final JCheckBox debugAudio = new JCheckBox("Debug Missed Audio");
	private static final JCheckBox debugSignal = new JCheckBox("Debug Find Signal");

	private static BufferedImage CISLogo;
	private static ImageModule logoPanel;

	public Home() {
		super();

		GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();
		gbc.setFill(GridBagConstraintsWrapper.BOTH);

		this.setLayout(new GridBagLayout());

		// CIS Logo
		try {
			CISLogo = ImageIO.read(new File("resources/images/CISClubLogo.png"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		logoPanel = new ImageModule(CISLogo);
		logoPanel.setBackground(CustomColors.BACKGROUND1);
		this.add(logoPanel, gbc.setLocation(0, 0).setSize(1, 1).setWeight(1.0, 2.0).setInsets(5, 5, 5, 5));

		// options Panel
		optionsPanel.setLayout(new GridBagLayout());
		optionsPanel.setBackground(CustomColors.BACKGROUND2);
		optionsPanel.add(new TitleLabel("Options"), gbc.setLocation(0, 0).setSize(3, 1).setWeight(0.0, 0.0));

		filesAndDirectories.setLayout(new GridBagLayout());
		filesAndDirectories.setBackground(CustomColors.BACKGROUND1);
		filesAndDirectories.add(new TitleLabel("Files and Directories"), gbc.setLocation(1, 0).setSize(3, 1));
		filesAndDirectories.add(new JLabel("Home Directory"), gbc.setLocation(0, 2).setSize(1, 1));

		filesAndDirectories.add(homeDirectory, gbc.setLocation(1, 3).setSize(1, 1).setWeight(1.0, 0.0));
		filesAndDirectories.add(new JLabel("Log Files Directory:"),
				gbc.setLocation(0, 3).setSize(1, 1).setWeight(0.0, 0.0));
		filesAndDirectories.add(new JLabel(System.getProperty("user.dir")), gbc.setLocation(1, 2).setSize(2, 1));
		optionsPanel.add(filesAndDirectories, gbc.setLocation(0, 1).setSize(2, 1));

		groundStationParams.setLayout(new GridBagLayout());
		groundStationParams.setBackground(CustomColors.BACKGROUND1);
		groundStationParams.add(new TitleLabel("Ground Station Params", SwingConstants.CENTER),
				gbc.setLocation(0, 0).setSize(2, 1).setWeight(1.0, 0.0));
		groundStationParams.add(new JLabel("GroundStation Name: "), gbc.setLocation(0, 1).setSize(1, 1));
		groundStationParams.add(new JLabel("Longitude: "), gbc.setLocation(0, 2));
		groundStationParams.add(new JLabel("Latittude:"), gbc.setLocation(0, 3));
		groundStationParams.add(groundstationName, gbc.setLocation(1, 1));
		groundStationParams.add(longitude, gbc.setLocation(1, 2));
		groundStationParams.add(latittude, gbc.setLocation(1, 3));
		groundStationParams.add(new JLabel("Lat Long gives Locator:"), gbc.setLocation(0, 4));
		groundStationParams.add(latLongLocator, gbc.setLocation(1, 4).setWeight(0.0, 0.0));
		groundStationParams.add(new JLabel("Altitude(m):"), gbc.setLocation(0, 5));
		groundStationParams.add(altitude, gbc.setLocation(1, 5));
		groundStationParams.add(new JLabel("Rf-Reciever Description"), gbc.setLocation(0, 6));
		groundStationParams.add(rfRecieverDescription, gbc.setLocation(1, 6));

		optionsPanel.add(groundStationParams, gbc.setLocation(0, 2).setSize(3, 1).setWeight(0.0, 0.0));

		decoderPanel.setLayout(new GridBagLayout());
		decoderPanel.setBackground(CustomColors.BACKGROUND1);
		decoderPanel.add(new TitleLabel("Decoder Options"), gbc.setLocation(0, 0).setSize(1, 1));
		decoderPanel.add(uploadToServer, gbc.setLocation(0, 1));
		decoderPanel.add(trackDoppler, gbc.setLocation(0, 2));
		decoderPanel.add(storePayload, gbc.setLocation(0, 3));
		decoderPanel.add(leftSteroChannel, gbc.setLocation(0, 4));
		decoderPanel.add(swapIQ, gbc.setLocation(0, 5));
		decoderPanel.add(fixDroppedBits, gbc.setLocation(0, 6));
		optionsPanel.add(decoderPanel, gbc.setLocation(0, 3).setWeight(1.0, 0.0));

		debugPanel.setLayout(new GridBagLayout());
		debugPanel.setBackground(CustomColors.BACKGROUND1);
		debugPanel.add(new TitleLabel("Debug Options"), gbc.setLocation(0, 0).setWeight(0.0, 0.0));
		debugPanel.add(enableLogging, gbc.setLocation(0, 1));
		debugPanel.add(debugFrames, gbc.setLocation(0, 2));
		debugPanel.add(debugFields, gbc.setLocation(0, 3));
		debugPanel.add(debugValues, gbc.setLocation(0, 4));
		debugPanel.add(debugClock, gbc.setLocation(0, 5));
		debugPanel.add(debugAudio, gbc.setLocation(0, 6));
		debugPanel.add(debugSignal, gbc.setLocation(0, 7));
		optionsPanel.add(debugPanel, gbc.setLocation(1, 3).setSize(1, 1).setWeight(1.0, 0.0));

		this.add(optionsPanel, gbc.setLocation(2, 0).setSize(1, 5).setWeight(0.0, 1.0).setInsets(5, 5, 5, 5));

		// Information Panel
		aboutPanel.setLayout(new GridBagLayout());
		aboutPanel.setBackground(CustomColors.BACKGROUND2);
		aboutPanel.add(aboutPanelHeader, gbc.setLocation(0, 1).setSize(2, 1).setWeight(1.0, 0.0));
		aboutPanel.add(aboutPanelScroll, gbc.setLocation(0, 2).setSize(2, 1).setWeight(1.0, 1.0));
		aboutPanelBody.setText("body");
		this.add(aboutPanel, gbc.setLocation(0, 1).setSize(1, 1).setWeight(1.0, 1.0));

		// Patch Notes
		patchNotesPanel.setLayout(new GridBagLayout());
		patchNotesPanel.setBackground(CustomColors.BACKGROUND2);
		patchNotesPanel.add(patchNotesHeaderText, gbc.setLocation(0, 3).setSize(2, 1).setWeight(1.0, 0.0));

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

		patchNotesPanel.add(patchNotesScroll,
				gbc.setLocation(0, 4).setSize(2, 1).setWeight(1.0, 1.0));
		this.add(patchNotesPanel, gbc.setLocation(0, 3).setSize(1, 1).setWeight(1.0, 1.0));

		this.setBackground(CustomColors.BACKGROUND2);
		this.repaint();

	}

	@Override
	public void updateUIScaling(UIScale uiScale) {
		Font font = Fonts.BODY_24;
		Font bodyfont = Fonts.BODY_16;
		int scrollBarSize = 20;

		switch (uiScale) {
		default:
		case SCALE_100:
			font = Fonts.BODY_24;
			bodyfont = Fonts.BODY_16;
			scrollBarSize = 20;
			break;
		case SCALE_150:
			font = Fonts.BODY_36;
			bodyfont = Fonts.BODY_24;
			scrollBarSize = 30;
			break;
		case SCALE_200:
			font = Fonts.BODY_48;
			bodyfont = Fonts.BODY_32;
			scrollBarSize = 40;
			break;
		case SCALE_300:
			font = Fonts.BODY_48;
			bodyfont = Fonts.BODY_48;
			scrollBarSize = 60;
			break;
		case SCALE_75:
			font = Fonts.BODY_16;
			bodyfont = Fonts.BODY_12;
			scrollBarSize = 15;
			break;
		}

		UIManager.put("ScrollBar.width", scrollBarSize);
		patchNotesScroll.setVerticalScrollBar(patchNotesScroll.createVerticalScrollBar());
		patchNotesScroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);

		for (Component component : this.getComponents()) {
			if (component instanceof TitleLabel) {
				component.setFont(font);
				component.setBackground(CustomColors.BACKGROUND1);
				component.setForeground(Color.white);

			} else if (component instanceof JTextArea) {
				component.setFont(bodyfont);
				component.setBackground(CustomColors.BACKGROUND1);
				component.setBackground(Color.red);
			}
			for (Component subComponent : ((Container) component).getComponents()) {
				if (subComponent instanceof TitleLabel) {
					subComponent.setFont(font);
					subComponent.setBackground(CustomColors.BACKGROUND1);
					subComponent.setForeground(Color.white);
				} else if (subComponent instanceof JLabel) {
					subComponent.setFont(bodyfont);
				} else if (subComponent instanceof JTextArea) {
					subComponent.setFont(bodyfont);
					subComponent.setBackground(CustomColors.BACKGROUND1);
					((JTextComponent) subComponent).setEditable(false);
					subComponent.setVisible(true);
				}
				for (Component subsubComponent : ((Container) subComponent).getComponents()) {
					if (subsubComponent instanceof TitleLabel) {
						subsubComponent.setFont(font);
						subsubComponent.setBackground(CustomColors.BACKGROUND1);
						subsubComponent.setForeground(Color.white);
					} else if (subsubComponent instanceof JLabel) {
						subsubComponent.setFont(bodyfont);
					} else if (subsubComponent instanceof JTextField) {
						subsubComponent.setFont(bodyfont);
					} else if (subsubComponent instanceof JCheckBox) {
						subsubComponent.setFont(bodyfont);
						subsubComponent.setBackground(CustomColors.BACKGROUND1);
					}
					for (Component subsubsubComponent : ((Container) subsubComponent).getComponents()) {
						if (subsubsubComponent instanceof JTextArea) {
							subsubsubComponent.setFont(bodyfont);
							subsubsubComponent.setBackground(CustomColors.BACKGROUND1);
							((JTextComponent) subsubsubComponent).setEditable(false);
							subsubsubComponent.setVisible(true);
						}
					}
				}
			}
		}
	}
}
