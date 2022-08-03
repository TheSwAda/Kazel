#include "kzpch.h"
#include "OpenGlShader.h"

#include <glm/gtc/type_ptr.hpp>
#include <map>

#include "Core/Log.h"

namespace Kazel {
std::map<std::string, Kazel::ShaderType> extensions = {
    {".vs", ShaderType::ST_Vertex},
    {".vert", ShaderType::ST_Vertex},
    {"_vert.glsl", ShaderType::ST_Vertex},
    {".vert.glsl", ShaderType::ST_Vertex},
    {".gs", ShaderType::ST_Geometry},
    {".geom", ShaderType::ST_Geometry},
    {".geom.glsl", ShaderType::ST_Geometry},
    {".tcs", ShaderType::ST_Tess_Control},
    {".tcs.glsl", ShaderType::ST_Tess_Control},
    {".tes", ShaderType::ST_Tess_Evaluation},
    {".tes.glsl", ShaderType::ST_Tess_Evaluation},
    {".fs", ShaderType::ST_Fragment},
    {".frag", ShaderType::ST_Fragment},
    {"_frag.glsl", ShaderType::ST_Fragment},
    {".frag.glsl", ShaderType::ST_Fragment},
    {".cs", ShaderType::ST_Compute},
    {".cs.glsl", ShaderType::ST_Compute}};

std::map<ShaderType, GLenum> glShaderType{
    {ShaderType::ST_Vertex, GL_VERTEX_SHADER},
    {ShaderType::ST_Fragment, GL_FRAGMENT_SHADER},
    {ShaderType::ST_Geometry, GL_GEOMETRY_SHADER},
    {ShaderType::ST_Tess_Control, GL_TESS_CONTROL_SHADER},
    {ShaderType::ST_Tess_Evaluation, GL_TESS_EVALUATION_SHADER},
    {ShaderType::ST_Compute, GL_COMPUTE_SHADER}};

std::map<GLenum, std::string> glTypeStr{
    {GL_FLOAT, "float"},
    {GL_FLOAT_VEC2, "vec2"},
    {GL_FLOAT_VEC3, "vec3"},
    {GL_FLOAT_VEC4, "vec4"},
    {GL_DOUBLE, "double"},
    {GL_INT, "int"},
    {GL_UNSIGNED_INT, "unsigned int"},
    {GL_BOOL, "bool"},
    {GL_FLOAT_MAT2, "mat2"},
    {GL_FLOAT_MAT3, "mat3"},
    {GL_FLOAT_MAT4, "mat4"},
};
std::shared_ptr<Shader> Shader::Create() { return CreateRef<OpenGlShader>(); }

OpenGlShader::OpenGlShader() : mProgramID(0), mbLinked(false) {}

OpenGlShader::~OpenGlShader() {
  if (mProgramID == 0) return;

  detachAndDeleteShaderObjects();
  glDeleteProgram(mProgramID);
}

void OpenGlShader::compileShaderSource(const std::string& source, ShaderType type) {
  if (mProgramID <= 0) {
    mProgramID = glCreateProgram();
    KZ_CORE_ASSERT(mProgramID != 0, "unable to create shader program.")
  }

  GLuint shaderHandle = glCreateShader(glShaderType.at(type));
  const char* c_code = source.c_str();
  glShaderSource(shaderHandle, 1, &c_code, nullptr);

  glCompileShader(shaderHandle);
  if (checkCompileErrors(shaderHandle, "SHADER")) {
    glAttachShader(mProgramID, shaderHandle);
  }
}

void OpenGlShader::compileShader(const char* fileName, ShaderType type) {
  std::ifstream shaderFile(fileName, std::ios::binary);
  if (!shaderFile) {
    KZ_CORE_ASSERT(false, "failed to open file");
    return;
  }
  std::stringstream shaderStream;
  shaderStream << shaderFile.rdbuf();
  compileShaderSource(shaderStream.str(), type);
}

void OpenGlShader::compileShader(const char* fileName) {
  // Check the file name's extension to determine the shader type
  std::string ext = getExtension(fileName);
  ShaderType type = ShaderType::ST_Vertex;

  auto it = extensions.find(ext);
  if (it != extensions.end()) {
    type = it->second;
  } else {
    KZ_CORE_ERROR("Unrecognized extension: {0}", ext);
    return;
  }
  // Pass the discovered shader type along
  compileShader(fileName, type);
}

void OpenGlShader::printActiveAttribs() {
  GLint numAttribs;
  /** query for the number of active attributes
   *
   * GL_PROGRAM_INPUT indicates that we are querying for information about the
   * program input variables GL_ACTIVE_RESOURCES indicates that we want the
   * number of active resources(in order for a vertex shader input variable to
   * be considered active, it must be used within the vertex shader)
   */
  glGetProgramInterfaceiv(mProgramID, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES,
                          &numAttribs);
  GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION};
  KZ_CORE_TRACE("Active attributes:");

