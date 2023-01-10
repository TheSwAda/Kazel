#pragma once

#include "Core/Core.h"
#include <functional>
#include <ostream>

namespace Kazel {
// Events in Hazel are currently blocking, meaning when an event occurs it
// immediately gets dispatched and must be dealt with right then an there.
// For the future, a better strategy might be to buffer events in an event
// bus and process them during the "event" part of the update stage.

enum class EventType {
  None = 0,
  WindowClose,
  WindowResize,
  WindowFocus,
  WindowLostFocus,
  WindowMoved,
  AppTick,
  AppUpdate,
  AppRender,
  KeyPressed,
  KeyReleased,
  KeyTyped,
  MouseButtonPressed,
  MouseButtonReleased,
  MouseMoved,
  MouseScrolled
};

enum EventCategory {
  None = 0,
  EventCategoryApplication = BIT(0),
  EventCategoryInput = BIT(1),
  EventCategoryKeyboard = BIT(2),
  EventCategoryMouse = BIT(3),
  EventCategoryMouseButton = BIT(4)
};

#define EVENT_CLASS_TYPE(type)                                                \
  static EventType GetStaticType() { return EventType::type; }                \
  virtual EventType GetEventType() const override { return GetStaticType(); } \
  virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
  virtual int GetCategoryFlags() const override { return category; }

class Event {
 public:
  virtual ~Event() = default;

  bool Handled = false;

  virtual EventType GetEventType() const = 0;
  virtual const char* GetName() const = 0;
  virtual int GetCategoryFlags() const = 0;
  virtual std::string ToString() const { return GetName(); }

  bool IsInCategory(EventCategory category) {
    return GetCategoryFlags() & category;
  }
};

/**
 * @brief �¼��ַ���
*/
class EventDispatcher {
 public:
  EventDispatcher(Event& event) : m_event(event) {}

  // F will be deduced by the compiler
  
  /**
   * @brief �¼�����
   * @tparam T �¼�����
   * @tparam F �¼�������
   * @param func 
   * @return ����������¼�������T��ͬʱ������true�����򷵻�false
  */
  template <typename T, typename F>
  bool Dispatch(const F& func) {
    if (m_event.GetEventType() == T::GetStaticType()) {
      m_event.Handled |= func(static_cast<T&>(m_event));
      return true;
    }
    return false;
  }

 private:
  Event& m_event;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e) {
  return os << e.ToString();
}
}  // namespace Kazel
