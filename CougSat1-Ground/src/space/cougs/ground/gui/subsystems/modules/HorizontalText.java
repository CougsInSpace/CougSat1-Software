package space.cougs.ground.gui.subsystems.modules;

import java.awt.Dimension;
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

public class HorizontalText extends JComponent implements UIScaling {

	private static final long serialVersionUID = 1L;

	private final String label;
	private final double divider;

	private String value;

	public HorizontalText(String label, String value, double divider) {
		super();
		this.label = label;
		this.divider = divider;
		this.value = value;
		this.setForeground(CustomColors.TEXT1);
		this.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));
	}

	@Override
	public Dimension getPreferredSize() {

		FontMetrics fontMetrics = this.getFontMetrics(this.getFont());
		int height = fontMetrics.getHeight();
		int labelWidth = fontMetrics.stringWidth(label);
		int valueWidth = fontMetrics.stringWidth(value);

		Insets insets = this.getBorder().getBorderInsets(this);

		labelWidth = (int) (labelWidth / (divider));
		valueWidth = (int) (valueWidth / (1 - divider));

		int width = Math.max(labelWidth, valueWidth);
		height += insets.bottom + insets.top;
		width += insets.left + insets.right;

		return new Dimension(width, height);

	}

	@Override
	public Dimension getMinimumSize() {

		FontMetrics fontMetrics = this.getFontMetrics(this.getFont());
		int height = fontMetrics.getHeight();
		int labelWidth = fontMetrics.stringWidth(label);
		int valueWidth = fontMetrics.stringWidth(value);

		Insets insets = this.getBorder().getBorderInsets(this);

		labelWidth = (int) (labelWidth / (divider));
		valueWidth = (int) (valueWidth / (1 - divider));

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
		FontMetrics fontMetrics = g2d.getFontMetrics();
		Insets insets = this.getBorder().getBorderInsets(this);

		int x = insets.left;
		int y = ((this.getHeight() - fontMetrics.getHeight()) / 2 + fontMetrics.getAscent());
		g2d.drawString(label, x, y);

		x += (int) ((this.getWidth() - insets.left - insets.right) * divider);
		g2d.drawString(value, x, y);

	}

	public void setValue(String value) {
		this.value = value;
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
