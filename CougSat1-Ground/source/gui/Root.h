#ifndef _GUI_ROOT_H_
#define _GUI_ROOT_H_

namespace GUI {

class Root {
public:
  Root() = delete;

  static void __stdcall callback(const char * id, const char * value);

private:
};

} // namespace GUI

#endif /* _ROOT_H_ */