#ifndef _GUI_RADIO_H_
#define _GUI_RADIO_H_

#include "tools/CircularBuffer.h"

namespace GUI {

class Radio {
public:
  Radio() = delete;

  static void __stdcall callback(const char * id, const char * value);

  static void sendUpdate();

  static void addConstellationIQ(int16_t i, int16_t q);
  static void sendConstellationDiagram();

private:
  static void changeRXSource(const char * value);

  static CircularBuffer<PairInt16_t> constellationData;
};

} // namespace GUI

#endif /* _RADIO_H_ */