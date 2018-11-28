package space.cougs.ground.gui.subsystems;

import java.awt.Component;
import java.awt.Container;
import java.awt.Font;
import java.awt.GridBagLayout;
import java.awt.GridLayout;
import java.io.File;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.UIManager;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.subsystems.modules.Thumbnail;
import space.cougs.ground.gui.subsystems.modules.TitleLabel;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;
import space.cougs.ground.gui.utils.GridBagConstraintsWrapper;
import space.cougs.ground.satellites.CougSat;

public class Plant extends JPanel implements UIScaling, SatelliteInfo {

	private static final long serialVersionUID = 1L;

	private static final JPanel photoWrapper = new JPanel();
	private static final JScrollPane photoWrapperScroll = new JScrollPane(photoWrapper);
	private static final JPanel photoViewer = new JPanel();

	public Plant() {

		super();
		GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();
		gbc.setFill(GridBagConstraintsWrapper.BOTH);
		this.setLayout(new GridBagLayout());

//		File[] files = new File("/").listFiles();
//	    showFiles(files);

		photoWrapper.setBackground(CustomColors.BACKGROUND2);
		photoWrapper.setLayout(new GridLayout(0, 2, 5, 5));

		photoViewer.setBackground(CustomColors.BACKGROUND2);

		this.add(photoWrapperScroll, gbc.setLocation(0, 0).setSize(2, 3).setWeight(0.4, 1.0).setInsets(10, 10, 10, 5));
		this.add(photoViewer, gbc.setLocation(3, 0).setSize(4, 3).setWeight(1.0, 1.0).setInsets(10, 5, 10, 10));

		this.setBackground(CustomColors.BACKGROUND1);
	}

	public static void showFiles(File[] files) {
		for (File file : files) {
			if (file.isDirectory()) {
				System.out.println("Directory: " + file.getName());
				showFiles(file.listFiles()); // Calls same method again.
			} else {
				System.out.println("File: " + file.getName());
			}
		}
	}

	@Override
	public void updateUIScaling(UIScale uiScale) {

		Font titleFont = Fonts.BODY_16;
		int scrollBarSize = 20;

		switch (uiScale) {
		case SCALE_100:
			titleFont = Fonts.BODY_16;
			scrollBarSize = 20;
			break;
		case SCALE_150:
			titleFont = Fonts.BODY_24;
			scrollBarSize = 30;
			break;
		case SCALE_200:
			titleFont = Fonts.BODY_32;
			scrollBarSize = 40;
			break;
		case SCALE_300:
			titleFont = Fonts.BODY_48;
			scrollBarSize = 60;
			break;
		case SCALE_75:
			titleFont = Fonts.BODY_12;
			scrollBarSize = 15;
			break;
		default:
			break;
		}
		UIManager.put("ScrollBar.width", scrollBarSize);
		photoWrapperScroll.setVerticalScrollBar(photoWrapperScroll.createVerticalScrollBar());
		photoWrapperScroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);

		for (Component component : this.getComponents()) {

			if (component instanceof UIScaling) {

				((UIScaling) component).updateUIScaling(uiScale);

			}
			if (component instanceof JPanel) {

				for (Component subComponent : ((Container) component).getComponents()) {

					if (subComponent instanceof UIScaling) {

						((UIScaling) subComponent).updateUIScaling(uiScale);

					} else if (subComponent instanceof JLabel && !(subComponent instanceof TitleLabel)) {

						subComponent.setFont(titleFont);
					}
				}
			}
		}
	}

	@Override
	public void updateSatellite(CougSat satellite) {

		File photo = new File("packets/images/image.png");


		for (int i = 0; i < 10; i++)
		{
			photoWrapper.add(new Thumbnail(photo, (photoWrapper.getWidth()), (photoWrapper.getWidth())));			
		}

//		photoWrapper.add(new Thumbnail(photo, (photoWrapper.getWidth()-15) / 2, photoWrapper.getHeight()*3/4));
		
//		System.out.print(photoWrapper.getWidth());
		System.out.print((photoWrapper.getWidth()- 15) / 2);
		System.out.println("\n");
		System.out.print((photoWrapper.getWidth()- 15) / 2 * 3/4);

		this.repaint();
	}

}
