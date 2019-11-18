#ifndef _GUI_RADIO_H_
#define _GUI_RADIO_H_

#include <ehbanana/Page.h>

#include <tools/CircularBuffer.h>

namespace GUI {

class Radio : public Ehbanana::Page {
public:
  Radio(EBGUI_t gui);
  ~Radio();

  Result onLoad();
  Result sendUpdate();

  Result handleInput(const EBMessage_t & msg);

  CircularBuffer<PairDouble_t> * getConstellationBuffer();
  Result                         updateConstellation();

private:
  CircularBuffer<PairDouble_t> data;
};

} // namespace GUI

#endif /* _RADIO_H_ */