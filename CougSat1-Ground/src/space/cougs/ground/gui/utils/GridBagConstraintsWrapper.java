package space.cougs.ground.gui.utils;

import java.awt.GridBagConstraints;
import java.awt.Insets;

public class GridBagConstraintsWrapper extends GridBagConstraints {
  private static final long serialVersionUID = 1L;

  public GridBagConstraintsWrapper() {
    super();
    insets    = new Insets(0, 0, 0, 0);
    this.fill = GridBagConstraints.BOTH;
  }

  public GridBagConstraintsWrapper setCommon(
      int x, int y, int width, int height) {
    return this.setXY(x, y).setSize(width, height);
  }

  public GridBagConstraintsWrapper setCommon(
      int x, int y, int width, int height, double weightX, double weightY) {
    return this.setXY(x, y)
        .setSize(width, height)
        .setWeight(weightX, weightY);
  }

  public GridBagConstraintsWrapper setXY(int x, int y) {
    this.gridx = x;
    this.gridy = y;
    return this;
  }

  public GridBagConstraintsWrapper setSize(int width, int height) {
    this.gridwidth  = width;
    this.gridheight = height;
    return this;
  }

  public GridBagConstraintsWrapper setWeight(double weightX, double weightY) {
    this.weightx = weightX;
    this.weighty = weightY;
    return this;
  }

  public GridBagConstraintsWrapper setFill(int fill) {
    this.fill   = fill;
    this.anchor = GridBagConstraints.CENTER;
    return this;
  }

  public GridBagConstraintsWrapper setFill(int fill, int anchor) {
    this.fill   = fill;
    this.anchor = anchor;
    return this;
  }

  public GridBagConstraintsWrapper setInsets(
      int top, int left, int bottom, int right) {
    this.insets.set(top, left, bottom, right);
    return this;
  }

  public GridBagConstraintsWrapper setPadding(int padX, int padY) {
    this.ipadx = padX;
    this.ipady = padY;
    return this;
  }
}