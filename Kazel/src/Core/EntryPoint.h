#pragma once

#ifdef KZ_PLATFORM_WINDOWS

extern Kazel::Application* Kazel::CreateApplication();
int main() {
  KZ_PROFILE_BEGIN_SESSION("StartUp", "KazelProfile-StartUp.json");
  auto* app = Kazel::CreateApplication();
  KZ_PROFILE_END_SESSION();

  KZ_PROFILE_BEGIN_SESSION("Run", "KazelProfile-Run.json");
  app->Run();
  KZ_PROFILE_END_SESSION();

  KZ_PROFILE_BEGIN_SESSION("End", "KazelProfile-End.json");
  delete app;
  KZ_PROFILE_END_SESSION();

  return 0;
}
#endif  // HZ_PLATFROM_WINDOWS
