package space.cougs.ground.gui;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;

import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;

import space.cougs.ground.CougSatGround;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;
import space.cougs.ground.satellites.CougSat;
import space.cougs.ground.satellites.CougSat1;

public class GUI extends JPanel implements UIScaling {

	private static final long serialVersionUID = 1L;
	private static final int defaultHeight = 650;
	private static final int defaultWidth = 1200;

	private final CougSat1GUI cougSat1GUI = new CougSat1GUI();
	private final JTabbedPane mainPanel = new JTabbedPane();
	private final JPanel home = new Home();
	private final JFrame mainFrame;


	private final ComponentListener componentListener = new ComponentListener() {

		@Override
		public void componentResized(ComponentEvent e) {

			int height = mainFrame.getHeight();
			int width = mainFrame.getWidth();

			if (height > defaultHeight * 3 && width > defaultWidth * 3) {
				updateUIScaling(UIScale.SCALE_300);
			} else if (height > defaultHeight * 2 && width > defaultWidth * 2) {
				updateUIScaling(UIScale.SCALE_200);
			} else if (height > defaultHeight * 1.5 && width > defaultWidth * 1.5) {
				updateUIScaling(UIScale.SCALE_150);
			} else if (height > defaultHeight && width > defaultWidth) {
				updateUIScaling(UIScale.SCALE_100);
			} else {
				updateUIScaling(UIScale.SCALE_75);
			}
		}

		@Override
		public void componentHidden(ComponentEvent e) { // not needed

		}

		@Override
		public void componentMoved(ComponentEvent e) { // not needed

		}

		@Override
		public void componentShown(ComponentEvent e) { // not needed

		}

	};

	public GUI() {
		super();

		Fonts.loadFonts();


		mainPanel.setBackground(CustomColors.ACCENT1);
		mainPanel.setPreferredSize(new Dimension(defaultWidth, defaultHeight));
		mainPanel.setMinimumSize(new Dimension(defaultWidth, defaultHeight));

		mainPanel.addTab("     Home      ", home);
		mainPanel.addTab("   CougSat-1   ", cougSat1GUI);
		mainPanel.setSelectedComponent(cougSat1GUI);

		mainFrame = new JFrame();
		mainFrame.setIconImage(new ImageIcon("resources/images/32/rocket.png").getImage());
		mainFrame.setTitle("CougSat Ground Control v" + CougSatGround.getVersionnumber());
		mainFrame.add(mainPanel);
		mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);// makes exit
																	// button
																	// work
		mainFrame.pack(); // default
		mainFrame.setLocationRelativeTo(null);// centers the screen
		mainFrame.setVisible(true); // makes GUI visible to user / makes visible
		mainFrame.addComponentListener(componentListener);
	}

	@Override
	public void updateUIScaling(UIScale uiScale) {

		for (Component component : mainPanel.getComponents()) {

			if (component instanceof UIScaling) {

				((UIScaling) component).updateUIScaling(uiScale);
			}
		}

		switch (uiScale) {
		case SCALE_100:
			mainPanel.setFont(Fonts.TITLE_16);
			break;
		case SCALE_150:
			mainPanel.setFont(Fonts.TITLE_24);
			break;
		case SCALE_200:
			mainPanel.setFont(Fonts.TITLE_32);
			break;
		case SCALE_300:
			mainPanel.setFont(Fonts.TITLE_48);
			break;
		case SCALE_75:
			mainPanel.setFont(Fonts.TITLE_12);
			break;
		default:
			break;
		}

	}

	public void update(CougSat satellite) {

		if (satellite instanceof CougSat1) {
			cougSat1GUI.updateSatellite(satellite);
		}
	}
}
