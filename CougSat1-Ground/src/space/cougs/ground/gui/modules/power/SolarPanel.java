package space.cougs.ground.gui.modules.power;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;

import javax.swing.JComponent;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.utils.AnimationComponent;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;

public class SolarPanel extends JComponent implements AnimationComponent, UIScaling {
	private static final long serialVersionUID = 1L;
	private double current = 0.0;
	private double voltage = 0.0;

	public SolarPanel(double current, double voltage) {
		this.setCurrent(current);
		this.setVoltage(voltage);

		this.setForeground(CustomColors.PRIMARY_TEXT);
	}

	@Override
	public void updateFrame(double timerDuration) {
		repaint();
	}

	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2d = (Graphics2D) g;
		g2d.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

		g2d.drawString(this.getName() + "", this.getHeight() / 2, this.getWidth() / 2);
		g2d.drawString(voltage + "V " + current + "I", this.getHeight() / 2, this.getWidth() / 2);
	}

	public double getVoltage() {
		return voltage;
	}

	public void setVoltage(double voltage) {
		this.voltage = voltage;
	}

	public double getCurrent() {
		return current;
	}

	public void setCurrent(double current) {
		this.current = current;
	}

	@Override
	public void updateUIScaling(UIScale uiScale) {
		this.setFont(Fonts.BODY_12);

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
			break;
		}
	}
}
