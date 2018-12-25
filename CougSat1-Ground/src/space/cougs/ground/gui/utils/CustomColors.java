package space.cougs.ground.gui.utils;

import java.awt.Color;

public class CustomColors {
  private static final Color CRIMSON = new Color(152, 30, 50);
  private static final Color GREY    = new Color(94, 106, 113);
  private static final Color RED     = new Color(198, 12, 48);
  private static final Color YELLOW  = new Color(198, 198, 48);
  private static final Color GREEN   = new Color(12, 198, 48);
  private static final Color NAVY    = new Color(14, 28, 49);
  private static final Color WHITE   = new Color(255, 255, 255);

  public static Color PRIMARY                  = NAVY;
  public static Color PRIMARY_TEXT             = WHITE;
  public static Color PRIMARY_BUTTON_ACTIVE    = RED;
  public static Color PRIMARY_BUTTON_HIGHLIGHT = WHITE;
  public static Color PRIMARY_BUTTON_INACTIVE  = CRIMSON;

  public static Color SECONDARY      = GREY;
  public static Color SECONDARY_TEXT = WHITE;

  public static Color ACCENT = CRIMSON;

  public static Color STATUS_GOOD = GREEN;
  public static Color STATUS_WARN = YELLOW;
  public static Color STATUS_BAD  = RED;
}
