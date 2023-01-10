#include "CameraLookAtPoint.h"

namespace Kazel {

CameraLookAtPoint::CameraLookAtPoint(uint32_t width, uint32_t height, const Ellipsoid& ellipsoid)
    : m_width(width),
      m_heigh(height),
      m_azimuth(glm::radians(90.0)),
      m_elevation(glm::radians(90.0)),
      m_camera(45, width * 1.0 / height, 0.01 * ellipsoid.MaximumRadius(), 10 * ellipsoid.MaximumRadius()) {
  AdjustFactor(ellipsoid);
  m_camera.ZoomToTarget(ellipsoid.MaximumRadius());
  m_camera.UpdateView();
  m_camera.UpdateProjection();

  m_range = m_camera.Distance();
  m_centerPoint = m_camera.m_target;
}

void CameraLookAtPoint::ViewPort(const Ellipsoid& ellipsoid, const Geodetic3D& geographic) {
  m_centerPoint = ellipsoid.ToVector3d(geographic);

  // Fixed to East-North-Up rotation, from Wikipedia's "Geodetic System" topic.
  double cosLon = std::cos(geographic.m_longitude);
  double cosLat = std::cos(geographic.m_latitude);
  double sinLon = std::sin(geographic.m_longitude);
  double sinLat = std::sin(geographic.m_latitude);
  m_fixedToLocalRotation = glm::dmat3(-sinLon, cosLon, 0.0, -sinLat * cosLon, -sinLat * sinLon, cosLat, cosLat * cosLon,
                                      cosLat * sinLon, sinLat);

  UpdateCameraFromParameters();
}

void CameraLookAtPoint::OnEvent(Event& event) {
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch<MouseScrolledEvent>(KZ_BIND_EVENT_FN(CameraLookAtPoint::OnMouseScrolled));
  dispatcher.Dispatch<WindowResizeEvent>(KZ_BIND_EVENT_FN(CameraLookAtPoint::OnWindowResized));
  dispatcher.Dispatch<MouseButtonPressedEvent>(KZ_BIND_EVENT_FN(CameraLookAtPoint::OnMousePressed));
  dispatcher.Dispatch<MouseButtonReleasedEvent>(KZ_BIND_EVENT_FN(CameraLookAtPoint::OnMouseReleased));
  dispatcher.Dispatch<MouseMovedEvent>(KZ_BIND_EVENT_FN(CameraLookAtPoint::OnMouseMoved));
}

bool CameraLookAtPoint::OnWindowResized(WindowResizeEvent& event) {
  m_width = event.GetWidth();
  m_heigh = event.GetHeight();
  m_camera.m_apsect = m_width * 1.0 / m_heigh;
  m_camera.UpdateProjection();
  return false;
}

void CameraLookAtPoint::UpdateCameraFromParameters() {
  m_camera.m_target = m_centerPoint;
  double rangeTimesSinElevation = m_range * std::sin(m_elevation);
  auto positionInENU = glm::vec3{rangeTimesSinElevation * std::cos(m_azimuth), m_range * std::cos(m_elevation),
                                 rangeTimesSinElevation * std::sin(m_azimuth)};
  auto localToFixed = glm::transpose(m_fixedToLocalRotation);
  //将ENU转换到ECFF，先旋转，在平移
  m_camera.m_position = (localToFixed * positionInENU + m_centerPoint);
  m_camera.UpdateView();
}

void CameraLookAtPoint::UpdateParametersFromCamera() {}

void CameraLookAtPoint::AdjustFactor(const Ellipsoid& ellipsoid) {
  m_zoomRateRangeAdjustment = ellipsoid.MaximumRadius();
  m_maximumZoomRate = std::numeric_limits<double>::max();
  m_minimumZoomRate = ellipsoid.MaximumRadius() / 100.0;
  m_rotateFactor = 1 / ellipsoid.MaximumRadius();
  m_rotateRateRangeAdjustment = ellipsoid.MaximumRadius();
  m_maximumRotateRate = 1.0;
  m_minimumRotateRate = 1.0 / 5000.0;
}

bool CameraLookAtPoint::OnMousePressed(MouseButtonPressedEvent& event) {
  if (event.GetMouseButton() == MouseCode::BUTTON_LEFT) {
    m_leftButtonPressed = true;
  } else if (event.GetMouseButton() == MouseCode::BUTTON_RIGHT) {
    m_rightButtonPressed = true;
  }
  m_lastX = event.GetX();
  m_lastY = event.GetY();
  return false;
}

bool CameraLookAtPoint::OnMouseReleased(MouseButtonReleasedEvent& event) {
  if (event.GetMouseButton() == MouseCode::BUTTON_LEFT) {
    m_leftButtonPressed = false;
  } else if (event.GetMouseButton() == MouseCode::BUTTON_RIGHT) {
    m_rightButtonPressed = false;
  }
  return false;
}

bool CameraLookAtPoint::OnMouseMoved(MouseMovedEvent& event) {
  if (!m_leftButtonPressed && !m_rightButtonPressed)
    return false;
  double movex = event.GetX() - m_lastX;
  double movey = event.GetY() - m_lastY;

  auto Rotate = [this](double movex, double movey) {
    double rotateRate = m_rotateFactor * (m_range - m_rotateRateRangeAdjustment);
    if (rotateRate > m_maximumRotateRate) {
      rotateRate = m_maximumRotateRate;
    }
    if (rotateRate < m_minimumRotateRate) {
      rotateRate = m_minimumRotateRate;
    }

    double azimuthWindowRatio = movex / m_width;
    double elevationWindowRatio = movey / m_heigh;

    m_azimuth += rotateRate * azimuthWindowRatio * glm::two_pi<double>();
    m_elevation -= rotateRate * elevationWindowRatio * glm::pi<double>();

    while (m_azimuth > glm::pi<double>()) {
      m_azimuth -= glm::two_pi<double>();
    }
    while (m_azimuth < -glm::pi<double>()) {
      m_azimuth += glm::two_pi<double>();
    }

    if (m_elevation < 0) {
      m_elevation = 0;
    } else if (m_elevation > glm::pi<double>()) {
      m_elevation = glm::pi<double>();
    }
  };

  auto Zoom = [this](double movex, double movey) {
    double zoomRate = m_zoomFactor * (m_range - m_zoomRateRangeAdjustment);
    if (zoomRate > m_maximumZoomRate) {
      zoomRate = m_maximumZoomRate;
    }
    if (zoomRate < m_minimumZoomRate) {
      zoomRate = m_minimumZoomRate;
    }

    double rangeWindowRatio = movey / m_heigh;
    m_range -= zoomRate * rangeWindowRatio;
  };

  //rotate
  if (m_leftButtonPressed) {
    Rotate(movex, movey);
  } else if (m_rightButtonPressed) {
    Zoom(movex, movey);
  }
  UpdateCameraFromParameters();
  m_lastX = event.GetX();
  m_lastY = event.GetY();

  return true;
}

bool CameraLookAtPoint::OnMouseScrolled(MouseScrolledEvent& event) {
  m_camera.m_fovy -= event.GetYOffset() * 0.5f;

  if (m_camera.m_fovy < 0.1f)
    m_camera.m_fovy = 0.1f;
  if (m_camera.m_fovy > 90.0f)
    m_camera.m_fovy = 90.0f;

  m_camera.UpdateProjection();
  return true;
}
}  // namespace Kazel