#pragma once
#include <any>
#include <map>
#include <memory>

#include "glm/glm.hpp"
#include "Core/Core.h"

namespace Kazel {
enum class ShaderType {
  ST_Vertex,
  ST_Fragment,
  ST_Geometry,
  ST_Tess_Control,
  ST_Tess_Evaluation,
  ST_Compute
};

using ShaderFormat = int;
enum : ShaderFormat { SF_Normal, SF_SPIP_V };

class Shader {
 public:
  virtual ~Shader() {}

  // for debug
  virtual void printActiveAttribs() {}
  virtual void printUniforms() {}

  virtual void saveProgram(const std::string& filePath) = 0;

  virtual void compileShader(const char* fileName) = 0;
  virtual void compileShader(const char* fileName, ShaderType type) = 0;
  virtual void compileShaderSource(const std::string& source, ShaderType type) = 0;

  virtual void use() = 0;

  virtual void setUniform(const char* name, float x, float y, float z) = 0;
  virtual void setUniform(const char* name, const glm::vec2& v) = 0;
  virtual void setUniform(const char* name, const glm::vec3& v) = 0;
  virtual void setUniform(const char* name, const glm::vec4& v) = 0;
  virtual void setUniform(const char* name, const glm::mat4& m) = 0;
  virtual void setUniform(const char* name, const glm::mat3& m) = 0;
  virtual void setUniform(const char* name, float val) = 0;
  virtual void setUniform(const char* name, int val) = 0;
  virtual void setUniform(const char* name, bool val) = 0;
  virtual void setUniform(const char* name, unsigned int val) = 0;
  virtual void setUniform(const char* name, const int* value,
                          uint32_t count) = 0;

  static Ref<Shader> Create();

  inline const static std::map<Kazel::ShaderType, std::string> s_ShaderName{
      {ShaderType::ST_Vertex, "Vertex"},
      {ShaderType::ST_Fragment, "Fragment"},
      {ShaderType::ST_Geometry, "Geometry"},
      {ShaderType::ST_Tess_Control, "Tess_Control"},
      {ShaderType::ST_Tess_Evaluation, "Evaluation"},
  };
};
}  // namespace Kazel