package space.cougs.ground.gui.subsystems;

import javax.swing.BorderFactory;

import space.cougs.ground.gui.modules.CISPanel;
import space.cougs.ground.gui.utils.CustomColors;

public class Comms extends CISPanel {
  private static final long serialVersionUID = 1L;

  public Comms() {
    // TODO
    super();
    this.setBackground(CustomColors.SECONDARY);
    this.setBorder(BorderFactory.createLineBorder(CustomColors.PRIMARY, 10));
  }
}
