package space.cougs.ground.gui;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;

import javax.swing.ImageIcon;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;

import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;

public class GUI extends Thread implements UIScaling {

	private static final int defaultHeight = 700;
	private static final int defaultWidth = 1200;

	private final CougSat1 cougSat1;
	private final JTabbedPane mainPanel;
	private final JPanel home;
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

		mainPanel = new JTabbedPane();
		mainPanel.setBackground(CustomColors.ACCENT1);
		mainPanel.setPreferredSize(new Dimension(defaultWidth, defaultHeight));
		mainPanel.setMinimumSize(new Dimension(defaultWidth, defaultHeight));

		cougSat1 = new CougSat1();
		home = new JPanel();

		mainPanel.addTab("     Home      ", home);
		mainPanel.addTab("   CougSat-1   ", cougSat1);
		mainPanel.setSelectedComponent(cougSat1);

		mainFrame = new JFrame();
		mainFrame.setIconImage(new ImageIcon("resources/images/32/rocket.png").getImage());
		mainFrame.setTitle("CougSat Ground Control");
		mainFrame.add(mainPanel);
		mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);// makes exit
																	// button
																	// work
		mainFrame.pack(); // default
		mainFrame.setLocationRelativeTo(null);// centers the screen
		mainFrame.setVisible(true); // makes GUI visible to user / makes visible
		mainFrame.setResizable(true); // Disables the full screen mode
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

}
