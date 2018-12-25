package space.cougs.ground.gui.modules;

import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Insets;
import java.awt.RenderingHints;

import javax.swing.AbstractButton;
import javax.swing.ButtonModel;
import javax.swing.JCheckBox;
import javax.swing.JComponent;
import javax.swing.plaf.ButtonUI;
import javax.swing.plaf.basic.BasicCheckBoxUI;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;

public class CISCheckBox extends JCheckBox implements UIScaling {
  private static final long serialVersionUID = 1L;

  private ButtonUI ui = new BasicCheckBoxUI() {
    @Override
    public synchronized void paint(Graphics g, JComponent component) {
      AbstractButton button = (AbstractButton)component;
      ButtonModel    model  = button.getModel();

      Graphics2D g2d = (Graphics2D)g;
      g2d.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,
          RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

      Font font = component.getFont();
      g2d.setFont(font);
      FontMetrics fontMetrics = component.getFontMetrics(font);

      Insets insets = component.getInsets();
      int    width  = button.getWidth();
      int    height = button.getHeight();

      int x = height + insets.left;
      int y = (height - fontMetrics.getHeight()) / 2 + fontMetrics.getAscent();
      g2d.setColor(button.getForeground());
      g2d.drawString(button.getText(), x, y);

      int boxSize = height / 2;

      y = (height - boxSize) / 2;
      x = (x - boxSize + insets.left) / 2;

      if(model.isPressed()){
        g2d.translate(1, 1);
      }

      if (model.isEnabled()) {
        g2d.setColor(button.getBackground());
      }else{        
        g2d.setColor(button.getBackground().darker());
      }
      if(model.isSelected()){
        g2d.fillRect(x, y, boxSize, boxSize);
      }

      if (model.isEnabled()) {
        g2d.setColor(button.getForeground());
      }else{        
        g2d.setColor(button.getForeground().darker());
      }
      g2d.drawRect(x, y, boxSize, boxSize);
      
      if(model.isPressed()){
        g2d.translate(-1, -1);
      }

    }
  };

  public CISCheckBox() {
    this("");
  }

  public CISCheckBox(String text) {
    super(text);
    this.setFont(Fonts.BODY_16);
    this.setBackground(CustomColors.STATUS_GOOD);
    this.setForeground(CustomColors.PRIMARY_TEXT);
    this.setOpaque(false);
    this.setUI(ui);
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