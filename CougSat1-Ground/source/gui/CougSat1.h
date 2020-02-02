#ifndef _GUI_COUGSAT1_H_
#define _GUI_COUGSAT1_H_

#include <ehbanana/Page.h>

namespace GUI {

class CougSat1 : public Ehbanana::Page {
public:
  CougSat1(EBGUI_t gui);
  ~CougSat1();

  Result onLoad();
  Result sendUpdate();

  Result handleInput(const EBMessage_t & msg);

private:
};

} // namespace GUI

#endif /* _GUI_COUGSAT1_CougSat1_H_ */