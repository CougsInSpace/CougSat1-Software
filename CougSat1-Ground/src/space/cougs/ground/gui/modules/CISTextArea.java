package space.cougs.ground.gui.modules;

import javax.swing.JTextArea;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;

public class CISTextArea extends JTextArea implements UIScaling {
  private static final long serialVersionUID = 1L;

  public CISTextArea() {
    this(0, 0);
  }

  public CISTextArea(int rows, int columns) {
    super(rows, columns);
    this.setFont(Fonts.BODY_16);
    this.setEditable(false);
    this.setLineWrap(true);
    this.setWrapStyleWord(true);
    this.setBackground(CustomColors.PRIMARY);
    this.setForeground(CustomColors.PRIMARY_TEXT);
  }

  @Override
  public void updateUIScaling(UIScale uiScale) {
    switch (uiScale) {
      case SCALE_100:
        this.setFont(Fonts.BODY_16);
        break;
      case SCALE_150:
        this.setFont(Fonts.BODY_24);
        break;
      case SCALE_200:
        this.setFont(Fonts.BODY_32);
        break;
      case SCALE_300:
        this.setFont(Fonts.BODY_48);
        break;
      case SCALE_75:
        this.setFont(Fonts.BODY_12);
        break;
      default:
        System.out.println("Text Area of " + this.getParent().getName() +
                           " UI Scale: " + uiScale);
        break;
    }
  }
}