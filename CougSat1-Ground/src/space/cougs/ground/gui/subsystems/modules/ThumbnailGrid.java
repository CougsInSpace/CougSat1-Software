package space.cougs.ground.gui.subsystems.modules;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.Rectangle;
import java.awt.event.ActionListener;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
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

public class ThumbnailGrid extends JPanel {

	private static final long serialVersionUID = 1L;

	private double scrollPosition = 0.0;
	private int columns = 1;
	private int barWidth = 20;
	private int gridHeight = 0;
	private int squareLength = 0;
	private final List<Image> thumbnails = new ArrayList<Image>();
	private final List<File> thumbnailFile = new ArrayList<File>();
	private final List<ActionListener> actionListners = new ArrayList<ActionListener>();
	private File currentThumbnail = null;

	private int lastScrollBarY = 0;

	private Rectangle scrollBar = new Rectangle(0, 0, 0, 0);

	
	public ThumbnailGrid(int columns) {
		this.columns = columns;
		this.addMouseMotionListener(mouseMotionListener);
		this.addMouseListener(mouseListener);
	}
	
	public void addActionListner(ActionListener listner)
	{
		actionListners.add(listner);
	}
	
	private MouseListener mouseListener = new MouseListener() {

		
		
		@Override
		public void mouseClicked(MouseEvent e) {

		}

		@Override
		public void mouseEntered(MouseEvent e) {
			// TODO Auto-generated method stub

		}

		@Override
		public void mouseExited(MouseEvent e) {
			// TODO Auto-generated method stub

		}

		@Override
		public void mousePressed(MouseEvent e) {

			if (scrollBar.contains(e.getPoint())) {
				lastScrollBarY = e.getY();
			}
		}

		@Override
		public void mouseReleased(MouseEvent e) {
			int hiddenHeight = (int) (scrollPosition * (gridHeight - getHeight()));
			int currentColumn = e.getX() / squareLength;
			int currentRow = (e.getY() + hiddenHeight) / squareLength;
			int selectedThumbnail = currentRow * 2 + currentColumn;


			if (selectedThumbnail < thumbnailFile.size())
			{
				
				setCurrentThumbnail(thumbnailFile.get(selectedThumbnail));	
				for (ActionListener listner : actionListners)
				{
					listner.actionPerformed(null);
				}
	
			}
		}

	};

	private final MouseMotionListener mouseMotionListener = new MouseMotionListener() {

		@Override
		public void mouseDragged(MouseEvent e) {
			if (scrollBar.contains(e.getPoint())) {
				int y = scrollBar.y + e.getY() - lastScrollBarY;
				y = (int) Math.max(0, Math.min(y, getHeight() - scrollBar.getHeight()));
				scrollBar.y = y;
				scrollPosition = y / (getHeight() - scrollBar.getHeight());
				lastScrollBarY = e.getY();
				repaint();
			}
		}

		@Override
		public void mouseMoved(MouseEvent e) {
		}

	};

	public void addThumbnail(File thumbnail) {
		try {
			thumbnailFile.add(thumbnail);
			Image image = ImageIO.read(thumbnail);
			thumbnails.add(image);
			gridHeight = (int) (squareLength * Math.ceil((double) thumbnails.size() / columns));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	@Override
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2d = (Graphics2D) g;
		g2d.setColor(CustomColors.TEXT1);
		
		squareLength = (getWidth() - barWidth) / columns;
		gridHeight = (int) (squareLength * Math.max(1, Math.ceil((double) thumbnails.size() / columns)));
		scrollBar.setSize(barWidth, getHeight() * getHeight() / gridHeight);
		scrollBar.x = squareLength * 2;
		
		int y = (int) (-squareLength - scrollPosition * Math.max(0, gridHeight - this.getHeight()));
		for (int i = 0; i < thumbnails.size(); i++) {
			if (i % columns == 0) {
				y += squareLength;
			}
			int x = squareLength * (i % columns);
			g2d.drawImage(thumbnails.get(i), x, y, squareLength, squareLength, null);
			g2d.drawString(String.valueOf(i), x + squareLength / 2, y + squareLength / 2);
		}

		g2d.setColor(CustomColors.BAR_BACKGROUND);
		g2d.fillRect(squareLength * 2, 0, this.getWidth() - squareLength * 2, this.getHeight());

		g2d.setColor(CustomColors.BAR_DEFAULT);
		g2d.fill(scrollBar);
		this.repaint();
	}

	public File getCurrentThumbnail() {
		return currentThumbnail;
	}

	public void setCurrentThumbnail(File newThumbnail) {
		this.currentThumbnail = newThumbnail;
	}
	
}
