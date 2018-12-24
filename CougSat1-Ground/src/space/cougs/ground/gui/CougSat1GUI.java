package space.cougs.ground.gui;

import java.awt.BorderLayout;
import java.awt.CardLayout;
import java.awt.Component;
import java.awt.Font;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.List;

import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.SwingConstants;

import space.cougs.ground.gui.modules.CISPanel;
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
import space.cougs.ground.gui.utils.SwingHelper;
import space.cougs.ground.satellites.CougSat;
import space.cougs.ground.utils.FileUtils;

class CougSat1GUI extends CISPanel implements SatelliteInfo {
  private static final long serialVersionUID = 1L;

  private final Health health = new Health();
  // private final ADCS adcs = new ADCS();
  // private final IFJR ifjr = new IFJR();
  // private final CDH cdh = new CDH();
  // private final Comms comms = new Comms();
  // private final Camera camera = new Camera();
  // private final Plant plant = new Plant();
  // private final EPS eps = new EPS();

  private final CardLayout subSystemLayout = new CardLayout();
  private final CISPanel subSystemWrapper    = new CISPanel(subSystemLayout);

  private final CardSwitcher cardSwitcher = new CardSwitcher();

  CougSat1GUI() {
    super();

    subSystemWrapper.add(health, "Health");
    // subSystemWrapper.add(adcs, "Attitude");
    // subSystemWrapper.add(ifjr, "IFJR");
    // subSystemWrapper.add(cdh, "Computer");
    // subSystemWrapper.add(comms, "Radio");
    // subSystemWrapper.add(camera, "Camera");
    // subSystemWrapper.add(plant, "Plant");
    // subSystemWrapper.add(eps, "Power");
    subSystemWrapper.setOpaque(false);

    this.setLayout(new BorderLayout());
    this.add(subSystemWrapper, BorderLayout.CENTER);
    this.add(cardSwitcher, BorderLayout.LINE_START);
  }

  private class CardSwitcher
      extends JPanel implements ActionListener, UIScaling {
    private static final long serialVersionUID = 1L;

    private final JButton healthButton  = new JButton("Health");
    private final JButton adcsButton    = new JButton("Attitude");
    private final JButton ifjrButton    = new JButton("IFJR");
    private final JButton cdhButton     = new JButton("Computer");
    private final JButton commsButton   = new JButton("Radio");
    private final JButton cameraButton  = new JButton("Camera");
    private final JButton plantButton   = new JButton("Plant");
    private final JButton epsButton     = new JButton("Power");
    private final List<JButton> buttons = new ArrayList<JButton>();

    private CardSwitcher() {
      this.setOpaque(false);
      this.setLayout(new GridLayout(0, 1, 10, 10));
      this.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 0));

      buttons.add(healthButton);
      buttons.add(healthButton);
      buttons.add(adcsButton);
      buttons.add(epsButton);
      buttons.add(ifjrButton);
      buttons.add(cdhButton);
      buttons.add(commsButton);
      buttons.add(cameraButton);
      buttons.add(plantButton);

      SwingHelper.addFromList(this, buttons);

      for (JButton button : buttons) {
        button.setHorizontalAlignment(SwingConstants.LEFT);
        button.addActionListener(this);
        button.setBorder(BorderFactory.createEmptyBorder(5, 15, 5, 15));
        button.setForeground(CustomColors.PRIMARY_TEXT);
        button.setIconTextGap(15);
        button.setFocusable(false);
      }

      healthButton.doClick();
    }

    @Override
    public void updateUIScaling(UIScale uiScale) {
      Font font     = Fonts.BODY_24;
      int  iconSize = 48;

      switch (uiScale) {
        case SCALE_100:
          font     = Fonts.BODY_24;
          iconSize = 48;
          break;
        case SCALE_150:
          font     = Fonts.BODY_36;
          iconSize = 64;
          break;
        case SCALE_200:
          font     = Fonts.BODY_48;
          iconSize = 128;
          break;
        case SCALE_300:
          font     = Fonts.BODY_48;
          iconSize = 128;
          break;
        case SCALE_75:
          font     = Fonts.BODY_16;
          iconSize = 32;
          break;
        default:
          System.out.println("Card Switcher unknown UIscale: " + uiScale);
          break;
      }

      for (JButton button : buttons) {
        button.setFont(font);
        button.setIcon(new ImageIcon(
            FileUtils.getImage(iconSize, button.getText() + ".png")));
      }
    }

    @Override
    public void actionPerformed(ActionEvent evt) {
      subSystemLayout.show(subSystemWrapper, evt.getActionCommand());

      for (JButton button : buttons) {
        button.setBackground(CustomColors.PRIMARY_BUTTON_INACTIVE);
      }
      ((JButton)evt.getSource())
          .setBackground(CustomColors.PRIMARY_BUTTON_ACTIVE);
    }
  }

  @Override
  public void updateSatellite(CougSat satellite) {
    for (Component component : subSystemWrapper.getComponents()) {
      if (component instanceof SatelliteInfo) {
        ((SatelliteInfo)component).updateSatellite(satellite);
      }
    }
  }
}
