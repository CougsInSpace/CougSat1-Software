package space.cougs.ground.gui.subsystems;

import java.awt.GridBagLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;

import space.cougs.ground.gui.modules.AttitudeIndicator;
import space.cougs.ground.gui.modules.BodyLabel;
import space.cougs.ground.gui.modules.CISButton;
import space.cougs.ground.gui.modules.CISPanel;
import space.cougs.ground.gui.modules.CISTextField;
import space.cougs.ground.gui.modules.HorizontalValue;
import space.cougs.ground.gui.modules.Map;
import space.cougs.ground.gui.utils.CustomColors;
import space.cougs.ground.gui.utils.GridBagConstraintsWrapper;
import space.cougs.ground.satellites.CougSat;

public class ADCS extends CISPanel implements SatelliteInfo {
  private static final long serialVersionUID = 1L;

  private final Map map                             = new Map(2.00);
  private final CISPanel coordinatePanel            = new CISPanel();
  private final CISPanel telemetryPanel             = new CISPanel();
  private final AttitudeIndicator attitudeIndicator = new AttitudeIndicator();
  private final CISPanel coordinateButtonPanel      = new CISPanel();

  private final CISButton buttonTerrestrial = new CISButton("Terrestrial");
  private final CISButton buttonCelestial   = new CISButton("Celestial");
  private final CISButton buttonEuler       = new CISButton("Euler");
  private final CISButton buttonTransmit    = new CISButton("Transmit");

  private final BodyLabel coordinateLabel1 = new BodyLabel();
  private final BodyLabel coordinateLabel2 = new BodyLabel();
  private final BodyLabel coordinateLabel3 = new BodyLabel();

  private final CISTextField coordinate1 = new CISTextField();
  private final CISTextField coordinate2 = new CISTextField();
  private final CISTextField coordinate3 = new CISTextField();

  private final HorizontalValue adcsTemp =
      new HorizontalValue("Temp: ", "        ", 0.4);
  private final HorizontalValue roll =
      new HorizontalValue("Roll: ", "        ", 0.4);
  private final HorizontalValue pitch =
      new HorizontalValue("Pitch: ", "        ", 0.4);
  private final HorizontalValue yaw =
      new HorizontalValue("Yaw: ", "        ", 0.4);
  private final HorizontalValue xPWMOut =
      new HorizontalValue("X PWM Out: ", "        ", 0.4);
  private final HorizontalValue yPWMOut =
      new HorizontalValue("Y PWM Out: ", "        ", 0.4);
  private final HorizontalValue zPWMOut =
      new HorizontalValue("Z PWM Out: ", "        ", 0.4);
  private final HorizontalValue xCurrent =
      new HorizontalValue("X Current: ", "        ", 0.4);
  private final HorizontalValue yCurrent =
      new HorizontalValue("Y Current: ", "        ", 0.4);
  private final HorizontalValue zCurrent =
      new HorizontalValue("Z Current: ", "        ", 0.4);

  private final ActionListener buttonListener = new ActionListener() {
    @Override
    public void actionPerformed(ActionEvent e) {
      if (e.getSource().equals(buttonTransmit)) {
        System.out.println("Transmitting ADCS request");
      } else if (e.getSource().equals(buttonTerrestrial)) {
		  buttonTerrestrial.setBackground(CustomColors.PRIMARY_BUTTON_ACTIVE);
		  buttonCelestial.setBackground(CustomColors.PRIMARY_BUTTON_INACTIVE);
		  buttonEuler.setBackground(CustomColors.PRIMARY_BUTTON_INACTIVE);
		  coordinateLabel1.setText("Latitude");
		  coordinateLabel2.setText("Longitude");
		  coordinateLabel3.setText("");
		  coordinate3.setEnabled(false);
      } else if (e.getSource().equals(buttonCelestial)) {
		buttonTerrestrial.setBackground(CustomColors.PRIMARY_BUTTON_INACTIVE);
		buttonCelestial.setBackground(CustomColors.PRIMARY_BUTTON_ACTIVE);
		buttonEuler.setBackground(CustomColors.PRIMARY_BUTTON_INACTIVE);
		coordinateLabel1.setText("Right Ascension");
		coordinateLabel2.setText("Declination");
		coordinateLabel3.setText("");
		coordinate3.setEnabled(false);
      } else if (e.getSource().equals(buttonEuler)) {
		buttonTerrestrial.setBackground(CustomColors.PRIMARY_BUTTON_INACTIVE);
		buttonCelestial.setBackground(CustomColors.PRIMARY_BUTTON_INACTIVE);
		buttonEuler.setBackground(CustomColors.PRIMARY_BUTTON_ACTIVE);
		coordinateLabel1.setText("Roll");
		coordinateLabel2.setText("Pitch");
		coordinateLabel3.setText("Yaw");
		coordinate3.setEnabled(true);
      } else {
        System.out.println("ADCS button not recognized: " + e.getActionCommand());
      }
    }
  };

