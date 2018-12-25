package space.cougs.ground.gui.subsystems;

import java.awt.BorderLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.LinkedList;
import java.util.List;

import javax.swing.Timer;

import space.cougs.ground.gui.modules.CISPanel;
import space.cougs.ground.gui.modules.CISTabbedPane;
import space.cougs.ground.gui.utils.AnimationComponent;
import space.cougs.ground.satellites.CougSat;

public class EPS extends CISPanel implements SatelliteInfo {
  private static final long serialVersionUID = 1L;

  private final CISTabbedPane mainPowerPanel = new CISTabbedPane();
  private final CISPanel powerGeneration     = new CISPanel();
  private final CISPanel powerDistribution   = new CISPanel();

  private final List<AnimationComponent> animationComponents =
      new LinkedList<AnimationComponent>();
  private final double frameDuration = 0.1;

  private final ActionListener timerListener = new ActionListener() {
    @Override
    public void actionPerformed(ActionEvent e) {
      for (AnimationComponent animationComponent : animationComponents) {
        animationComponent.updateFrame(frameDuration);
      }
    }
  };

  private final Timer timer =
      new Timer((int)(frameDuration * 1000), timerListener);

  public EPS() {
    super();

    mainPowerPanel.addTab("   Generation  ", powerGeneration);
    mainPowerPanel.addTab("   Distribution   ", powerDistribution);
    mainPowerPanel.setSelectedComponent(powerGeneration);

    this.setLayout(new BorderLayout());
    this.add(mainPowerPanel);
    timer.start();
  }

  @Override
  public void updateSatellite(CougSat satellite) {}
}
