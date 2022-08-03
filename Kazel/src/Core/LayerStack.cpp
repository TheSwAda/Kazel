#include "kzpch.h"

#include "LayerStack.h"

namespace Kazel {
LayerStack::LayerStack() {}

LayerStack::~LayerStack() {
  for (auto* layer : m_Layers) delete layer;
}

void LayerStack::PushLayer(Layer* layer) {
  m_Layers.emplace(m_Layers.begin() + m_InsertLayerIndex, layer);
  m_InsertLayerIndex++;
}

void LayerStack::PushOverLayer(Layer* layer) { m_Layers.emplace_back(layer); }

void LayerStack::PopLayer(Layer* layer) {
  auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
  if (it != m_Layers.end()) {
    m_Layers.erase(it);
    m_InsertLayerIndex--;
  }
}

void LayerStack::PopOverLayer(Layer* layer) {
  auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
  if (it != m_Layers.end()) {
    m_Layers.erase(it);
  }
}
}  // namespace Kazel