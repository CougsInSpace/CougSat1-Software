package space.cougs.ground.gui.subsystems;

import java.awt.Component;
import java.awt.Container;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.io.File;
import java.util.Iterator;

import javax.imageio.ImageIO;
import javax.imageio.ImageReader;
import javax.imageio.stream.ImageInputStream;
import javax.swing.JLabel;
import javax.swing.JPanel;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.subsystems.modules.MetaData;
import space.cougs.ground.gui.subsystems.modules.PhotoViewer;
import space.cougs.ground.gui.subsystems.modules.ThumbnailGrid;
import space.cougs.ground.gui.subsystems.modules.TitleLabel;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;
import space.cougs.ground.satellites.CougSat;

public class Plant extends JPanel implements UIScaling, SatelliteInfo {

	private static final long serialVersionUID = 1L;

	private static final double thumbnailWidthFactor = 0.3;

	private static final ThumbnailGrid thumbnailGrid = new ThumbnailGrid(2);
	private static final PhotoViewer photoViewer = new PhotoViewer();
	MetaData meta = new MetaData();
	private int border = 10;

	private static final File photo = new File("packets/images/image_thumbnail.png");
	private static final File highRes = new File("packets/images/image.png");
	private static final File test = new File("packets/images/P2090193.JPG");

	private final ComponentListener componentListener = new ComponentListener() {

		@Override
		public void componentHidden(ComponentEvent e) {
		}

		@Override
		public void componentMoved(ComponentEvent e) {
		}

		@Override
		public void componentResized(ComponentEvent e) {
			int thumbnailWidth = (int) (getWidth() * thumbnailWidthFactor);
			thumbnailGrid.setBounds(border, border, thumbnailWidth - border * 2, getHeight() - border * 2);
			photoViewer.setBounds(thumbnailWidth, border, getWidth() - thumbnailWidth - border,
					getHeight() - border * 2);
			repaint();
		}

		@Override
		public void componentShown(ComponentEvent e) {
			this.componentResized(e);
		}

	};

	public Plant() {

		super();
		this.setLayout(null);
		this.addComponentListener(componentListener);

//		File[] files = new File("/").listFiles();
//	    showFiles(files);

		thumbnailGrid.setBackground(CustomColors.BACKGROUND2);
		photoViewer.setBackground(CustomColors.BACKGROUND2);

		this.add(thumbnailGrid);
		this.add(photoViewer);

		this.setBackground(CustomColors.BACKGROUND1);

		thumbnailGrid.addThumbnail(photo);
		thumbnailGrid.addThumbnail(highRes);
		thumbnailGrid.addThumbnail(photo);
		thumbnailGrid.addThumbnail(photo);
		thumbnailGrid.addThumbnail(photo);
		thumbnailGrid.addThumbnail(photo);
		thumbnailGrid.addThumbnail(photo);
		thumbnailGrid.addThumbnail(photo);
		thumbnailGrid.addThumbnail(photo);
		thumbnailGrid.addThumbnail(test);
		thumbnailGrid.addActionListner(actionListener);

	}

	private ActionListener actionListener = new ActionListener() {

		@Override
		public void actionPerformed(ActionEvent e) {

			photoViewer.setThumbnail(thumbnailGrid.getCurrentThumbnail());
			
			Metadata meta = ImageMetadataReader.readMetadata(thumbnailGrid.getCurrentThumbnail());
		}
	};

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
		// TODO Auto-generated method stub

	}
}
