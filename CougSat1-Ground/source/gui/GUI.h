#ifndef _GUI_GUI_H_
#define _GUI_GUI_H_

#include <Ehbanana.h>

namespace GUI {

class GUI {
public:
  GUI(const GUI &) = delete;
  GUI & operator=(const GUI &) = delete;

  /**
   * @brief Get the singleton instance
   *
   * @return Logger*
   */
  static GUI * Instance() {
    static GUI instance;
    return &instance;
  }

  ~GUI();

  Result handleInput(const EBMessage_t & msg);

private:
  /**
   * @brief Construct a new GUI object
   *
   */
  GUI() {}
};

} // namespace GUI

#endif /* _GUI_GUI_H_ */