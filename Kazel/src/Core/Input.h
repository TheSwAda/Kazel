#pragma once

#include <utility>
#include "Core.h"
#include "KeyMouseCode.h"

namespace Kazel {
class Input {
 public:
  static bool IsKeyPressed(KeyCode keyCode) {
    return s_Instance->IsKeyPressedImpl(keyCode);
  }
  static bool IsMouseButtonPressed(MouseCode button) {
    return s_Instance->IsMouseButtonPressedImpl(button);
  }
  static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
  static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
  static std::pair<float, float> GetMousePos() {
    return s_Instance->GetMousePosImpl();
  }

 protected:
  virtual bool IsKeyPressedImpl(KeyCode keyCode) = 0;
  virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;

  virtual float GetMouseXImpl() = 0;
  virtual float GetMouseYImpl() = 0;
  virtual std::pair<float, float> GetMousePosImpl() = 0;

 private:
  static Input* s_Instance;
};
}  // namespace Kazel
