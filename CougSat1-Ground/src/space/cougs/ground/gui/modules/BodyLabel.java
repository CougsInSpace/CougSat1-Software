package space.cougs.ground.gui.modules;

import javax.swing.Icon;
import javax.swing.JLabel;
import javax.swing.SwingConstants;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;

public class BodyLabel extends JLabel implements UIScaling {
  private static final long serialVersionUID = 1L;

  public BodyLabel() {
    super();
    setDefaults();
  }

  public BodyLabel(String text) {
    super(text, SwingConstants.CENTER);
    setDefaults();
  }

  public BodyLabel(Icon image) {
    super(image);
    setDefaults();
  }

  public BodyLabel(String text, int horizontalAlignment) {
    super(text, horizontalAlignment);
    setDefaults();
  }

  public BodyLabel(Icon image, int horizontalAlignment) {
    super(image, horizontalAlignment);
    setDefaults();
  }

  public BodyLabel(String text, Icon icon, int horizontalAlignment) {
    super(text, icon, horizontalAlignment);
    setDefaults();
  }

  private void setDefaults() {
    this.setFont(Fonts.BODY_16);
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
        System.out.println("Body label \"" + this.getText() +
                           "\" unknown UI Scale: " + uiScale);
        break;
    }
  }
}
