package space.cougs.ground.gui.modules;

import java.awt.Color;
import java.awt.Graphics;

import javax.swing.plaf.basic.BasicArrowButton;

import space.cougs.ground.gui.utils.CustomColors;

public class CISArrowButton extends BasicArrowButton {
  private static final long serialVersionUID = 1L;

  public CISArrowButton(int orientation) {
    super(orientation);
  }

  @Override
  public void paint(Graphics g) {
    int     width     = getSize().width;
    int     height    = getSize().height;
    Color   origColor = g.getColor();
    boolean isPressed = getModel().isPressed();

    if (isPressed) {
      g.setColor(CustomColors.PRIMARY_BUTTON_ACTIVE);
    } else {
      g.setColor(CustomColors.PRIMARY_BUTTON_INACTIVE);
    }
    g.fillRect(0, 0, width, height);

    if (isPressed) {
      g.translate(1, 1);
    }

    int size   = Math.min(height / 3, width / 3);
    size       = Math.max(size, 2);
    int mid    = size / 2;
    int offset = 0;

    g.setColor(CustomColors.PRIMARY_BUTTON_HIGHLIGHT);
    switch (direction) {
      case NORTH:
        mid    = width / 2;
        offset = (height - size) / 2;
        for (int i = 0; i < size; i++) {
          g.drawLine(mid - i, i + offset, mid + i, i + offset);
        }
        break;
      case SOUTH:
        mid    = width / 2;
        offset = (height - size) / 2 + size;
        for (int i = 0; i < size; i++) {
          g.drawLine(mid - i, offset - i, mid + i, offset - i);
        }
        break;
      case WEST:
        mid    = height / 2;
        offset = (width - size) / 2;
        for (int i = 0; i < size; i++) {
          g.drawLine(i + offset, mid - i, i + offset, mid + i);
        }
        break;
      case EAST:
        mid    = height / 2;
        offset = (width - size) / 2 + size;
        for (int i = 0; i < size; i++) {
          g.drawLine(offset - i, mid - i, offset - i, mid + i);
        }
        break;
    }

    if (isPressed) {
      g.translate(-1, -1);
    }
    g.setColor(origColor);
  }
}