package space.cougs.ground.gui.utils;

import java.awt.Container;
import java.util.List;

import javax.swing.JButton;

public final class SwingHelper {
  public static void addFromList(Container container, List<JButton> list) {
    for (JButton component : list) {
      container.add(component);
    }
  }
}