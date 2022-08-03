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
   * aiProcess_Triangulate:��ģ�����е�ͼԪ��״�任Ϊ������(if not Triangule)
   * aiProcess_FlipUVs:��תy�����������
   * aiProcess_GenNormals:���ģ�Ͳ������������Ļ�����Ϊÿ�����㴴������
   * aiProcess_SplitLargeMeshes:�Ƚϴ������ָ�ɸ�С��������(����ÿ������Ķ������)
   * aiProcess_OptimizeMeshes�������С����ƴ��Ϊһ��������񣬼��ٻ��Ƶ���
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
   * ����ڵ����е�����
   * Scene�µ�mMeshes���鴢����������Mesh���󣬽ڵ��е�mMeshes���鱣���ֻ�ǳ��������������������
   */
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    processMesh(mesh, scene);
  }

  /**
   * �����ӽڵ�
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
    if (mesh->mTextureCoords[0])  // �����Ƿ����������ꣿ
    {
      texCoord.x = mesh->mTextureCoords[0][i].x;
      texCoord.y = mesh->mTextureCoords[0][i].y;
    } else
      texCoord = glm::vec2(0.0f, 0.0f);

    vertices.emplace_back(position, normal, texCoord);
  }

  //ÿ��������һ���棨ͼԪ����һ��������������
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    //���棨ͼԪ������������
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  m_Meshes.emplace_back(vertices, indices);
  auto& cMesh = m_Meshes.back();

  //һ������ֻ������һ��ָ����ʶ��������
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