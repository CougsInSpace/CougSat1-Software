package space.cougs.ground.gui.modules;

import java.awt.Component;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Insets;
import java.awt.Rectangle;

import javax.swing.JComponent;
import javax.swing.JTabbedPane;
import javax.swing.plaf.basic.BasicTabbedPaneUI;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;

public class CISTabbedPane extends JTabbedPane implements UIScaling {
  private static final long serialVersionUID = 1L;

  BasicTabbedPaneUI ui = new BasicTabbedPaneUI() {
    @Override
    protected void installDefaults() {
      super.installDefaults();
      contentBorderInsets = new Insets(1, 0, 0, 0);
    }

    @Override
    protected void paintTab(Graphics g, int tabPlacement, Rectangle[] rects,
        int tabIndex, Rectangle iconRect, Rectangle textRect) {
      Rectangle tabRect       = rects[tabIndex];
      int       selectedIndex = tabPane.getSelectedIndex();
      boolean   isSelected    = selectedIndex == tabIndex;
      g.setColor(tabPane.getBackgroundAt(tabIndex));
      g.fillRect(tabRect.x, tabRect.y + 2, tabRect.width, tabRect.height - 2);

      if(!isSelected && tabIndex != selectedIndex - 1){
        g.setColor(tabPane.getForeground());
        int x = tabRect.x + tabRect.width - 1;
        int y1 = tabRect.y + 4 + contentBorderInsets.top;
        int y2 = tabRect.y + tabRect.height - 4;
        g.drawLine(x, y1, x, y2);
      }

      String      title   = tabPane.getTitleAt(tabIndex);
      Font        font    = tabPane.getFont();
      FontMetrics metrics = tabPane.getFontMetrics(font);

      layoutLabel(tabPlacement, metrics, tabIndex, title, null, tabRect,
          iconRect, textRect, isSelected);
      paintText(g, tabPlacement, font, metrics, tabIndex, title, textRect,
          isSelected);
    }

    @Override
    public void paint(Graphics g, JComponent c) {
      int selectedIndex = tabPane.getSelectedIndex();
      int tabPlacement  = tabPane.getTabPlacement();
      if (!tabPane.isValid()) {
        tabPane.validate();
      }

      if (!tabPane.isValid()) {
        TabbedPaneLayout layout = (TabbedPaneLayout)tabPane.getLayout();
        layout.calculateLayoutInfo();
      }

      g.setColor(tabPane.getBackground());
      g.fillRect(0, 0, tabPane.getWidth(), tabPane.getHeight());

      paintTabArea(g, tabPlacement, selectedIndex);

      g.setColor(tabPane.getBackgroundAt(selectedIndex));
      int height = tabPane.getInsets().top;
      height += calculateTabAreaHeight(tabPlacement, runCount, maxTabHeight);
      Rectangle selectedTab = rects[selectedIndex];

      int size = selectedTab.height / 4;
      int x    = 0;
      int y    = height;
      for (int i = 0; i < size; i++) {
        x = selectedTab.x - size + i;
        g.fillRect(x, y, size - i, 1);
        x = selectedTab.x + selectedTab.width;
        g.fillRect(x, y, size - i, 1);
        y--;
      }

      g.setColor(g.getColor().brighter());
      int right = selectedTab.x + selectedTab.width;
      g.drawLine(0, height, selectedTab.x - size, height);
      g.drawLine(selectedTab.x - size, height, selectedTab.x, height - size);
      g.drawLine(selectedTab.x, 2, selectedTab.x,
          (selectedIndex == 0) ? height : height - size);
      g.drawLine(selectedTab.x, 2, right, 2);
      g.drawLine(right, 2, right, height - size);
      g.drawLine(right, height - size, right + size, height);
      g.drawLine(right + size, height, tabPane.getWidth(), height);
    }
  };

  public CISTabbedPane() {
    super();
    this.setBackground(CustomColors.SECONDARY);
    this.setForeground(CustomColors.SECONDARY_TEXT);
    this.setUI(ui);
  }

  @Override
  public void addTab(String title, Component component) {
    super.addTab(title, component);
    this.setBackgroundAt(
        this.indexOfComponent(component), component.getBackground());
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