#include "kzpch.h"

#include "RenderCommand.h"
#include "Platform/OpenGl/OpenGlRendererAPI.h"

namespace Kazel {
RendererAPI* RenderCommand::m_RenderAPI = new OpenGlRendererAPI;
}  // namespace Kazel