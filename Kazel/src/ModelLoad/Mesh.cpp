#include "kzpch.h"
#include "Mesh.h"
#include "Renderer/Renderer.h"

namespace Kazel {
CMesh::CMesh(const std::vector<SMeshVertex>& vertices,
             const std::vector<unsigned int>& indices) {
  m_VAO = Kazel::VertexArray::Create();
  const float* begin =
      static_cast<const float*>(glm::value_ptr(vertices[0].Position));
  const auto& VBO =
      Kazel::VertexBuffer::Create(begin, vertices.size() * sizeof(SMeshVertex));

  VBO->SetLayout(ms_Layout);
  m_VAO->AddVertexBuffer(VBO);
  const auto& EBO = Kazel::IndexBuffer::Create(indices.data(), indices.size());
  m_VAO->SetIndexBuffer(EBO);
}

void CMesh::Draw(const Ref<Shader>& shader) {
  setTexture(shader);
  m_VAO->Bind();
  RenderCommand::Draw(m_VAO,PrimitiveType::Triangles);
}

void CMesh::DrawInstanced(const Ref<Shader>& shader, uint32_t count) {
  setTexture(shader);
  m_VAO->Bind();
  RenderCommand::DrawIndexedInstanced(m_VAO, count);
}

void CMesh::setTexture(const Ref<Shader>& shader) {
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;

  char name[50] = {};
  int count = 0;
  auto it = m_Textures.find(CMesh::Diffuse);
  if (it != m_Textures.end()) {
    sprintf_s(name, "u_Material.Diffuse%d", diffuseNr++);
    for (const auto& texture : it->second) {
      shader->setUniform(name, count);
      texture->Bind(count++);
    }
  }

  it = m_Textures.find(CMesh::Specular);
  if (it != m_Textures.end()) {
    sprintf_s(name, "u_Material.Specular%d", specularNr++);
    for (const auto& texture : it->second) {
      shader->setUniform(name, count);
      texture->Bind(count++);
    }
  }
}

const Kazel::BufferLayout CMesh::ms_Layout = {
    {Kazel::ShaderDataType::Float3, "a_Position"},
    {Kazel::ShaderDataType::Float3, "a_Normal"},
    {Kazel::ShaderDataType::Float2, "a_TexCoords"},
};
}  // namespace Kazel