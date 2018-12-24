package space.cougs.ground.gui.modules;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Rectangle;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

import javax.imageio.ImageIO;
import javax.swing.JPanel;

import space.cougs.ground.gui.utils.CustomColors;

public class PlantGrid extends JPanel {

	private static final long serialVersionUID = 1L;

	private final List<Image> gridImages = new ArrayList<Image>();
	private final List<File> gridFiles = new ArrayList<File>();

	private Rectangle scrollBar = new Rectangle(5, 0, 0, 0);

	private int lastScrollBarX = 0;
	private int barWidth = 20;
	private int squareLength = 0;

	public PlantGrid() {

		this.addMouseMotionListener(mouseMotionListener);
		this.addMouseListener(mouseListener);
	}

	private MouseListener mouseListener = new MouseListener() {

		@Override
		public void mouseClicked(MouseEvent arg0) {
		}

		@Override
		public void mouseEntered(MouseEvent arg0) {
		}

		@Override
		public void mouseExited(MouseEvent arg0) {
		}

		@Override
		public void mousePressed(MouseEvent e) {
			if (scrollBar.contains(e.getPoint())) {
				lastScrollBarX = e.getX();
			}
		}

		@Override
		public void mouseReleased(MouseEvent arg0) {
		}

	};

	private final MouseMotionListener mouseMotionListener = new MouseMotionListener() {

		@Override
		public void mouseDragged(MouseEvent e) {
			if (scrollBar.contains(e.getPoint())) {
				int x = scrollBar.x + e.getX() - lastScrollBarX;
				x = (int) Math.max(5, Math.min(x, getWidth() - scrollBar.getWidth() - 5));
				scrollBar.x = x;
				lastScrollBarX = e.getX();
				repaint();
			}
		}

		@Override
		public void mouseMoved(MouseEvent e) {
		}

	};

	public void addThumbnail(File thumbnail) {
		try {
			gridFiles.add(thumbnail);
			Image image = ImageIO.read(thumbnail);
			gridImages.add(image);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2d = (Graphics2D) g;
		g2d.setColor(CustomColors.TEXT1);
		
		
		squareLength = Math.min(getHeight(), getWidth()) - barWidth - 10;
		int selectedPhoto = ((gridImages.size()) * (scrollBar.x + scrollBar.width) / (this.getWidth() - 10));

		g2d.drawImage(gridImages.get(Math.max(0, selectedPhoto - 1)), 5, 5, squareLength, squareLength, null);

		g2d.drawString(String.valueOf(Math.max(0, selectedPhoto - 1)), 5 + squareLength / 2, 5 + squareLength / 2);

		g2d.setColor(CustomColors.BAR_BACKGROUND);
		g2d.fillRect(5, this.getHeight() - 5 - barWidth, this.getWidth() - 10, barWidth);

		if (gridImages.size() > 0) {
			scrollBar.setSize((getWidth() - 10) / gridImages.size(), barWidth);
		}
		scrollBar.y = getHeight() - 5 - barWidth;

		g2d.setColor(CustomColors.BAR_DEFAULT);
		g2d.fill(scrollBar);
	}

}
