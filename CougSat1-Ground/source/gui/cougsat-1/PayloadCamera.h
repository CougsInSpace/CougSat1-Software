#ifndef _GUI_COUGSAT1_PAYLOAD_CAMERA_H_
#define _GUI_COUGSAT1_PAYLOAD_CAMERA_H_

namespace GUI {
namespace CougSat1 {

class PayloadCamera {
public:
  PayloadCamera() = delete;

  static void __stdcall callback(const char * id, const char * value);
};
} // namespace CougSat1
} // namespace GUI

#endif /* _GUI_COUGSAT1_PAYLOAD_CAMERA_H_ */