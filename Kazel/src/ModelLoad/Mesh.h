#pragma once

#include "Renderer/VertexArray.h"
#include "Renderer/Texture.h"
#include "Renderer/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Kazel {
struct SMeshVertex {
  SMeshVertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texCoord)
      : Position(position), Normal(normal), TexCoord(texCoord) {}
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoord;
};

class CMesh {
 public:
  using TextureType = int;
  enum : TextureType {
    Diffuse,  //漫反射贴图
    Specular  //镜面反射贴图
  };

  //顶点数据, 顶点索引，
  CMesh(const std::vector<SMeshVertex>& vertices,
        const std::vector<unsigned int>& indices);

  void Draw(const Ref<Shader>& shader);
  void DrawInstanced(const Ref<Shader>& shader, uint32_t count);

  Ref<VertexArray> GetVAO() { return m_VAO; }

  void AddTextures(TextureType type,
                   const std::vector<Ref<Texture2D>>& textures) {
    m_Textures[type].insert(m_Textures[type].end(), textures.begin(),
                            textures.end());
  }

 private:
  void setTexture(const Ref<Shader>& shader);

 private:
  Ref<VertexArray> m_VAO;
  std::map<TextureType, std::vector<Ref<Texture2D>>> m_Textures;
  const static Kazel::BufferLayout ms_Layout;
};
}  // namespace Kazel