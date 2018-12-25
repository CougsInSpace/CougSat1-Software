package space.cougs.ground.gui.modules;

import java.awt.Component;

import javax.swing.BorderFactory;
import javax.swing.JScrollBar;
import javax.swing.JScrollPane;
import javax.swing.ScrollPaneConstants;
import javax.swing.plaf.basic.BasicScrollPaneUI;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.utils.CustomColors;

public class CISScrollPane extends JScrollPane implements UIScaling {
  private static final long serialVersionUID = 1L;

  public CISScrollPane(Component view) {
    super(view);
    this.setOpaque(false);
    this.setVerticalScrollBarPolicy(
        ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
    this.setHorizontalScrollBarPolicy(
        ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
    this.setBorder(BorderFactory.createLineBorder(CustomColors.PRIMARY.brighter(), 2));
  }

  @Override
  public JScrollBar createVerticalScrollBar() {
    return new CISScrollBar();
  }

  @Override
  public JScrollBar createHorizontalScrollBar() {
    return new CISScrollBar(CISScrollBar.HORIZONTAL);
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