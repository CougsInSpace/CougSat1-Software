package space.cougs.ground.gui.modules.power;

import java.awt.Graphics;
import java.awt.Graphics2D;

import javax.swing.JPanel;

import space.cougs.ground.gui.utils.AnimationComponent;

public class Wire extends JPanel implements AnimationComponent {
  private static final long serialVersionUID = 1L;

  private double            current          = 0.0;
  private double            voltage          = 0.0;

  public Wire(
      int x, int y, int width, int height, double current, double voltage) {
    this.setBounds(x, y, width, height);
    this.setCurrent(current);
    this.setVoltage(voltage);
  }

  protected void paintComponent(Graphics g) {
    super.paintComponent(g);
    Graphics2D g2d = (Graphics2D)g;

    for (int i = 0; i < this.getHeight(); i++) {
      g2d.fillRect(i * 20, 0, 20, this.getHeight() / 2);
    }
  }

  public void changeDimensions(int x, int y, int width, int height,
      double newCurrent, double newVoltage) {
    voltage = newVoltage;
    current = newCurrent;
    this.setBounds(x, y, width, height);
  }

  public double getCurrent() {
    return current;
  }

  public void setCurrent(double current) {
    this.current = current;
  }

  public double getVoltage() {
    return voltage;
  }

  public void setVoltage(double voltage) {
    this.voltage = voltage;
  }

  @Override
  public void updateFrame(double timerDuration) {
    repaint();
  }

  // solar panel, output, regulator, battery
}