  public ADCS() {
    super();

    GridBagConstraintsWrapper gbc = new GridBagConstraintsWrapper();
    gbc.setInsets(5, 5, 5, 5);

    coordinateButtonPanel.setLayout(new GridBagLayout());

    coordinateButtonPanel.add(
        buttonTerrestrial, gbc.setCommon(0, 0, 1, 1, 1.0, 1.0));
    coordinateButtonPanel.add(
        buttonCelestial, gbc.setCommon(1, 0, 1, 1, 1.0, 1.0));
    coordinateButtonPanel.add(buttonEuler, gbc.setCommon(2, 0, 1, 1, 1.0, 1.0));

    coordinatePanel.setLayout(new GridBagLayout());
    coordinatePanel.add(coordinateButtonPanel,
        gbc.setCommon(0, 0, 3, 1, 1.0, 0.0).setInsets(0, 0, 0, 0));

    gbc.setInsets(5, 5, 5, 5);
    coordinatePanel.add(coordinateLabel1, gbc.setCommon(0, 1, 1, 1, 0.0, 1.0));
    coordinatePanel.add(coordinateLabel2, gbc.setXY(0, 2));
    coordinatePanel.add(coordinateLabel3, gbc.setXY(0, 3));

    coordinatePanel.add(coordinate1, gbc.setCommon(1, 1, 1, 1, 1.0, 1.0));
    coordinatePanel.add(coordinate2, gbc.setXY(1, 2));
    coordinatePanel.add(coordinate3, gbc.setXY(1, 3));

    coordinatePanel.add(
        new BodyLabel("\u00B0"), gbc.setCommon(2, 1, 1, 1, 0.0, 1.0));
    coordinatePanel.add(new BodyLabel("\u00B0"), gbc.setXY(2, 2));
    coordinatePanel.add(new BodyLabel("\u00B0"), gbc.setXY(2, 3));

    coordinatePanel.add(buttonTransmit, gbc.setCommon(0, 4, 3, 1, 1.0, 0.0));

    telemetryPanel.setLayout(new GridBagLayout());
    telemetryPanel.add(adcsTemp, gbc.setCommon(0, 0, 1, 1, 1.0, 0.0));
    telemetryPanel.add(xPWMOut, gbc.setXY(0, 3));
    telemetryPanel.add(yPWMOut, gbc.setXY(0, 4));
    telemetryPanel.add(zPWMOut, gbc.setXY(0, 5));
    telemetryPanel.add(roll, gbc.setXY(1, 0));
    telemetryPanel.add(pitch, gbc.setXY(1, 1));
    telemetryPanel.add(yaw, gbc.setXY(1, 2));
    telemetryPanel.add(xCurrent, gbc.setXY(1, 3));
    telemetryPanel.add(yCurrent, gbc.setXY(1, 4));
    telemetryPanel.add(zCurrent, gbc.setXY(1, 5));

    this.setLayout(new GridBagLayout());
    this.setBackground(CustomColors.SECONDARY);
    this.setBorder(BorderFactory.createLineBorder(CustomColors.PRIMARY, 10));
    this.add(map, gbc.setCommon(0, 0, 3, 1, 1.0, 1.0));
    this.add(attitudeIndicator, gbc.setCommon(0, 1, 1, 1, 1.0, 0.0));
    this.add(telemetryPanel, gbc.setCommon(1, 1, 1, 1, 0.0, 0.0));
    this.add(coordinatePanel, gbc.setCommon(2, 1, 1, 1, 1.0, 0.0));

    buttonTerrestrial.addActionListener(buttonListener);
    buttonCelestial.addActionListener(buttonListener);
    buttonEuler.addActionListener(buttonListener);
    buttonTransmit.addActionListener(buttonListener);
    buttonTerrestrial.doClick();
  }

  @Override
  public void updateSatellite(CougSat satellite) {
    map.setValue(
        satellite.getADCS().getLatitude(), satellite.getADCS().getLongitude());

    adcsTemp.setValue(
        String.format("%d\u00B0C", satellite.getECS().getADCSTemp()));
    roll.setValue(String.format("%6.2f\u00B0", satellite.getADCS().getRoll()));
    pitch.setValue(
        String.format("%6.2f\u00B0", satellite.getADCS().getPitch()));
    yaw.setValue(String.format("%6.2f\u00B0", satellite.getADCS().getYaw()));
    xPWMOut.setValue(String.format("%d", satellite.getADCS().getXPWMOut()));
    yPWMOut.setValue(String.format("%d", satellite.getADCS().getYPWMOut()));
    zPWMOut.setValue(String.format("%d", satellite.getADCS().getZPWMOut()));
    xCurrent.setValue(String.format("%fA", satellite.getADCS().getXCurrent()));
    yCurrent.setValue(String.format("%fA", satellite.getADCS().getYCurrent()));
    zCurrent.setValue(String.format("%fA", satellite.getADCS().getZCurrent()));
  }
}
