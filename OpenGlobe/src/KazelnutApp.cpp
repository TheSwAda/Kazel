#include <Kazel.h>
#include <Core/EntryPoint.h>
#include "GlobeLayer.h"

class SandBoxApp : public Kazel::Application {
 public:
  SandBoxApp(const Kazel::WindowProps& props) : m_WindowProps(props), Application(props) {
    PushLayer(new GlobeLayer);
  }
  ~SandBoxApp() {}

 private:
  Kazel::WindowProps m_WindowProps;
};

Kazel::Application* Kazel::CreateApplication() {
  Kazel::WindowProps props("SandBox3D", 1600, 1000);

  return new SandBoxApp(props);
}