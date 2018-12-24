package space.cougs.ground.gui.modules;

import javax.swing.JButton;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;

public class CISButton extends JButton implements UIScaling {
  private static final long serialVersionUID = 1L;

  public CISButton() {
    super();
    setDefaults();
  }

  public CISButton(String text) {
    super(text);
    setDefaults();
  }

  private void setDefaults() {
    this.setFont(Fonts.BODY_16);
    this.setBackground(CustomColors.PRIMARY_BUTTON_INACTIVE);
    this.setForeground(CustomColors.PRIMARY_TEXT);
    this.setFocusable(false);
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
        System.out.println("Body label \"" + this.getText() +
                           "\" unkmown UI Scale: " + uiScale);
        break;
    }
  }
}