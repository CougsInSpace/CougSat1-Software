package space.cougs.ground.gui.subsystems;

import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.io.File;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JPanel;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.modules.PlantGrid;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.satellites.CougSat;

public class Plant extends JPanel implements UIScaling, SatelliteInfo {

	private static final long serialVersionUID = 1L;

	private PlantGrid grid = new PlantGrid();
	// private ListFiles itr = new ListFiles();
	private List<File> plantFolder = new ArrayList<File>();
	private static final File plantFile = new File("packets/plant");
	
	private int border = 10;



	public Plant() {
		super();
		this.setLayout(null);
		this.addComponentListener(componentListener);

		// itr.listFilesForFolder(plantFile, plantFolder);

		for (File file : plantFolder) {
			grid.addThumbnail(file);
		}

		grid.setBackground(CustomColors.BACKGROUND22);
		
		this.add(grid);
		this.setBackground(CustomColors.BACKGROUND12);
	}

	private final ComponentListener componentListener = new ComponentListener() {

		@Override
		public void componentHidden(ComponentEvent e) {
		}

		@Override
		public void componentMoved(ComponentEvent e) {
		}

		@Override
		public void componentResized(ComponentEvent e) {
			
			grid.setBounds(border, border, getWidth() - border * 2, getHeight() - border * 2);
			repaint();
		}

		@Override
		public void componentShown(ComponentEvent e) {
			this.componentResized(e);
		}
	};

	@Override
	public void updateSatellite(CougSat satellite) {
		// TODO Auto-generated method stub

	}

	@Override
	public void updateUIScaling(UIScale uiScale) {
		// TODO Auto-generated method stub

	}

}