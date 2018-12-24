package space.cougs.ground.gui.modules;

import java.awt.Component;

import javax.swing.JTabbedPane;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;

public class CISTabbedPane extends JTabbedPane implements UIScaling {
  private static final long serialVersionUID = 1L;

  public CISTabbedPane() {
    super();
    this.setBackground(CustomColors.PRIMARY);
  }

  @Override
  public void updateUIScaling(UIScale uiScale) {
    switch (uiScale) {
      case SCALE_100:
        this.setFont(Fonts.TITLE_16);
        break;
      case SCALE_150:
        this.setFont(Fonts.TITLE_24);
        break;
      case SCALE_200:
        this.setFont(Fonts.TITLE_32);
        break;
      case SCALE_300:
        this.setFont(Fonts.TITLE_48);
        break;
      case SCALE_75:
        this.setFont(Fonts.TITLE_12);
        break;
      default:
        System.out.println("Tabbed Pane unknown UIscale: " + uiScale);
        break;
    }
    for (Component child : this.getComponents()) {
      if (child instanceof UIScaling) {
        ((UIScaling)child).updateUIScaling(uiScale);
      }
    }
  }
}