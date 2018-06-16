package space.cougs.ground.gui.subsystems.modules;

import java.awt.Dimension;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;

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
	}

	@Override
	public Dimension getPreferredSize() {
	
		FontMetrics fontMetrics = this.getFontMetrics(this.getFont());
		int height = fontMetrics.getHeight();
		int width = fontMetrics.stringWidth(value);
		
		width = (int) (width/ (1 - divider));
		height += 2;
		width += 2;
		
		return new Dimension(width, height);

	}

	@Override
	public Dimension getMinimumSize() {
		
		FontMetrics fontMetrics = this.getFontMetrics(this.getFont());
		int height = fontMetrics.getHeight();
		int width = fontMetrics.stringWidth(value);
		
		width = (int) (width/ (1 - divider));
		height += 2;
		width += 2;
		
		return new Dimension(width, height);

	}

	@Override
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2d = (Graphics2D) g;
		FontMetrics fontMetrics = g2d.getFontMetrics();
		int x = 0;
		int y = this.getHeight() / 2 + fontMetrics.getAscent();
		g2d.drawString(label, x, y);

		x = (int) (this.getWidth() * divider);
		g2d.drawString(value, x, y);

		this.setMinimumSize(new Dimension(200, 200));

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
