#ifndef _GUI_H_
#define _GUI_H_

#include <Ehbanana.h>

namespace GUI {

class GUI {
public:
  GUI() = delete;

  static void init();
  static void run();
  static void deinit();
};

} // namespace GUI

#endif /* _GUI_H_ */