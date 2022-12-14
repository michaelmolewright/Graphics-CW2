#ifndef SIMPLE_MESH_HPP_C6B749D6_C83B_434C_9E58_F05FC27FEFC9
#define SIMPLE_MESH_HPP_C6B749D6_C83B_434C_9E58_F05FC27FEFC9

#include <glad.h>

#include <vector>

#include "../vmlib/vec3.hpp"

int ccw( Vec3f a, Vec3f b, Vec3f c );

void reverse_cw_triangles( std::vector<Vec3f> &pos );

struct SimpleMeshData {
    std::vector<Vec3f> positions;
    std::vector<Vec3f> colors;
};

SimpleMeshData concatenate( SimpleMeshData, SimpleMeshData const & );

GLuint create_vao( SimpleMeshData const & );

#endif   // SIMPLE_MESH_HPP_C6B749D6_C83B_434C_9E58_F05FC27FEFC9
