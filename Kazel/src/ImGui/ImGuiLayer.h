#pragma once

#include "Core/Layer.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

namespace Kazel {
class ImGuiLayer : public Layer {
 public:
  ImGuiLayer();
  ~ImGuiLayer();
  void OnAttach() override;
  void OnDetach() override;
  void OnImGuiRender() override;

  void Begin();
  void End();

 private:
  float m_time = 0.0f;
};
}  // namespace Kazel
