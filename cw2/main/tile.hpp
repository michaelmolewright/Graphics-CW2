#include <vector>

#include <cstdlib>
#include <cstdio>
#include "../vmlib/vec3.hpp"
#include "../vmlib/mat44.hpp"

constexpr float const tileValues[] = {
    //positions         //normals           //texture co-ordinates
    0.f, 1.f, 0.f,      0.f, 0.f, 1.f,      0.f, 1.f, 
    0.f, 0.f, 0.f,      0.f, 0.f, 1.f,      0.f, 0.f,
    1.f, 0.f, 0.f,      0.f, 0.f, 1.f,      1.f, 0.f,

    0.f, 1.f, 0.f,      0.f, 0.f, 1.f,      0.f, 1.f,              
    1.f, 0.f, 0.f,      0.f, 0.f, 1.f,      1.f, 0.f,
    1.f, 1.f, 0.f,      0.f, 0.f, 1.f,      1.f, 1.f
};


GLuint createTextureTileVao(){
    GLuint valuesVBO = 0;
    glGenBuffers( 1, &valuesVBO );
    glBindBuffer( GL_ARRAY_BUFFER, valuesVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( tileValues ), tileValues, GL_STATIC_DRAW );

    GLuint tileVao = 0;
    glGenVertexArrays( 1, &tileVao );
    glBindVertexArray( tileVao );

    glBindBuffer( GL_ARRAY_BUFFER, valuesVBO );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), 0 );
    glEnableVertexAttribArray( 0 );

    glBindBuffer( GL_ARRAY_BUFFER, valuesVBO );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void*)(3 * sizeof(float)) );
    glEnableVertexAttribArray( 1 );

	glBindBuffer( GL_ARRAY_BUFFER, valuesVBO );
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, (8 * sizeof(float)), (void*)(6 * sizeof(float)) );
    glEnableVertexAttribArray( 2 );

    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glDeleteBuffers( 1, &valuesVBO );

    return tileVao;
}   

void drawTile(GLuint textureID, Mat44f baseMVP, Mat44f translation, GLuint vao){


    glBindTexture(GL_TEXTURE_2D, textureID);

    Mat44f MVP = baseMVP * translation;

    glUniformMatrix4fv( 0, 1, GL_TRUE, MVP.v );
    glUniformMatrix4fv( 1, 1, GL_TRUE, translation.v );
    glBindVertexArray( vao );

    glDrawArrays( GL_TRIANGLES, 0, 6 );

}
