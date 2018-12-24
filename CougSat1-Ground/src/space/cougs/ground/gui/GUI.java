package space.cougs.ground.gui;

import java.awt.Dimension;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;

import javax.swing.JFrame;

import space.cougs.ground.CougSatGround;
import space.cougs.ground.gui.modules.CISTabbedPane;
import space.cougs.ground.gui.utils.Fonts;
import space.cougs.ground.satellites.CougSat;
import space.cougs.ground.satellites.CougSat1;
import space.cougs.ground.utils.FileUtils;

public class GUI implements UIScaling {
  private static final int defaultHeight = 650;
  private static final int defaultWidth  = 1200;

  private final JFrame mainFrame;
  private final CISTabbedPane mainPanel;

  private final Home home;
  private final CougSat1GUI cougSat1GUI;

  private UIScale currentUIScale = null;

  public GUI() {
    Fonts.loadFonts();

    mainFrame   = new JFrame();
    mainPanel   = new CISTabbedPane();
    home        = new Home();
    cougSat1GUI = new CougSat1GUI();

    mainPanel.setPreferredSize(new Dimension(defaultWidth, defaultHeight));
    mainPanel.setMinimumSize(new Dimension(defaultWidth, defaultHeight));

    mainPanel.addTab("     Home      ", home);
    mainPanel.addTab("   CougSat-1   ", cougSat1GUI);
    mainPanel.setSelectedComponent(cougSat1GUI);

    mainFrame.add(mainPanel);

    mainFrame.setIconImage(FileUtils.getImage(32, "rocket.png"));
    mainFrame.setTitle(
        "CougSat Ground Control v" + CougSatGround.getVersionnumber());
    mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    mainFrame.addComponentListener(componentListener);

    mainFrame.pack();
    mainFrame.setLocationRelativeTo(null); // Center on screen
    mainFrame.setVisible(true);
  }

  private final ComponentListener componentListener = new ComponentListener() {
    @Override
    public void componentResized(ComponentEvent e) {
      int height = mainPanel.getHeight();
      int width  = mainPanel.getWidth();

      if (height >= defaultHeight * 3 && width >= defaultWidth * 3) {
        updateUIScaling(UIScale.SCALE_300);
      } else if (height >= defaultHeight * 2 && width >= defaultWidth * 2) {
        updateUIScaling(UIScale.SCALE_200);
      } else if (height >= defaultHeight * 1.5 && width >= defaultWidth * 1.5) {
        updateUIScaling(UIScale.SCALE_150);
      } else if (height >= defaultHeight * 0.9 && width >= defaultWidth * 0.9) {
        updateUIScaling(UIScale.SCALE_100);
      } else {
        updateUIScaling(UIScale.SCALE_75);
      }
    }

    @Override
    public void componentHidden(ComponentEvent e) {}

    @Override
    public void componentMoved(ComponentEvent e) {}

    @Override
    public void componentShown(ComponentEvent e) {}
  };

  @Override
  public void updateUIScaling(UIScale uiScale) {
    if (uiScale == currentUIScale) {
      return;
    }
    currentUIScale = uiScale;
    mainPanel.updateUIScaling(uiScale);
    mainFrame.repaint();
  }

  public void updateSatellite(CougSat satellite) {
    if (satellite instanceof CougSat1) {
      cougSat1GUI.updateSatellite(satellite);
    }
  }
}
