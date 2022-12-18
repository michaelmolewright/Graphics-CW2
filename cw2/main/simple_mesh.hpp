#ifndef SIMPLE_MESH_HPP_C6B749D6_C83B_434C_9E58_F05FC27FEFC9
#define SIMPLE_MESH_HPP_C6B749D6_C83B_434C_9E58_F05FC27FEFC9

#include <glad.h>

#include <vector>
#include "../vmlib/vec2.hpp"
#include "../vmlib/vec3.hpp"
#include "../vmlib/vec4.hpp"
#include "../vmlib/mat44.hpp"

struct SimpleMeshData {
    std::vector<Vec3f> positions;
    std::vector<Vec3f> normals;
    std::vector<Vec2f> textureCoord;
};

SimpleMeshData concatenate( SimpleMeshData, SimpleMeshData const & );

GLuint create_vao( SimpleMeshData const & );

GLuint reversedNormalsVAO( SimpleMeshData const & );

GLuint create_full_vao( SimpleMeshData const & );

SimpleMeshData transformPositions(SimpleMeshData, Mat44f transform);



#endif   // SIMPLE_MESH_HPP_C6B749D6_C83B_434C_9E58_F05FC27FEFC9
