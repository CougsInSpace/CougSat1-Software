package space.cougs.ground.gui.subsystems;

import java.awt.Component;
import java.awt.FontMetrics;
import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.util.ArrayList;
import java.util.List;

import javax.swing.JPanel;
import javax.swing.JTabbedPane;
import javax.swing.Timer;

import space.cougs.ground.gui.UIScaling;
import space.cougs.ground.gui.subsystems.modules.power.SolarPanel;
import space.cougs.ground.gui.subsystems.modules.power.Wire;
import space.cougs.ground.gui.utils.AnimationComponent;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.Fonts;
import space.cougs.ground.gui.utils.GridBagConstraintsWrapper;
import space.cougs.ground.satellites.CougSat;

public class EPS extends JPanel implements UIScaling, SatelliteInfo {

	private static final long serialVersionUID = 1L;
	private static final JTabbedPane mainPowerPanel = new JTabbedPane();
	private static final JPanel powerGeneration = new JPanel();
	private static final JPanel powerDistribution = new JPanel();

	private List<AnimationComponent> movingComponents = new ArrayList<AnimationComponent>();
	private static final double timerDelay = (1 / 10);

	private Wire wire1 = new Wire(0, 0, 0, 0, 0.0, 0.0);

	private SolarPanel pV0 = new SolarPanel(0.0, 0.0);
	private SolarPanel pV1 = new SolarPanel(0.0, 0.0);
	private SolarPanel pV2 = new SolarPanel(0.0, 0.0);
	private SolarPanel pV3 = new SolarPanel(0.0, 0.0);
	private SolarPanel pV4 = new SolarPanel(0.0, 0.0);
	private SolarPanel pV5 = new SolarPanel(0.0, 0.0);
	private SolarPanel pV6 = new SolarPanel(0.0, 0.0);
	private SolarPanel pV7 = new SolarPanel(0.0, 0.0);

	private List<SolarPanel> solarPanels = new ArrayList<SolarPanel>();
//	int x, int y, int width, int height, double current, double voltage

	public EPS() {

		super();

		solarPanels.add(pV0);
		solarPanels.add(pV1);
		solarPanels.add(pV2);
		solarPanels.add(pV3);
		solarPanels.add(pV4);
		solarPanels.add(pV5);
		solarPanels.add(pV6);
		solarPanels.add(pV7);

		movingComponents.add(wire1);

		Timer timer = new Timer(100, new MyActionListener());
		timer.start();

		GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();
		gbc.setFill(GridBagConstraintsWrapper.BOTH);
		this.setLayout(new GridBagLayout());

		powerGeneration.add(pV0);
		powerGeneration.add(pV1);
		powerGeneration.add(pV2);
		powerGeneration.add(pV3);
		powerGeneration.add(pV4);
		powerGeneration.add(pV5);
		powerGeneration.add(pV6);
		powerGeneration.add(pV7);
		powerGeneration.addComponentListener(generationListener);

		powerDistribution.addComponentListener(distributionListener);

		mainPowerPanel.setBackground(CustomColors.BACKGROUND1);
		mainPowerPanel.addTab("   Power Gen    ", powerGeneration);
		mainPowerPanel.addTab("   Power Dist   ", powerDistribution);
		mainPowerPanel.setSelectedComponent(powerGeneration);

		powerGeneration.setBackground(CustomColors.BACKGROUND2);
		powerDistribution.setBackground(CustomColors.BACKGROUND2);

		this.add(mainPowerPanel, gbc.setLocation(0, 0).setSize(1, 1).setWeight(1.0, 1.0).setInsets(10, 10, 10, 10));

		this.setBackground(CustomColors.BACKGROUND1);
	}

	private final ComponentListener generationListener = new ComponentListener() {

		@Override
		public void componentHidden(ComponentEvent e) {

		}

		@Override
		public void componentMoved(ComponentEvent e) {

		}

		@Override
		public void componentResized(ComponentEvent e) {

			FontMetrics fontMetrics = powerGeneration.getFontMetrics(powerGeneration.getFont());

			for (int i = 0; i < 8; i++) {
				solarPanels.get(i).setBackground(CustomColors.BACKGROUND1);
				solarPanels.get(i).setBounds(solarPanels.get(Math.max(0, i-1)).getX() + 10, 10,
						2 + fontMetrics.stringWidth(
								solarPanels.get(i).getVoltage() + "V " + solarPanels.get(i).getCurrent() + "I"),
						fontMetrics.getHeight() * 2 + 2);
			}
			repaint();
		}

		@Override
		public void componentShown(ComponentEvent e) {
			this.componentResized(e);
		}
	};

	private final ComponentListener distributionListener = new ComponentListener() {

		@Override
		public void componentHidden(ComponentEvent e) {

		}

		@Override
		public void componentMoved(ComponentEvent e) {

		}

		@Override
		public void componentResized(ComponentEvent e) {

		}

		@Override
		public void componentShown(ComponentEvent e) {
			this.componentResized(e);
		}
	};

	public final class MyActionListener implements ActionListener {

		@Override
		public void actionPerformed(ActionEvent arg0) {

			for (AnimationComponent movingComponent : movingComponents) {
				movingComponent.updateFrame(timerDelay);
			}
		}
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

		int i = 0;
		for (SolarPanel solarPanel : solarPanels) {

			solarPanel.setVoltage(satellite.getEPS().getPVVoltage(i));
			solarPanel.setCurrent(satellite.getEPS().getPVCurrent(i));
			i++;
		}
	}
}
