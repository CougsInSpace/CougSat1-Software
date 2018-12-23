package space.cougs.ground.gui;

import java.awt.Component;
import java.awt.Container;
import java.awt.Font;
import java.awt.GridBagLayout;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

import javax.swing.JCheckBox;
import javax.swing.JComponent;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.ScrollPaneConstants;
import javax.swing.SwingConstants;
import javax.swing.UIManager;

import space.cougs.ground.CougSatGround;
import space.cougs.ground.gui.subsystems.modules.ImageModule;
import space.cougs.ground.gui.subsystems.modules.TitleLabel;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;
import space.cougs.ground.gui.utils.GridBagConstraintsWrapper;
import space.cougs.ground.utils.FileUtils;

public class Home extends JPanel implements UIScaling {
  private static final long serialVersionUID = 1L;

  private final JPanel patchNotesPanel       = new JPanel();
  private final JTextArea patchNotesBody     = new JTextArea();
  private final JScrollPane patchNotesScroll = new JScrollPane(patchNotesBody);

  private final JPanel aboutPanel   = new JPanel();
  private final JTextArea aboutBody = new JTextArea();

  private final JPanel optionsPanel = new JPanel();

  private final JPanel filesAndDirectories = new JPanel();
  private final JTextField homeDirectory   = new JTextField();

  private final JPanel groundStationInfo         = new JPanel();
  private final JTextField groundstationName     = new JTextField();
  private final JTextField latitude              = new JTextField();
  private final JTextField longitude             = new JTextField();
  private final JTextField gridLocator           = new JTextField();
  private final JTextField altitude              = new JTextField();
  private final JTextField rfRecieverDescription = new JTextField();

  private final JPanel decoderPanel      = new JPanel();
  private final JCheckBox uploadToServer = new JCheckBox("Upload to Server");
  private final JCheckBox trackDoppler   = new JCheckBox("Track Doppler");
  private final JCheckBox storePayload   = new JCheckBox("Store Payload");
  private final JCheckBox swapIQ         = new JCheckBox("Swap I/Q");
  private final JCheckBox fixDroppedBits = new JCheckBox("Fix Dropped Bits");

  private final JPanel debugPanel       = new JPanel();
  private final JCheckBox enableLogging = new JCheckBox("Enable Logging");
  private final JCheckBox debugFrames   = new JCheckBox("Debug Frames");
  private final JCheckBox debugClock    = new JCheckBox("Debug Clock");
  private final JCheckBox debugAudio    = new JCheckBox("Debug Missed Audio");
  private final JCheckBox debugSignal   = new JCheckBox("Debug Find Signal");

  private ImageModule logoPanel;

  public Home() {
    super();

    GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();
    gbc.setInsets(5, 5, 5, 5);
    this.setLayout(new GridBagLayout());

    logoPanel = new ImageModule(FileUtils.getImage("CISClubLogo-1000.png"));
    logoPanel.setBackground(CustomColors.PRIMARY);

    aboutPanel.setLayout(new GridBagLayout());
    aboutPanel.setBackground(CustomColors.SECONDARY);
    aboutPanel.add(
        new TitleLabel("About"), gbc.setCommon(0, 1, 2, 1, 1.0, 0.0));
    aboutPanel.add(aboutBody, gbc.setCommon(0, 2, 2, 1, 1.0, 1.0));

    patchNotesPanel.setLayout(new GridBagLayout());
    patchNotesPanel.setBackground(CustomColors.SECONDARY);
    patchNotesPanel.add(
        new TitleLabel("Patch Notes"), gbc.setCommon(0, 3, 2, 1, 1.0, 0.0));
    patchNotesPanel.add(patchNotesScroll, gbc.setCommon(0, 4, 2, 1, 1.0, 1.0));

    layoutOptionPanel();

    this.add(logoPanel, gbc.setCommon(0, 0, 1, 1, 1.0, 1.0));
    this.add(aboutPanel, gbc.setCommon(0, 1, 1, 1, 0.0, 0.0));
    this.add(patchNotesPanel, gbc.setCommon(0, 2, 1, 1, 0.0, 1.0));
    this.add(optionsPanel, gbc.setCommon(1, 0, 1, 3));

    loadText();
    configureAppearance();

    this.setBackground(CustomColors.SECONDARY);
  }

