#ifndef CUBE_HPP_6874B39C_112D_4D34_BD85_AB81A730955B
#define CUBE_HPP_6874B39C_112D_4D34_BD85_AB81A730955B

#include "simple_mesh.hpp"

// This defines the vertex data for a colored unit cube.

// vertices and normals
constexpr float const cubePositions[] = {
     0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f, 
     0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f, 
    // Front face
     0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,    0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  0.0f, 1.0f,
    // Left face
    -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,
    // Right face
     0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,
    // Bottom face          
     0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,
    // Top face
     0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,
};

// CUBE 1 material colour data
// http://devernay.free.fr/cours/opengl/materials.html
static float const cubeAmb[] = { 0.25f, 0.20725f, 0.20725f };
static float const cubeDiff[] = { 1.f, 0.829f, 0.829f };
static float const cubeSpec[] = { 0.296648f, 0.296648f, 0.296648f };
static float const cubeShin = 0.088f * 128;
Mat44f cube1Model = make_translation( { 2.f, 0.5f, 0.f } );

// CUBE 2
static float const cube2Amb[] = { 0.f, 0.f, 0.f };
static float const cube2Diff[] = { 0.01f, 0.01f, 0.01f };
static float const cube2Spec[] = { 0.5f, 0.5f, 0.5f };
static float const cube2Shin = 16.f;
Mat44f cube2Model = make_translation( { 4.f, 0.5f, 0.f } );



SimpleMeshData make_cube( Mat44f aPreTransform, Vec3f aColor ) { 

    std::vector<Vec3f> pos;

    // add each triangle from cube positions to pos
    for (size_t i = 0; i < sizeof(cubePositions); i += 6 ) {
        Vec3f tri = { cubePositions[i], cubePositions[i+1], cubePositions[i+2] };

        pos.emplace_back( tri );
    }

    // pre transform each point
    for ( auto &p : pos ) {
        Vec4f p4{ p.x, p.y, p.z, 1.f };
        Vec4f t = aPreTransform * p4;
        t /= t.w;

        p = Vec3f{ t.x, t.y, t.z };
    }

    std::vector col( pos.size(), aColor );

    return SimpleMeshData{ std::move( pos ), std::move( col ) };
}



GLuint create_cube_vao() {
    // CUBE - now contains positions and normals
    GLuint cubeVBO = 0;
    glGenBuffers( 1, &cubeVBO );
    glBindBuffer( GL_ARRAY_BUFFER, cubeVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( cubePositions ), cubePositions,
                  GL_STATIC_DRAW );

    GLuint cubeVAO = 0;
    glGenVertexArrays( 1, &cubeVAO );
    glBindVertexArray( cubeVAO );

    glBindBuffer( GL_ARRAY_BUFFER, cubeVBO );
    // positions
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ),
                           (void *)0 );
    glEnableVertexAttribArray( 0 );
    // normals
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ),
                           (void *)( 3 * sizeof( float ) ) );
    glEnableVertexAttribArray( 1 );

    // reset and delete buffers
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glDeleteBuffers( 1, &cubeVBO );

    return cubeVAO;
}


void draw_cube( GLuint vao, Mat44f baseMVP, Mat44f model ) {
    Mat44f cubeMVP = baseMVP * model;

    glUniformMatrix4fv( 0, 1, GL_TRUE, cubeMVP.v );
    glUniformMatrix4fv( 1, 1, GL_TRUE, model.v );   // model matrix

    // // material props
    // glUniform3fv( 6, 1, cubeAmb );    // amb
    // glUniform3fv( 7, 1, cubeDiff );   // diff
    // glUniform3fv( 8, 1, cubeSpec );   // spec
    // glUniform1f( 9, cubeShin );      // shin

    float const cubeColor [] = { 1.0f, 0.5f, 0.31f };
    glUniform3fv( 5, 1, cubeColor );    // object color

    glBindVertexArray( vao );
    glDrawArrays( GL_TRIANGLES, 0, 6 * 2 * 3 );
}

void draw_cube2( GLuint vao, Mat44f MVP ) {
    glUniformMatrix4fv( 0, 1, GL_TRUE, MVP.v );
    glUniformMatrix4fv( 1, 1, GL_TRUE, cube2Model.v );   // model matrix

    // material props
    glUniform3fv( 6, 1, cube2Amb );    // amb
    glUniform3fv( 7, 1, cube2Diff );   // diff
    glUniform3fv( 8, 1, cube2Spec );   // spec
    glUniform1f( 9, cube2Shin );      // shin

    glBindVertexArray( vao );
    glDrawArrays( GL_TRIANGLES, 0, 6 * 2 * 3 );
}

#endif   // CUBE_HPP_6874B39C_112D_4D34_BD85_AB81A730955B
