package space.cougs.ground.gui.modules;

import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;

import javax.swing.JComponent;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;

public class AttitudeIndicator extends JComponent implements UIScaling {
  private static final long serialVersionUID = 1L;

  public AttitudeIndicator() {
    super();
    this.setBackground(CustomColors.PRIMARY);
  }

  @Override
  public Dimension getPreferredSize() {
    return new Dimension();
  }

  @Override
  public Dimension getMinimumSize() {
    return new Dimension();
  }

  @Override
  protected void paintComponent(Graphics g) {
    super.paintComponent(g);
    Graphics2D g2d = (Graphics2D) g;
    g2d.setColor(this.getBackground());
    g2d.fillRect(0, 0, this.getWidth(), this.getHeight());
  }

  @Override
  public void updateUIScaling(UIScale uiScale) {
    this.setFont(Fonts.BODY_16);

    switch (uiScale) {
    default:
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
    }
  }
}
