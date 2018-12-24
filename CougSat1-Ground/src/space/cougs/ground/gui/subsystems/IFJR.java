package space.cougs.ground.gui.subsystems;

import javax.swing.BorderFactory;

import space.cougs.ground.gui.modules.CISPanel;
import space.cougs.ground.gui.utils.CustomColors;

public class IFJR extends CISPanel {
  private static final long serialVersionUID = 1L;

  public IFJR() {
    // TODO
    super();
    this.setBackground(CustomColors.SECONDARY);
    this.setBorder(BorderFactory.createLineBorder(CustomColors.PRIMARY, 10));
  }
}
