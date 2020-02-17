#ifndef _GUI_COUGSAT1_PAYLOAD_PLANT_H_
#define _GUI_COUGSAT1_PAYLOAD_PLANT_H_

namespace GUI {
namespace CougSat1 {

class PayloadPlant {
public:
  PayloadPlant() = delete;

  static void __stdcall callback(const char * id, const char * value);
};
} // namespace CougSat1
} // namespace GUI

#endif /* _GUI_COUGSAT1_PAYLOAD_PLANT_H_ */