  for (int i = 0; i < numAttribs; ++i) {
    GLint results[3];
    // get the length of the name
    /*
    i:the index of the attribute
    fisrt 3:the number of values in the properties array
    second 3:the size of the buffer that will receive the results
    seventh argument:a pointer to an integer that would receive the
number of results that were written
*/
    glGetProgramResourceiv(mProgramID, GL_PROGRAM_INPUT, i,
                           sizeof(properties) / sizeof(GLenum), properties,
                           sizeof(results) / sizeof(GLint), nullptr, results);
    GLint nameBufSize = results[0] + i;
    char* name = new char[nameBufSize];
    glGetProgramResourceName(mProgramID, GL_PROGRAM_INPUT, i, nameBufSize,
                             nullptr, name);
    KZ_CORE_TRACE("location:{0} {1} ({2})", results[2], name,
                  getTypeString(results[1]));

    delete[] name;
  }
}

void OpenGlShader::printUniforms() {
  GLint numUniforms;
  /** query for the number of active attributes
   *
   * GL_PROGRAM_INPUT indicates that we are querying for information about the
   * program input variables GL_ACTIVE_RESOURCES indicates that we want the
   * number of active resources(in order for a vertex shader input variable to
   * be considered active, it must be used within the vertex shader)
   */
  glGetProgramInterfaceiv(mProgramID, GL_UNIFORM, GL_ACTIVE_RESOURCES,
                          &numUniforms);
  GLenum properties[] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX};
  KZ_CORE_TRACE("Active uniforms:");

  for (int i = 0; i < numUniforms; ++i) {
    GLint results[4];
    // get the length of the name
    glGetProgramResourceiv(mProgramID, GL_UNIFORM, i,
                           sizeof(properties) / sizeof(GLenum), properties,
                           sizeof(results) / sizeof(GLint), nullptr, results);
    GLint nameBufSize = results[0] + i;
    char* name = new char[nameBufSize];
    glGetProgramResourceName(mProgramID, GL_UNIFORM, i, nameBufSize, nullptr,
                             name);
    KZ_CORE_TRACE("location:{0} {1} ({2})", results[2], name,
                  getTypeString(results[1]));

    delete[] name;
  }
}

void OpenGlShader::saveProgram(const std::string& filePath) {
  GLint formats = 0;
  glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
  if (formats < 1) {
    KZ_CORE_ERROR("Driver does not support any binary formats.");
    return;
  }

  GLint length = 0;
  glGetProgramiv(mProgramID, GL_PROGRAM_BINARY_LENGTH, &length);
  std::vector<GLubyte> buffer(length);
  GLenum format = 0;

  glGetProgramBinary(mProgramID, length, nullptr, &format, buffer.data());

  try {
    std::ofstream of(filePath, std::ios_base::binary);
    of.write(reinterpret_cast<char*>(buffer.data()), length);
    of.close();
  } catch (const std::exception& e) {
    KZ_CORE_ERROR("failed to open {0}, result:{1}", filePath, e.what());
    return;
  }

  KZ_CORE_TRACE("Writing to {0}, binary format = {1}", filePath, format);
}

void OpenGlShader::use() {
  if (mProgramID <= 0) {
    KZ_CORE_ERROR("shader program is empty.");
    return;
  }

  if (!mbLinked) {
    glLinkProgram(mProgramID);
    if (checkCompileErrors(mProgramID, "PROGRAM")) {
      mbLinked = true;
    }
    detachAndDeleteShaderObjects();
  }

  if (mbLinked) glUseProgram(mProgramID);
}

