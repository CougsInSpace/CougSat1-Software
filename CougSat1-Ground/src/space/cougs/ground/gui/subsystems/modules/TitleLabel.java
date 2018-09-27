package space.cougs.ground.gui.subsystems.modules;

import javax.swing.Icon;
import javax.swing.JLabel;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.utils.Fonts;

public class TitleLabel extends JLabel implements UIScaling {

	private static final long serialVersionUID = 1L;

	public TitleLabel() {
	}

	public TitleLabel(String text) {
		super(text);
	}

	public TitleLabel(Icon image) {
		super(image);
	}

	public TitleLabel(String text, int horizontalAlignment) {
		super(text, horizontalAlignment);
	}

	public TitleLabel(Icon image, int horizontalAlignment) {
		super(image, horizontalAlignment);
	}

	public TitleLabel(String text, Icon icon, int horizontalAlignment) {
		super(text, icon, horizontalAlignment);
	}

	@Override
	public void updateUIScaling(UIScale uiScale) {

		switch (uiScale) {
		case SCALE_100:
			this.setFont(Fonts.TITLE_16);
			break;
		case SCALE_150:
			this.setFont(Fonts.TITLE_24);
			break;
		case SCALE_200:
			this.setFont(Fonts.TITLE_32);
			break;
		case SCALE_300:
			this.setFont(Fonts.TITLE_48);
			break;
		case SCALE_75:
			this.setFont(Fonts.TITLE_12);
			break;
		default:
			break;
		}

	}

}
