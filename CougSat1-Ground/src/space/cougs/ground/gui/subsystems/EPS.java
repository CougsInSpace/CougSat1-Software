package space.cougs.ground.gui.subsystems;

import java.awt.Component;
import java.awt.GridBagLayout;
import java.awt.Insets;

import javax.swing.BorderFactory;
import javax.swing.JPanel;
import javax.swing.JTabbedPane;
import javax.swing.UIManager;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;
import space.cougs.ground.gui.utils.GridBagConstraintsWrapper;
import space.cougs.ground.satellites.CougSat;

public class EPS extends JPanel implements UIScaling{

	private static final long serialVersionUID = 1L;
	private static final JTabbedPane mainPowerPanel = new JTabbedPane();
	private static final JPanel powerGeneration = new JPanel();
	private static final JPanel powerDistribution = new JPanel();

	public EPS() {
	
		super();

		GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();
		gbc.setFill(GridBagConstraintsWrapper.BOTH);
		this.setLayout(new GridBagLayout());
		
		mainPowerPanel.setBackground(CustomColors.BACKGROUND1);
		mainPowerPanel.addTab("   Power Gen    ", powerGeneration);
		mainPowerPanel.addTab("   Power Dist   ", powerDistribution);
		mainPowerPanel.setSelectedComponent(powerGeneration);
		
		
		powerGeneration.setBackground(CustomColors.BACKGROUND2);
		powerDistribution.setBackground(CustomColors.BACKGROUND2);
		
		this.add(mainPowerPanel, gbc.setLocation(0, 0).setSize(1, 1).setWeight(1.0, 1.0).setInsets(10, 10, 10, 10));
		
		this.setBackground(CustomColors.BACKGROUND1);
	}

	@Override
	public void updateUIScaling(UIScale uiScale) {

		for (Component component : mainPowerPanel.getComponents()) {

			if (component instanceof UIScaling) {

				((UIScaling) component).updateUIScaling(uiScale);
			}
		}

		switch (uiScale) {
		case SCALE_100:
			mainPowerPanel.setFont(Fonts.TITLE_16);
			break;
		case SCALE_150:
			mainPowerPanel.setFont(Fonts.TITLE_24);
			break;
		case SCALE_200:
			mainPowerPanel.setFont(Fonts.TITLE_32);
			break;
		case SCALE_300:
			mainPowerPanel.setFont(Fonts.TITLE_48);
			break;
		case SCALE_75:
			mainPowerPanel.setFont(Fonts.TITLE_12);
			break;
		default:
			break;
		}

	}

	public void updateSatellite(CougSat satellite) {

	
	}
}
