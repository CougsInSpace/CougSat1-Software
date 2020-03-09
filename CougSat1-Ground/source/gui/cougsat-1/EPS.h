#ifndef _GUI_COUGSAT1_EPS_H_
#define _GUI_COUGSAT1_EPS_H_

namespace GUI {
namespace CougSat1 {

class EPS {
public:
  EPS() = delete;

  static void __stdcall callback(const char * id, const char * value);

  static void sendUpdate();
};
} // namespace CougSat1
} // namespace GUI

#endif /* _GUI_COUGSAT1_EPS_H_ */