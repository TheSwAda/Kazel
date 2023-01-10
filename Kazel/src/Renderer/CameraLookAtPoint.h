#include "PerspectCamera.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "OpenGlobe/Ellipsoid.h"

namespace Kazel {

/**
 * @brief 相机始终指向一个特定的点
 * 
*/
class CameraLookAtPoint {
 public:
  /**
   * @brief 
   * @param width 
   * @param height 
   * @param ellipsoid 
  */
  CameraLookAtPoint(uint32_t width, uint32_t height, const Ellipsoid& ellipsoid);
  PerspectiveCamera& GetCamera() { return m_camera; }

  /**
   * @brief 设置m_centorPoint和m_fixedToLocalRotation，使相机固定看向一点
   * @param ellipsoid 椭球体
   * @param geographi 目标点在大地坐标系下的经纬高
  */
  void ViewPort(const Ellipsoid& ellipsoid, const Geodetic3D& geographic);
  void OnEvent(Event& event);

 private:
  bool OnWindowResized(WindowResizeEvent& event);
  bool OnMousePressed(MouseButtonPressedEvent& event);
  bool OnMouseReleased(MouseButtonReleasedEvent& event);
  bool OnMouseMoved(MouseMovedEvent& event);
  bool OnMouseScrolled(MouseScrolledEvent& event);

  void UpdateCameraFromParameters();
  void UpdateParametersFromCamera();

  void AdjustFactor(const Ellipsoid& ellipsoid);

 private:
  PerspectiveCamera m_camera;
  uint32_t m_width;
  uint32_t m_heigh;

  //相机的球坐标系定义
  double m_range{0};      //与目标的距离
  double m_azimuth{0};    //X轴与相机和目标点的连线在XOY平面上的投影之间的夹角
  double m_elevation{0};  //相机和目标点的连线与Z轴的夹角，大于0表示与Z轴正向夹角，小于0表示与Z轴负方向的夹角

  //鼠标移动控制
  double m_lastX{0};
  double m_lastY{0};
  bool m_leftButtonPressed{false};
  bool m_rightButtonPressed{false};

  //鼠标缩放控制
  double m_zoomFactor{5.0};
  double m_zoomRateRangeAdjustment{0};
  double m_maximumZoomRate{0};
  double m_minimumZoomRate{0};

  //旋转控制
  double m_rotateFactor{0};
  double m_rotateRateRangeAdjustment{0};
  double m_maximumRotateRate{0};
  double m_minimumRotateRate{0};

  //其他
  glm::dvec3 m_centerPoint{0};  //相机看向的中心点坐标（ECEF）see https://en.wikipedia.org/wiki/ECEF
  //ECFF TO ENU see https://en.wikipedia.org/wiki/Geographic_coordinate_conversion#From_ECEF_to_ENU
  glm::dmat3 m_fixedToLocalRotation{1.0f};
};
}  // namespace Kazel