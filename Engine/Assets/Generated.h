#include <filesystem>
#include "Vertex.h"
#include "glm/glm.hpp"
#include <GL/glew.h>


const std::string GeneratedMeshExtension = ".gen";

std::filesystem::path GeneratePathForCircle(float Radius, int Segments);

std::pair<std::vector<Vertex>, std::vector<unsigned int>> GenerateVerticesAndIndices(std::filesystem::path path, glm::vec2* AABBmin, glm::vec2* AABBmax, GLenum* DrawingMode);