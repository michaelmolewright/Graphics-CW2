#ifndef CUBE_HPP_6874B39C_112D_4D34_BD85_AB81A730955B
#define CUBE_HPP_6874B39C_112D_4D34_BD85_AB81A730955B

#include "simple_mesh.hpp"

// This defines the vertex data for a colored unit cube.

// vertices and normals
constexpr float const cubePositions[] = {
    0.5f, 0.5f, -0.5f,     
    0.5f, -0.5f, -0.5f,  
    -0.5f, -0.5f, -0.5f,   

    -0.5f, -0.5f, -0.5f,  
    -0.5f, 0.5f, -0.5f,    
    0.5f, 0.5f, -0.5f,     
     // Front face
    0.5f, -0.5f, 0.5f,   
    0.5f, 0.5f, 0.5f,     
    -0.5f, -0.5f, 0.5f,    

    -0.5f, 0.5f, 0.5f,  
    -0.5f, -0.5f, 0.5f,  
    0.5f, 0.5f, 0.5f,   
    // Left face
    -0.5f, 0.5f, -0.5f,   
    -0.5f, -0.5f, -0.5f,   
    -0.5f, 0.5f, 0.5f,    

    -0.5f, -0.5f, 0.5f,   
    -0.5f, 0.5f, 0.5f,   
    -0.5f, -0.5f, -0.5f, 
                           // Right face
    0.5f, -0.5f, -0.5f,   
    0.5f, 0.5f, -0.5f,     
    0.5f, 0.5f, 0.5f,     

    0.5f, 0.5f, 0.5f,    
    0.5f, -0.5f, 0.5f,     
    0.5f, -0.5f, -0.5f,    
                           // Bottom face
    0.5f, -0.5f, -0.5f,    
    0.5f, -0.5f, 0.5f,    
    -0.5f, -0.5f, -0.5f,   

    -0.5f, -0.5f, 0.5f,   
    -0.5f, -0.5f, -0.5f,   
    0.5f, -0.5f, 0.5f,    
                           // Top face
    0.5f, 0.5f, 0.5f,    
    0.5f, 0.5f, -0.5f,     
    -0.5f, 0.5f, -0.5f,   

    -0.5f, 0.5f, -0.5f,   
    -0.5f, 0.5f, 0.5f,   
    0.5f, 0.5f, 0.5f,     
};

// CUBE 1 material colour data
// http://devernay.free.fr/cours/opengl/materials.html
static float const cubeAmb[] = { 0.25f, 0.20725f, 0.20725f };
static float const cubeDiff[] = { 0.1f, 0.1f, 0.1f };
static float const cubeSpec[] = { 0.296648f, 0.296648f, 0.296648f };
static float const cubeShin = 4.f;
Mat44f cubeBaseModel = make_translation( { 0.5f, 0.5f, 0.5f } );

SimpleMeshData make_cube( Mat44f aPreTransform ) {

    std::vector<Vec3f> pos;

    // add each triangle from cube positions to pos
    for ( size_t i = 0; i < sizeof( cubePositions ); i += 3 ) {
        Vec3f tri = { cubePositions[i], cubePositions[i + 1],
                      cubePositions[i + 2] };

        pos.emplace_back( tri );
    }

    // pre transform each point
    for ( auto &p : pos ) {
        Vec4f p4{ p.x, p.y, p.z, 1.f };
        Vec4f t = aPreTransform * cubeBaseModel * p4;
        t /= t.w;

        p = Vec3f{ t.x, t.y, t.z };
    }

    return SimpleMeshData{ std::move( pos ) };
}

void draw_cube( GLuint vao, Mat44f baseMVP, Mat44f model ) {
    Mat44f cubeMVP = baseMVP * model;

    glUniformMatrix4fv( 0, 1, GL_TRUE, cubeMVP.v );
    glUniformMatrix4fv( 1, 1, GL_TRUE, model.v );   // model matrix


    glBindVertexArray( vao );
    glDrawArrays( GL_TRIANGLES, 0, 6 * 2 * 3 );
}

#endif   // CUBE_HPP_6874B39C_112D_4D34_BD85_AB81A730955B
