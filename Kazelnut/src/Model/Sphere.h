#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>

class Sphere {
  // memeber vars
  float radius;
  int sectorCount;                        // longitude, # of slices
  int stackCount;                         // latitude, # of stacks
  bool smooth;
  std::vector<float> vertices;
  std::vector<float> normals;
  std::vector<float> texCoords;
  std::vector<unsigned int> indices;
  std::vector<unsigned int> lineIndices;
 public:
   Sphere(float radius, int sectorCount, int stackCount, bool smooth);

 private:
  void buildVerticesSmooth();
  void clearArrays();

  void buildVerticesFlat();
  void addVertex(float x, float y, float z);
  void addNormal(float nx, float ny, float nz);
  void addTexCoord(float s, float t);
};