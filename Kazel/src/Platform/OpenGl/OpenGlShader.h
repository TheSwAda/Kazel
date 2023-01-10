#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include "Renderer/Shader.h"
namespace Kazel {
class OpenGlShader : public Shader {
 public:
  OpenGlShader();
  ~OpenGlShader();

  // for debug
  void printActiveAttribs() override;
  void printUniforms() override;

  virtual void compileShader(const char* fileName) override;
  virtual void compileShader(const char* fileName, ShaderType type) override;
  virtual void compileShaderSource(const std::string& source,
                                   ShaderType type) override;

  void saveProgram(const std::string& filePath) override;

  void use() override;

  void setUniform(const char* name, float x, float y, float z) override;

  void setUniform(const char* name, const glm::vec2& v) override;

  void setUniform(const char* name, const glm::vec3& v) override;

  void setUniform(const char* name, const glm::vec4& v) override;

  void setUniform(const char* name, const glm::mat4& m) override;

  void setUniform(const char* name, const glm::mat3& m) override;

  void setUniform(const char* name, float val) override;

  void setUniform(const char* name, int val) override;

  void setUniform(const char* name, bool val) override;

  void setUniform(const char* name, unsigned int val) override;

  void setUniform(const char* name, const int* value, uint32_t count) override;

 private:
  const bool checkCompileErrors(GLuint shader, std::string type);
  std::string getExtension(const char* fileName);
  const char* getTypeString(GLenum type);
  void detachAndDeleteShaderObjects();
  GLint getUniformLocation(const char* name);

 private:
  std::map<std::string, int> uniformLocations;  
  uint32_t mProgramID;
  bool mbLinked;
};
}  // namespace Kazel