  private void layoutOptionPanel() {
    GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();
    gbc.setInsets(5, 5, 5, 5).setWeight(1.0, 1.0);

    filesAndDirectories.setLayout(new GridBagLayout());
    filesAndDirectories.setBackground(CustomColors.PRIMARY);
    filesAndDirectories.add(new TitleLabel("Home Directory"), gbc.setXY(0, 0));
    filesAndDirectories.add(homeDirectory, gbc.setXY(0, 1));

    groundStationInfo.setLayout(new GridBagLayout());
    groundStationInfo.setBackground(CustomColors.PRIMARY);
    groundStationInfo.add(
        new TitleLabel("Ground Station Info", SwingConstants.CENTER),
        gbc.setCommon(0, 0, 2, 1, 0.0, 0.0));
    groundStationInfo.add(new JLabel("GroundStation Name: "),
        gbc.setCommon(0, 1, 1, 1, 0.0, 1.0));
    groundStationInfo.add(new JLabel("Longitude: "), gbc.setXY(0, 2));
    groundStationInfo.add(new JLabel("Latittude: "), gbc.setXY(0, 3));
    groundStationInfo.add(new JLabel("Grid Locator: "), gbc.setXY(0, 4));
    groundStationInfo.add(new JLabel("Altitude (m): "), gbc.setXY(0, 5));
    groundStationInfo.add(
        new JLabel("RF Radio Description: "), gbc.setXY(0, 6));
    groundStationInfo.add(
        groundstationName, gbc.setXY(1, 1).setWeight(1.0, 1.0));
    groundStationInfo.add(longitude, gbc.setXY(1, 2));
    groundStationInfo.add(latitude, gbc.setXY(1, 3));
    groundStationInfo.add(gridLocator, gbc.setXY(1, 4));
    groundStationInfo.add(altitude, gbc.setXY(1, 5));
    groundStationInfo.add(rfRecieverDescription, gbc.setXY(1, 6));

    decoderPanel.setLayout(new GridBagLayout());
    decoderPanel.setBackground(CustomColors.PRIMARY);
    decoderPanel.add(
        new TitleLabel("Decoder Options"), gbc.setXY(0, 0).setWeight(1.0, 0.0));
    decoderPanel.add(uploadToServer, gbc.setXY(0, 1).setWeight(1.0, 1.0));
    decoderPanel.add(trackDoppler, gbc.setXY(0, 2));
    decoderPanel.add(storePayload, gbc.setXY(0, 3));
    decoderPanel.add(swapIQ, gbc.setXY(0, 4));
    decoderPanel.add(fixDroppedBits, gbc.setXY(0, 5));

    debugPanel.setLayout(new GridBagLayout());
    debugPanel.setBackground(CustomColors.PRIMARY);
    debugPanel.add(
        new TitleLabel("Debug Options"), gbc.setXY(0, 0).setWeight(1.0, 0.0));
    debugPanel.add(enableLogging, gbc.setXY(0, 1).setWeight(1.0, 1.0));
    debugPanel.add(debugFrames, gbc.setXY(0, 2));
    debugPanel.add(debugClock, gbc.setXY(0, 3));
    debugPanel.add(debugAudio, gbc.setXY(0, 4));
    debugPanel.add(debugSignal, gbc.setXY(0, 5));

    optionsPanel.setLayout(new GridBagLayout());
    optionsPanel.setBackground(CustomColors.SECONDARY);
    optionsPanel.add(
        new TitleLabel("Options"), gbc.setCommon(0, 0, 2, 1, 1.0, 0.0));
    optionsPanel.add(filesAndDirectories, gbc.setCommon(0, 1, 2, 1, 1.0, 1.0));
    optionsPanel.add(groundStationInfo, gbc.setCommon(0, 2, 2, 1));
    optionsPanel.add(decoderPanel, gbc.setCommon(0, 3, 1, 1));
    optionsPanel.add(debugPanel, gbc.setCommon(1, 3, 1, 1));
  }

