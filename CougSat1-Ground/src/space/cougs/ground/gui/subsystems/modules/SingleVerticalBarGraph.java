package space.cougs.ground.gui.subsystems.modules;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Insets;
import java.awt.RenderingHints;

import javax.swing.BorderFactory;
import javax.swing.JComponent;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;

public class SingleVerticalBarGraph extends JComponent implements UIScaling {

	private static final long serialVersionUID = 1L;

	private final String label;
	private final boolean printMinMax;
	private final int min;
	private final int max;
	private final double barWidth;
	private double value;
	private double warningHigh;
	private double warningLow;
	private double dangerHigh;
	private double dangerLow;

	private Font sideFont = Fonts.CONSOLE_12;
	private Color barFill = CustomColors.BAR_DEFAULT;

	public SingleVerticalBarGraph(String label, int min, int max, int value, double barWidth, boolean printMinMax,
			double warningHigh, double warningLow, double dangerHigh, double dangerLow) {

		this.label = label;
		this.min = min;
		this.max = max;
		this.value = value;
		this.barWidth = barWidth;
		this.printMinMax = printMinMax;
		this.warningHigh = warningHigh;
		this.warningLow = warningLow;
		this.dangerHigh = dangerHigh;
		this.dangerLow = dangerLow;

		this.setForeground(CustomColors.TEXT1);
		this.setBackground(CustomColors.BAR_BACKGROUND);

		this.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, 0));

	}

	@Override
	public Dimension getPreferredSize() {

		FontMetrics fontMetrics = this.getFontMetrics(this.getFont());
		FontMetrics sideFontMetrics = this.getFontMetrics(sideFont);
		int height = fontMetrics.getHeight() + sideFontMetrics.getHeight() * 3;
		int labelWidth = fontMetrics.stringWidth(label);
		int valueWidth = Math.max(sideFontMetrics.stringWidth(" " + min), sideFontMetrics.stringWidth(" " + max));

		Insets insets = this.getBorder().getBorderInsets(this);

		valueWidth = (int) (valueWidth / (1 - barWidth));

		int width = Math.max(labelWidth, valueWidth);
		height += insets.bottom + insets.top;
		width += insets.left + insets.right;

		return new Dimension(width, height);

	}

	@Override
	public Dimension getMinimumSize() {

		FontMetrics fontMetrics = this.getFontMetrics(this.getFont());
		FontMetrics sideFontMetrics = this.getFontMetrics(sideFont);
		int height = fontMetrics.getHeight();
		int labelWidth = fontMetrics.stringWidth(label);
		int valueWidth = Math.max(sideFontMetrics.stringWidth(" " + min), sideFontMetrics.stringWidth(" " + max));

		Insets insets = this.getBorder().getBorderInsets(this);

		valueWidth = (int) (valueWidth / (1 - barWidth));

		int width = Math.max(labelWidth, valueWidth);
		height += insets.bottom + insets.top;
		width += insets.left + insets.right;

		return new Dimension(width, height);
	}

	@Override
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2d = (Graphics2D) g;
		g2d.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

		FontMetrics fontMetrics = this.getFontMetrics(this.getFont());
		FontMetrics sideFontMetrics = this.getFontMetrics(sideFont);

		Insets insets = this.getBorder().getBorderInsets(this);

		int adjustedWidth = this.getWidth() - insets.left - insets.right;
		int adjustedHeight = this.getHeight() - insets.left - insets.right;

		int barHeight = adjustedHeight - fontMetrics.getHeight();
		int barWidth = (int) (adjustedWidth * this.barWidth);
		int rectX = (printMinMax) ? insets.left : (adjustedWidth - barWidth) / 2 + insets.left;

		g2d.setColor(this.getBackground());
		g2d.fillRect(rectX, insets.top, barWidth, barHeight);

		int x = Math.max((barWidth - fontMetrics.stringWidth(label)) / 2, 0) + rectX;
		int y = this.getHeight() - insets.bottom - fontMetrics.getDescent();

		g2d.setColor(this.getForeground());
		g2d.drawString(label, x, y);

		g2d.setFont(sideFont);

		if (printMinMax) {
			g2d.drawString(" " + max, insets.left + barWidth, insets.top + sideFontMetrics.getAscent());
			g2d.drawString(" " + min, insets.left + barWidth, insets.top + barHeight);
		}

		barHeight = (int) (barHeight * (value - min) / (double) (max - min));
		y = this.getHeight() - fontMetrics.getHeight() - insets.bottom - barHeight;
		g2d.setColor(barFill);
		g2d.fillRect(rectX, y, barWidth, barHeight);

		g2d.setColor(this.getForeground());
		g2d.setFont(sideFont);
		x = (barWidth - sideFontMetrics.stringWidth(String.format("%.1f", value))) / 2 + rectX;
		y = y - sideFontMetrics.getDescent();
		y = Math.min(adjustedHeight - fontMetrics.getHeight(), Math.max(sideFontMetrics.getAscent(), y));
		g2d.drawString(String.format("%.1f", value), x, y);

	}

	public void setValue(double d, Color barFill) {
		this.value = d;
		this.barFill = barFill;
	}

	public void setValue(double d) {
		this.value = d;

		double value = ((this.max - this.value) / this.max);

		if ((value <= dangerLow || ((value) >= dangerHigh))) {
			this.barFill = Color.RED;
		} else if ((value) <= warningLow || (value) >= warningHigh) {
			this.barFill = Color.YELLOW;
		} else {
			this.barFill = Color.GREEN;
		}
	}

	@Override
	public void updateUIScaling(UIScale uiScale) {
		this.setFont(Fonts.BODY_12);

		switch (uiScale) {
		case SCALE_100:
			sideFont = Fonts.CONSOLE_12;
			this.setFont(Fonts.BODY_16);
			break;
		case SCALE_150:
			sideFont = Fonts.CONSOLE_18;
			this.setFont(Fonts.BODY_24);
			break;
		case SCALE_200:
			sideFont = Fonts.CONSOLE_24;
			this.setFont(Fonts.BODY_32);
			break;
		case SCALE_300:
			sideFont = Fonts.CONSOLE_36;
			this.setFont(Fonts.BODY_48);
			break;
		case SCALE_75:
			sideFont = Fonts.CONSOLE_8;
			this.setFont(Fonts.BODY_12);
			break;
		default:
			break;
		}

	}

}