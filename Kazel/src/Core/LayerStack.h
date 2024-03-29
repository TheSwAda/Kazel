#pragma once
#include <vector>

#include "Core.h"
#include "Layer.h"

namespace Kazel {
class LayerStack {
 public:
  LayerStack();
  ~LayerStack();

  void PushLayer(Layer* layer);
  void PushOverLayer(Layer* layer);
  void PopLayer(Layer* layer);
  void PopOverLayer(Layer* layer);

  std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
  std::vector<Layer*>::iterator end() { return m_Layers.end(); }

 private:
  std::vector<Layer*> m_Layers;
  unsigned int m_InsertLayerIndex = 0;
};
}  // namespace Kazel
