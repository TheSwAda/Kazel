#pragma once

#include <sstream>
#include "Core/KeyMouseCode.h"

#include "Event.h"
namespace Kazel {
class KeyEvent : public Event {
 public:
  KeyCode GetKeyCode() const { return m_keyCode; }

  EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
 protected:
  KeyEvent(const KeyCode keycode) : m_keyCode(keycode) {}

  KeyCode m_keyCode;
};

class KeyPressedEvent : public KeyEvent {
 public:
  KeyPressedEvent(const KeyCode keycode, const uint16_t repeatCount)
      : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

  uint16_t GetRepeatCount() const { return m_RepeatCount; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyPressedEvent: " << static_cast<int>(m_keyCode) << " (" << m_RepeatCount
       << " repeats)";
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyPressed)
 private:
  uint16_t m_RepeatCount;
};

class KeyReleasedEvent : public KeyEvent {
 public:
  KeyReleasedEvent(const KeyCode keycode) : KeyEvent(keycode) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyReleasedEvent: " << static_cast<int>(m_keyCode);
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent : public KeyEvent {
 public:
  KeyTypedEvent(const KeyCode keycode) : KeyEvent(keycode) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyTypedEvent: " << static_cast<int>(m_keyCode);
    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyTyped)
};
}  // namespace Kazel