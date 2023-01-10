#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <memory>

#include "Core.h"

namespace Kazel {
class Log {
 public:
  static void Init();
  static std::shared_ptr<spdlog::logger>& GetCoreLogger() {
    return m_CoreLogger;
  }
  static std::shared_ptr<spdlog::logger>& GetClientLogger() {
    return m_ClientLogger;
  }

 private:
  static std::shared_ptr<spdlog::logger> m_CoreLogger;
  static std::shared_ptr<spdlog::logger> m_ClientLogger;
};

// get core logger
#define KZ_CORE_TRACE(...) ::Kazel::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define KZ_CORE_INFO(...) ::Kazel::Log::GetCoreLogger()->info(__VA_ARGS__)
#define KZ_CORE_WARN(...) ::Kazel::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define KZ_CORE_ERROR(...) ::Kazel::Log::GetCoreLogger()->error(__VA_ARGS__);
#define KZ_CORE_FATAL(...) ::Kazel::Log::GetCoreLogger()->critical(__VA_ARGS__)

// get app logger
#define KZ_INFO(...) ::Kazel::Log::GetClientLogger()->info(__VA_ARGS__)
#define KZ_TRACE(...) ::Kazel::Log::GetClientLogger()->trace(__VA_ARGS__)
#define KZ_ERROR(...) ::Kazel::Log::GetClientLogger()->error(__VA_ARGS__)
#define KZ_WARN(...) ::Kazel::Log::GetClientLogger()->warn(__VA_ARGS__)
#define KZ_FATAL(...) ::Kazel::Log::GetClientLogger()->critical(__VA_ARGS__)
}  // namespace Kazel
