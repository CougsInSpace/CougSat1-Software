package space.cougs.ground.gui.modules;

import java.awt.Component;

import javax.swing.JScrollPane;
import javax.swing.ScrollPaneConstants;
import javax.swing.UIManager;

import space.cougs.ground.gui.UIScaling;

public class CISScrollPane extends JScrollPane implements UIScaling {
  private static final long serialVersionUID = 1L;

  public CISScrollPane() {
    super();
    setDefaults();
  }

  public CISScrollPane(Component view) {
    super(view);
    setDefaults();
  }

  private void setDefaults() {
    this.setOpaque(false);
    this.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
    this.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
  }

  @Override
  public void updateUIScaling(UIScale uiScale) {
    int scrollBarSize = 20;

    switch (uiScale) {
    case SCALE_100:
      scrollBarSize = 20;
      break;
    case SCALE_150:
      scrollBarSize = 30;
      break;
    case SCALE_200:
      scrollBarSize = 40;
      break;
    case SCALE_300:
      scrollBarSize = 60;
      break;
    case SCALE_75:
      scrollBarSize = 15;
      break;
    default:
      System.out.println("Home unknown UIscale: " + uiScale);
      break;
    }

    UIManager.put("ScrollBar.width", scrollBarSize);
    this.setVerticalScrollBar(this.createVerticalScrollBar());
  }
}