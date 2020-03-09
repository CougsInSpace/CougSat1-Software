#ifndef _GUI_COUGSAT1_ADCS_H_
#define _GUI_COUGSAT1_ADCS_H_

namespace GUI {
namespace CougSat1 {

class ADCS {
public:
  ADCS() = delete;

  static void __stdcall callback(const char * id, const char * value);

  static void sendUpdate();
};
} // namespace CougSat1
} // namespace GUI

#endif /* _GUI_COUGSAT1_ADCS_H_ */