  private void loadText() {
    File aboutFile = new File("resources\\About.txt");
    if (!aboutFile.exists()) {
      System.out.printf(
          "About file does not exist: %s\n", aboutFile.getAbsolutePath());
    } else {
      try {
        BufferedReader fileIn = new BufferedReader(new FileReader(aboutFile));
        aboutBody.read(fileIn, null);
        fileIn.close();
      } catch (IOException e) {
        System.out.printf(
            "Failed to read about file: %s", aboutFile.getAbsolutePath());
        e.printStackTrace();
      }
    }
    File patchNotesFile = new File(
        "resources\\PatchNotes." + CougSatGround.getVersionnumber() + ".txt");
    if (!patchNotesFile.exists()) {
      System.out.printf("Patch notes file does not exist: %s\n",
          patchNotesFile.getAbsolutePath());
    } else {
      try {
        BufferedReader fileIn =
            new BufferedReader(new FileReader(patchNotesFile));
        patchNotesBody.read(fileIn, null);
        fileIn.close();
      } catch (IOException e) {
        System.out.printf(
            "Failed to read about file: %s", patchNotesFile.getAbsolutePath());
        e.printStackTrace();
      }
    }
    homeDirectory.setText(CougSatGround.getHomeDir().getAbsolutePath());
  }

  private void configureAppearance() {
    aboutBody.setEditable(false);
    aboutBody.setLineWrap(true);
    aboutBody.setWrapStyleWord(true);
    aboutBody.setBackground(CustomColors.PRIMARY);
    patchNotesBody.setEditable(false);
    patchNotesBody.setLineWrap(true);
    patchNotesBody.setWrapStyleWord(true);
    patchNotesBody.setBackground(CustomColors.PRIMARY);
    patchNotesScroll.setOpaque(false);
    patchNotesScroll.setVerticalScrollBarPolicy(
        ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
    patchNotesScroll.setHorizontalScrollBarPolicy(
        ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
    configureAppearance(this);
  }

  private void configureAppearance(Container container) {
    for (Component child : container.getComponents()) {
      if (child instanceof JLabel || child instanceof JCheckBox) {
        child.setForeground(CustomColors.PRIMARY_TEXT);
        ((JComponent)child).setOpaque(false);
      } else if (child instanceof JTextArea) {
        child.setForeground(CustomColors.PRIMARY_TEXT);
      } else if (child instanceof Container) {
        configureAppearance((Container)child);
      }
    }
  }

  private void configureAppearance(Container container, Font font) {
    for (Component child : container.getComponents()) {
      if ((child instanceof JLabel || child instanceof JCheckBox ||
              child instanceof JTextArea) &&
          !(child instanceof TitleLabel)) {
        child.setFont(font);
      } else if (child instanceof Container) {
        configureAppearance((Container)child, font);
      }
    }
  }

  @Override
  public void updateUIScaling(UIScale uiScale) {
    Font font = Fonts.BODY_16;

    int scrollBarSize = 20;

    switch (uiScale) {
      default:
      case SCALE_100:
        font          = Fonts.BODY_16;
        scrollBarSize = 20;
        break;
      case SCALE_150:
        font          = Fonts.BODY_24;
        scrollBarSize = 30;
        break;
      case SCALE_200:
        font          = Fonts.BODY_32;
        scrollBarSize = 40;
        break;
      case SCALE_300:
        font          = Fonts.BODY_48;
        scrollBarSize = 60;
        break;
      case SCALE_75:
        font          = Fonts.BODY_12;
        scrollBarSize = 15;
        break;
    }

    aboutBody.setFont(font);
    patchNotesBody.setFont(font);
    UIManager.put("ScrollBar.width", scrollBarSize);
    patchNotesScroll.setVerticalScrollBar(
        patchNotesScroll.createVerticalScrollBar());
    configureAppearance(this, font);
  }
}
