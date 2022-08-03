#pragma once
#include <memory>

#ifdef _WIN32
#ifdef _WIN64
#define KZ_PLATFORM_WINDOWS
#else
#error X86 Are Not Support!!
#endif  // _WIN64
#endif

#ifdef KZ_DEBUG
#define KZ_ENABLE_ASSERT
#endif  // KZ_DEBUG

#ifdef KZ_ENABLE_ASSERT

#define KZ_ASSERT_STRINGIFY_HELPER(x) #x
#define KZ_ASSERT_STRINGIFY(x) KZ_ASSERT_STRINGIFY_HELPER(x)
#define KZ_ASSERT_STRING(cond)                   \
  KZ_CORE_ERROR("\"" cond "\" in file " __FILE__ \
                ", line " KZ_ASSERT_STRINGIFY_HELPER(__LINE__))

#define KZ_CHECK(cond)       \
  if (!(cond)) {             \
    KZ_ASSERT_STRING(#cond); \
    __debugbreak();          \
  }

#define KZ_ASSERT(x, ...)                         \
  if (!(x)) {                                     \
    KZ_ERROR("Assertion Faile:{0}", __VA_ARGS__); \
    __debugbreak();                               \
  }

#define KZ_CORE_CHECK(cond)  \
  if (!(cond)) {             \
    KZ_ASSERT_STRING(#cond); \
    __debugbreak();          \
  }

#define KZ_CORE_ASSERT(cond, ...)                      \
  if (!(cond)) {                                       \
    KZ_CORE_ERROR("Assertion Faile:{0}", __VA_ARGS__); \
    __debugbreak();                                    \
  }

#else
#define KZ_ASSERT(x, ...) ;
#define KZ_CORE_ASSERT(x, ...)
#define KZ_CORE_CHECK(x, ...) 
#endif  // KZ_ENABLE_ASSERT

#define BIT(x) (1 << x)
#define KZ_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

namespace Kazel {
template <typename T>
using Scope = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr Scope<T> CreateScope(Args&&... args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args&&... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}
};  // namespace Kazel
