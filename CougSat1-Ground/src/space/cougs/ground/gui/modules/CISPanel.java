package space.cougs.ground.gui.modules;

import java.awt.Component;
import java.awt.LayoutManager;

import javax.swing.JPanel;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.utils.CustomColors;

public class CISPanel extends JPanel implements UIScaling {
  private static final long serialVersionUID = 1L;

  public CISPanel() {
    super();
    setDefaults();
  }

  public CISPanel(LayoutManager layout) {
    super(layout);
    setDefaults();
  }

  private void setDefaults() {
    this.setBackground(CustomColors.PRIMARY);
  }

  @Override
  public void updateUIScaling(UIScale uiScale) {
    for (Component child : this.getComponents()) {
      if (child instanceof UIScaling) {
        ((UIScaling)child).updateUIScaling(uiScale);
      }
    }
  }
}