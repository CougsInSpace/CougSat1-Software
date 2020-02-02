#ifndef _GUI_ROOT_H_
#define _GUI_ROOT_H_

#include <ehbanana/Page.h>

namespace GUI {

class Root : public Ehbanana::Page {
public:
  Root(EBGUI_t gui);
  ~Root();

  Result onLoad();
  Result sendUpdate();

  Result handleInput(const EBMessage_t & msg);

private:
};

} // namespace GUI

#endif /* _ROOT_H_ */