package space.cougs.ground.gui.modules.power;

import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.RenderingHints;

import javax.swing.JComponent;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;

public class Regulator extends JComponent implements UIScaling {

	private static final long serialVersionUID = 1L;
	private String varName;
	private double varCurrent = 0.0;
	private double varVoltage = 0.0;

	public Regulator(String name) {
		varName = name;
	}

  @Override
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2d = (Graphics2D) g;
		FontMetrics fontMetrics = this.getFontMetrics(this.getFont());
		g2d.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

		g2d.setColor(CustomColors.PRIMARY);
		g2d.fillRect(0, 0, this.getWidth(), this.getHeight());

		g2d.setColor(CustomColors.PRIMARY_TEXT);
		g2d.drawString(varName, this.getWidth() / 2 - fontMetrics.stringWidth(varName) / 2, fontMetrics.getAscent());

		String printValues = "E: " + 100 + "%";
		g2d.drawString(printValues, this.getWidth() / 2 - fontMetrics.stringWidth(printValues) / 2,
				fontMetrics.getHeight() + fontMetrics.getAscent());

	}

	public String getName() {
		return varName;
	}

	public void setName(String name) {
		this.varName = name;
	}

	public double getCurrent() {
		return varCurrent;
	}

	public void setCurrent(double current) {
		this.varCurrent = current;
	}

	public double getVoltage() {
		return varVoltage;
	}

	public void setVoltage(double voltage) {
		this.varVoltage = voltage;
	}

	@Override
	public void updateUIScaling(UIScale uiScale) {
		this.setFont(Fonts.BODY_16);

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