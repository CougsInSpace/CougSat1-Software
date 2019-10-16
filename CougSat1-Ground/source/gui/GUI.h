#ifndef _GUI_GUI_H_
#define _GUI_GUI_H_

#include <Ehbanana.h>

#include "Root.h"

namespace GUI {

class GUI {
public:
  GUI(const GUI &) = delete;
  GUI & operator=(const GUI &) = delete;

  /**
   * @brief Get the singleton instance
   *
   * @return GUI*
   */
  static GUI * Instance() {
    static GUI instance;
    return &instance;
  }

  ~GUI();

  Result init();
  Result run();
  Result deinit();

  Result handleInput(const EBMessage_t & msg);

  static ResultCode_t __stdcall guiProcess(const EBMessage_t & msg);

private:
  /**
   * @brief Construct a new GUI object
   *
   */
  GUI() {}

  EBGUI_t gui = nullptr;

  Root * root;
};

} // namespace GUI

#endif /* _GUI_H_ */