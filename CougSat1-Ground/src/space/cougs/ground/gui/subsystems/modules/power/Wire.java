package space.cougs.ground.gui.subsystems.modules.power;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JPanel;
import javax.swing.Timer;

public class Wire extends JPanel{

	private static final long serialVersionUID = 1L;
	private double current = 0.0;
	private double voltage = 0.0;
	
	
	public Wire(int x, int y, int width, int height, double current, double voltage) {
		
		this.setBounds(x, y, width, height);
		this.setCurrent(current);
		this.setVoltage(voltage);
	}
	
	public void animate()
	{
		
			
		Timer timer = new Timer(100,new MyActionListener());
	    timer.start();
			
		
	}
	
    public final class MyActionListener implements ActionListener{

     
		@Override
		public void actionPerformed(ActionEvent arg0) {
			
			repaint();
		}

    }
	public void paintComponent(Graphics g) {
		super.paintComponent(g);
		Graphics2D g2d = (Graphics2D) g;
		
		
		
		for(int i = 0; i < this.getHeight(); i++)
		{
			g2d.fillRect(i*20, 0, 20, this.getHeight());
		}

      }
	
	public void changeDimensions(int x, int y, int width, int height)
	{
		this.setBounds(x, y, width, height);
	}

	public double getCurrent() {
		return current;
	}

	public void setCurrent(double current) {
		this.current = current;
	}

	public double getVoltage() {
		return voltage;
	}

	public void setVoltage(double voltage) {
		this.voltage = voltage;
	}
	
	
	
	
//solar panel, output, regulator, battery
}
