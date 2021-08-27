#ifndef _LIBRARY_COMPONENT_COMMUNICATIONS_SESSION_H_
#define _LIBRARY_COMPONENT_COMMUNICATIONS_SESSION_H_

#include "Frame.h"

#include <memory>
#include <cstdint>

namespace Communications {

class Session {
public:
  Session();
  ~Session();

  void add(std::unique_ptr<Frame> frame);

  uint16_t getID();

  bool isTXEmpty();

  std::unique_ptr<Frame> getNextFrame();
};

} // namespace Communications

#endif /* _LIBRARY_COMPONENT_COMMUNICATIONS_SESSION_H_ */