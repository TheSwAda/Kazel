#include "kzpch.h"

#include "Layer.h"

namespace Kazel {
Layer::Layer(const std::string& name) : m_DebugName(name) {}
Layer::~Layer() {}
}  // namespace Kazel