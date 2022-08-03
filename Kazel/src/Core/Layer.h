#pragma once
#include <string>

#include "Core.h"
#include "Events/Event.h"

namespace Kazel {
class Layer {
 public:
  Layer(const std::string& name = "Layer");
  virtual ~Layer();

  virtual void OnAttach() {}
  virtual void OnDetach() {}
  virtual void OnUpdate(float timeStep) {}
  virtual void OnImGuiRender() {}
  virtual void OnEvent(Event& e) {}

  const std::string& GetName() const { return m_DebugName; }

 protected:
  std::string m_DebugName;
};
}  // namespace Kazel