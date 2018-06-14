package space.cougs.ground.gui;

import java.awt.CardLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;

import javax.swing.JButton;
import javax.swing.JPanel;

import space.cougs.ground.utils.CustomColors;

class CardSwitcher implements ActionListener {

	private CardLayout cardLayout;
	private JPanel cards;
	private ArrayList<JButton> buttons;

	public CardSwitcher(JPanel cards, CardLayout cardLayout, ArrayList<JButton> buttons) {
		this.cardLayout = cardLayout;
		this.cards = cards;
		this.buttons = buttons;
	}

	@Override
	public void actionPerformed(ActionEvent evt) {
		cardLayout.show(cards, evt.getActionCommand());
		for (JButton button : buttons) {
			button.setBackground(CustomColors.CRIMSON);
		}
		((JButton) evt.getSource()).setBackground(CustomColors.WSU_RED);
	}
}
