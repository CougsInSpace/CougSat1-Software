#ifndef _GUI_COUGSAT1_EPS_H_
#define _GUI_COUGSAT1_EPS_H_

#include <ehbanana/Page.h>

namespace GUI {
namespace CougSat1 {

class EPS : public Ehbanana::Page {
public:
  EPS(EBGUI_t gui);
  ~EPS();

  Result onLoad();
  Result sendUpdate();

  Result handleInput(const EBMessage_t & msg);

private:
};

} // namespace CougSat1
} // namespace GUI

#endif /* _GUI_COUGSAT1_EPS_H_ */