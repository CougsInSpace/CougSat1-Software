package space.cougs.ground.gui.modules;

import java.awt.Color;

import javax.swing.JTextField;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;

public class CISTextField extends JTextField implements UIScaling {
  private static final long serialVersionUID = 1L;

  private Color enabledBackground = null;

  public CISTextField() {
    super();
    this.setFont(Fonts.BODY_16);
    this.setForeground(CustomColors.PRIMARY_TEXT);
    this.setCaretColor(CustomColors.PRIMARY_TEXT);
    this.setBackground(CustomColors.PRIMARY);
  }

  @Override
  public void setBackground(Color bg) {
    super.setBackground(bg);
    this.enabledBackground = bg;
  }

  @Override
  public void setEnabled(boolean b) {
    super.setEnabled(b);
    if (b) {
      super.setBackground(enabledBackground);
    } else {
      super.setBackground(enabledBackground.darker());
    }
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
        System.out.println("Text field label \"" + this.getText() +
                           "\" unknown UI Scale: " + uiScale);
        break;
    }
  }
}