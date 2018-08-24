package space.cougs.ground.gui.subsystems;

import java.awt.GridBagLayout;

import javax.swing.JPanel;
import javax.swing.SwingConstants;

import space.cougs.ground.gui.subsystems.modules.TitleLabel;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.GridBagConstraintsWrapper;

public class ADCS extends JPanel {

	private static final long serialVersionUID = 1L;
	
	private final JPanel attitudePanel = new JPanel();
	private final JPanel buttonPanel = new JPanel();
	
	
	public ADCS() {
	
		super();
		this.setBackground(CustomColors.BACKGROUND2);
		
		GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();
		gbc.setFill(GridBagConstraintsWrapper.BOTH);
		
//		adcs.setLayout(new GridBagLayout());

//		adcs.add(new TitleLabel("Attitude", SwingConstants.CENTER),
//				gbc.setLocation(0, 0).setSize(1, 1).setWeight(0.0, 0.0).setInsets(5, 5, 5, 5));
//		adcs.add(map, gbc.setLocation(0, 1).setSize(1, 1).setWeight(1.0, 1.0).setInsets(5, 5, 5, 5));
	}
}
