#include "kzpch.h"
#include "Model.h"
#include <assimp/Importer.hpp>

namespace Kazel {
Model::Model(const std::string& path) { loadModel(path); }

void Model::Draw(const Ref<Shader>& shader) {
  for (auto& mesh : m_Meshes) {
    mesh.Draw(shader);
  }
}

void Model::DrawInstanced(const Ref<Shader>& shader, uint32_t count) {
  for (auto& mesh : m_Meshes) {
    mesh.DrawInstanced(shader, count);
  }
}

void Model::AddBuffer(const Ref<VertexBuffer>& buffer) {
  for (auto& mesh : m_Meshes) {
    auto vao = mesh.GetVAO();
    vao->AddVertexBuffer(buffer);
  }
}

void Model::loadModel(const std::string& path) {
  Assimp::Importer import;
  /**
   * aiProcess_Triangulate:将模型所有的图元形状变换为三角形(if not Triangule)
   * aiProcess_FlipUVs:翻转y轴的纹理坐标
   * aiProcess_GenNormals:如果模型不包含法向量的话，就为每个顶点创建法线
   * aiProcess_SplitLargeMeshes:比较大的网格分割成更小的子网格(减少每个网格的顶点个数)
   * aiProcess_OptimizeMeshes：将多个小网格拼接为一个大的网格，减少绘制调用
   */
  const aiScene* scene =
      import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    KZ_CORE_ERROR("ERROR::ASSIMP::{}", import.GetErrorString());
    return;
  }
  m_Directory = path.substr(0, path.find_last_of('\\'));

  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
  /**
   * 处理节点所有的网格
   * Scene下的mMeshes数组储存了真正的Mesh对象，节点中的mMeshes数组保存的只是场景中网格数组的索引。
   */
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    processMesh(mesh, scene);
  }

  /**
   * 处理子节点
   */
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

void Model::processMesh(aiMesh* mesh, const aiScene* scene) {
  std::vector<SMeshVertex> vertices;
  std::vector<unsigned int> indices;
  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    glm::vec3 position{mesh->mVertices[i].x, mesh->mVertices[i].y,
                       mesh->mVertices[i].z};
    glm::vec3 normal{mesh->mNormals[i].x, mesh->mNormals[i].y,
                     mesh->mNormals[i].z};

    glm::vec2 texCoord;
    if (mesh->mTextureCoords[0])  // 网格是否有纹理坐标？
    {
      texCoord.x = mesh->mTextureCoords[0][i].x;
      texCoord.y = mesh->mTextureCoords[0][i].y;
    } else
      texCoord = glm::vec2(0.0f, 0.0f);

    vertices.emplace_back(position, normal, texCoord);
  }

  //每个网格都有一个面（图元），一个面包含多个索引
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    //该面（图元）的索引个数
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  m_Meshes.emplace_back(vertices, indices);
  auto& cMesh = m_Meshes.back();

  //一个网格只包含了一个指向材质对象的索引
  if (mesh->mMaterialIndex >= 0) {
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    cMesh.AddTextures(CMesh::Diffuse,
                      LoadTextures(material, aiTextureType_DIFFUSE));
    cMesh.AddTextures(CMesh::Specular,
                      LoadTextures(material, aiTextureType_SPECULAR));
  }
}

std::vector<Ref<Texture2D>> Model::LoadTextures(aiMaterial* mat,
                                                aiTextureType type) {
  std::vector<Ref<Texture2D>> textures;
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);

    std::string path = std::string(str.C_Str(), str.length);
    auto it = m_LoadTextures.find(path);
    if (it == m_LoadTextures.end()) {
      // load this
      std::string filename = m_Directory + "/" + str.C_Str();
      auto texture = Kazel::Texture2D::Create(filename);
      m_LoadTextures.emplace(path, texture);
    }

    textures.push_back(m_LoadTextures.at(path));
  }
  return textures;
}
}  // namespace Kazel