package space.cougs.ground.gui.subsystems.modules;

import java.awt.Dimension;
import java.awt.Graphics;

import javax.swing.JPanel;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.utils.Fonts;

public class AttitudeIndicator extends JPanel implements UIScaling {

	private static final long serialVersionUID = 1L;

	public AttitudeIndicator() {
		super();

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
	public void paintComponent(Graphics g) {
		super.paintComponent(g);

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
