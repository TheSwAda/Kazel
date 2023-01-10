#include "PerspectCamera.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "OpenGlobe/Ellipsoid.h"

namespace Kazel {

/**
 * @brief ���ʼ��ָ��һ���ض��ĵ�
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
   * @brief ����m_centorPoint��m_fixedToLocalRotation��ʹ����̶�����һ��
   * @param ellipsoid ������
   * @param geographi Ŀ����ڴ������ϵ�µľ�γ��
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

  //�����������ϵ����
  double m_range{0};      //��Ŀ��ľ���
  double m_azimuth{0};    //X���������Ŀ����������XOYƽ���ϵ�ͶӰ֮��ļн�
  double m_elevation{0};  //�����Ŀ����������Z��ļнǣ�����0��ʾ��Z������нǣ�С��0��ʾ��Z�Ḻ����ļн�

  //����ƶ�����
  double m_lastX{0};
  double m_lastY{0};
  bool m_leftButtonPressed{false};
  bool m_rightButtonPressed{false};

  //������ſ���
  double m_zoomFactor{5.0};
  double m_zoomRateRangeAdjustment{0};
  double m_maximumZoomRate{0};
  double m_minimumZoomRate{0};

  //��ת����
  double m_rotateFactor{0};
  double m_rotateRateRangeAdjustment{0};
  double m_maximumRotateRate{0};
  double m_minimumRotateRate{0};

  //����
  glm::dvec3 m_centerPoint{0};  //�����������ĵ����꣨ECEF��see https://en.wikipedia.org/wiki/ECEF
  //ECFF TO ENU see https://en.wikipedia.org/wiki/Geographic_coordinate_conversion#From_ECEF_to_ENU
  glm::dmat3 m_fixedToLocalRotation{1.0f};
};
}  // namespace Kazel