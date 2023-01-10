#pragma once

#include "Core/Input.h"

namespace Kazel {
class WindowsInput : public Input {
 protected:
  bool IsKeyPressedImpl(KeyCode keyCode) override;

  bool IsMouseButtonPressedImpl(MouseCode button) override;

  float GetMouseXImpl() override;

  float GetMouseYImpl() override;

  std::pair<float, float> GetMousePosImpl() override;
};
}  // namespace Kazel
