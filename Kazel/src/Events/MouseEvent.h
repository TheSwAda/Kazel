#pragma once

#include <sstream>
#include "Core/KeyMouseCode.h"
#include "Event.h"
namespace Kazel {
class MouseMovedEvent : public Event {
 public:
  MouseMovedEvent(const double x, const double y) : m_MouseX(x), m_MouseY(y) {}

  double GetX() const { return m_MouseX; }
  double GetY() const { return m_MouseY; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseMoved)
  EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
 private:
  double m_MouseX, m_MouseY;
};

class MouseScrolledEvent : public Event {
 public:
  MouseScrolledEvent(const double xOffset, const double yOffset)
      : m_XOffset(xOffset), m_YOffset(yOffset) {}

  double GetXOffset() const { return m_XOffset; }
  double GetYOffset() const { return m_YOffset; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseScrolled)
  EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
 private:
  double m_XOffset, m_YOffset;
};

class MouseButtonEvent : public Event {
 public:
  MouseCode GetMouseButton() const { return m_button; }

  double GetX() const { return m_xpos; }
  double GetY() const { return m_ypos; }

  EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
 protected:
  MouseButtonEvent(MouseCode button, double x, double y) : m_button(button), m_xpos(x), m_ypos(y) {}

  MouseCode m_button;
  double m_xpos{0};
  double m_ypos{0};
};

class MouseButtonPressedEvent : public MouseButtonEvent {
 public:
  MouseButtonPressedEvent(MouseCode button, double x, double y) : MouseButtonEvent(button, x, y) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseButtonPressedEvent: " << static_cast<int>(m_button);
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
 public:
  MouseButtonReleasedEvent(MouseCode button, double x, double y) : MouseButtonEvent(button, x, y) {}

  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseButtonReleasedEvent: " << static_cast<int>(m_button);
    return ss.str();
  }

  EVENT_CLASS_TYPE(MouseButtonReleased)
};
}  // namespace Kazel
