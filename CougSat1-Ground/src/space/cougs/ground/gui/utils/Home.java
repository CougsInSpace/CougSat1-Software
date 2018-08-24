package space.cougs.ground.gui.utils;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridBagLayout;
import java.awt.RenderingHints;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.JLabel;
import javax.swing.JPanel;

import space.cougs.ground.gui.UIScaling;

public class Home extends JPanel implements UIScaling {

	private static final long serialVersionUID = 1L;
	private static final int defaultHeight = 650;
	private static final int defaultWidth = 1200;
	BufferedImage CISLogo;

	public Home() {
		super();

		GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();
		gbc.setFill(GridBagConstraintsWrapper.BOTH);

		this.setLayout(new GridBagLayout());
		this.setBackground(CustomColors.BACKGROUND4);
		this.setPreferredSize(new Dimension(defaultWidth, defaultHeight));
		this.setMinimumSize(new Dimension(defaultWidth, defaultHeight));

		try {
			CISLogo = ImageIO.read(new File("resources/images/CISClubLogo.png"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		JLabel LogoText = new JLabel();
		LogoText.setText("CIS Patch Notes V1.0.1");

		this.add(LogoText, gbc.setLocation(0, 1).setSize(3, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));

		this.repaint();
	}

	@Override
	public void updateUIScaling(UIScale uiScale) {
		Font font = Fonts.BODY_24;

		switch (uiScale) {
		default:
		case SCALE_100:
			font = Fonts.BODY_24;
			break;
		case SCALE_150:
			font = Fonts.BODY_36;
			break;
		case SCALE_200:
			font = Fonts.BODY_48;
			break;
		case SCALE_300:
			font = Fonts.BODY_48;
			break;
		case SCALE_75:
			font = Fonts.BODY_16;
			break;
		}

		for (Component component : this.getComponents()) {
			if (component instanceof JLabel) {

				component.setFont(font);

			}
		}
	}

	@Override
	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2d = (Graphics2D) (g);

		g2d.setRenderingHint(RenderingHints.KEY_ALPHA_INTERPOLATION, RenderingHints.VALUE_ALPHA_INTERPOLATION_QUALITY);
		g2d.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING, RenderingHints.VALUE_TEXT_ANTIALIAS_ON);

		g2d.drawImage(CISLogo, 0, 0, this.getWidth() / 2 - this.getWidth()/6, this.getHeight() / 2, this);

	}

}
