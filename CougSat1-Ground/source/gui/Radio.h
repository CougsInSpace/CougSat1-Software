#ifndef _GUI_RADIO_H_
#define _GUI_RADIO_H_

#include <ehbanana/Page.h>

namespace GUI {

class Radio : public Ehbanana::Page {
public:
  Radio(EBGUI_t gui);
  ~Radio();

  Result onLoad();
  Result sendUpdate();

  Result handleInput(const EBMessage_t & msg);

private:
};

} // namespace GUI

#endif /* _RADIO_H_ */