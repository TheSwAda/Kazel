#pragma once
#include <string>
#include <map>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Renderer/Shader.h"

namespace Kazel {
class Model {
 public:
  Model(const std::string& path);
  void Draw(const Ref<Shader>& shader);
  void DrawInstanced(const Ref<Shader>& shader, uint32_t count);

  void AddBuffer(const Ref<VertexBuffer>& buffer);

 private:
  void loadModel(const std::string& path);
  void processNode(aiNode* node, const aiScene* scene);
  void processMesh(aiMesh* mesh, const aiScene* scene);
  std::vector<Ref<Texture2D>> LoadTextures(aiMaterial* mat, aiTextureType type);

  std::string m_Directory;
  std::vector<CMesh> m_Meshes;
  std::map<std::string, Ref<Texture2D>> m_LoadTextures;
};
}  // namespace Kazel