void OpenGlShader::setUniform(const char* name, float x, float y, float z) {
  GLint loc = getUniformLocation(name);
  glUniform3f(loc, x, y, z);
}

void OpenGlShader::setUniform(const char* name, const glm::vec3& v) {
  this->setUniform(name, v.x, v.y, v.z);
}

void OpenGlShader::setUniform(const char* name, const glm::vec4& v) {
  GLint loc = getUniformLocation(name);
  glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void OpenGlShader::setUniform(const char* name, const glm::vec2& v) {
  GLint loc = getUniformLocation(name);
  glUniform2f(loc, v.x, v.y);
}

void OpenGlShader::setUniform(const char* name, const glm::mat4& m) {
  GLint loc = getUniformLocation(name);
  glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

void OpenGlShader::setUniform(const char* name, const glm::mat3& m) {
  GLint loc = getUniformLocation(name);
  glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}

void OpenGlShader::setUniform(const char* name, float val) {
  GLint loc = getUniformLocation(name);
  glUniform1f(loc, val);
}

void OpenGlShader::setUniform(const char* name, int val) {
  GLint loc = getUniformLocation(name);
  glUniform1i(loc, val);
}

void OpenGlShader::setUniform(const char* name, unsigned int val) {
  GLint loc = getUniformLocation(name);
  glUniform1ui(loc, val);
}

void OpenGlShader::setUniform(const char* name, bool val) {
  int loc = getUniformLocation(name);
  glUniform1i(loc, val);
}

void OpenGlShader::setUniform(const char* name, const int* value,
                              uint32_t count) {
  int loc = getUniformLocation(name);
  glUniform1iv(loc, count, value);
}

const bool OpenGlShader::checkCompileErrors(GLuint shader, std::string type) {
  GLint success;
  GLchar infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    // GLint logLen;
    // glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      KZ_CORE_ERROR("Shader::compilation_error of type {0}:{1}", type, infoLog);
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      KZ_CORE_ERROR("Shader::compilation_error of type {0}:{1}", type, infoLog);
    }
  }
  return success;
}

std::string OpenGlShader::getExtension(const char* fileName) {
  std::string nameStr(fileName);

  size_t dotLoc = nameStr.find_last_of('.');
  if (dotLoc != std::string::npos) {
    std::string ext = nameStr.substr(dotLoc);
    if (ext == ".glsl") {
      size_t loc = nameStr.find_last_of('.', dotLoc - 1);
      if (loc == std::string::npos) {
        loc = nameStr.find_last_of('_', dotLoc - 1);
      }
      if (loc != std::string::npos) {
        return nameStr.substr(loc);
      }
    } else {
      return ext;
    }
  }
  return "";
}

const char* OpenGlShader::getTypeString(GLenum type) {
  // There are many more types than are covered here, but
  // these are the most common
  auto it = glTypeStr.find(type);

  if (it == glTypeStr.end()) return "?";
  return it->second.c_str();
}

void OpenGlShader::detachAndDeleteShaderObjects() {
  // Detach and delete the shader objects (if they are not already removed)
  GLint numShaders = 0;
  glGetProgramiv(mProgramID, GL_ATTACHED_SHADERS, &numShaders);
  std::vector<GLuint> shaderNames(numShaders);
  glGetAttachedShaders(mProgramID, numShaders, NULL, shaderNames.data());
  for (GLuint shader : shaderNames) {
    glDetachShader(mProgramID, shader);
    glDeleteShader(shader);
  }
}

GLint OpenGlShader::getUniformLocation(const char* name) {
  auto it = uniformLocations.find(name);
  if (it == uniformLocations.end()) {
    use();
    auto location = glGetUniformLocation(mProgramID, name);
    if (location == -1) {
      KZ_CORE_ERROR("{0} does not correspond to an active uniform variable",
                    name);
      return -1;
    }
    bool bOk;
    std::tie(it, bOk) = uniformLocations.emplace(name, location);
  }
  return it->second;
}
}  // namespace Kazel