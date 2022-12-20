#ifndef tile
#define tile

#include <vector>

#include <cstdlib>
#include <cstdio>
#include "../vmlib/vec3.hpp"
#include "../vmlib/mat44.hpp"
#include "simple_mesh.hpp"

class plane{
    public:
        SimpleMeshData planeMesh;
        SimpleMeshData boxMesh;
        GLuint textureID;
        GLuint vao;


        void createTile(GLuint ID);
        void createBox(GLuint ID);
        void drawBox(Mat44f baseMVP, Mat44f translation);

};

void plane::createTile(GLuint ID ){
    textureID = ID;
    std::vector<Vec3f> positions;
    std::vector<Vec3f> normals;
    std::vector<Vec2f> textureCoord;

    positions.emplace_back(Vec3f{0.f, 1.f, 0.f});
    positions.emplace_back(Vec3f{0.f, 0.f, 0.f});
    positions.emplace_back(Vec3f{1.f, 0.f, 0.f});

    positions.emplace_back(Vec3f{0.f, 1.f, 0.f});
    positions.emplace_back(Vec3f{1.f, 0.f, 0.f});
    positions.emplace_back(Vec3f{1.f, 1.f, 0.f});

    normals.emplace_back(Vec3f{0.f, 0.f, 1.f});
    normals.emplace_back(Vec3f{0.f, 0.f, 1.f});
    normals.emplace_back(Vec3f{0.f, 0.f, 1.f});
    normals.emplace_back(Vec3f{0.f, 0.f, 1.f});
    normals.emplace_back(Vec3f{0.f, 0.f, 1.f});
    normals.emplace_back(Vec3f{0.f, 0.f, 1.f});

    textureCoord.emplace_back(Vec2f{0.f, 1.f});
    textureCoord.emplace_back(Vec2f{0.f, 0.f});
    textureCoord.emplace_back(Vec2f{1.f, 0.f});
    textureCoord.emplace_back(Vec2f{0.f, 1.f});
    textureCoord.emplace_back(Vec2f{1.f, 0.f});
    textureCoord.emplace_back(Vec2f{1.f, 1.f});

    planeMesh = SimpleMeshData{ std::move( positions ), std::move( normals ), std::move( textureCoord )};
}

void plane::createBox(GLuint ID){
    createTile(ID);
    SimpleMeshData plane1 = planeMesh;
    SimpleMeshData plane2 = transformPositions(plane1, make_translation({1.f,0.f,0.f}) * make_rotation_y(PI/2.f));

    SimpleMeshData plane3 = transformPositions(plane1, make_translation({0.f,0.f,-1.f}) * make_rotation_y(-PI/2.f));
    SimpleMeshData plane4 = transformPositions(plane1, make_translation({1.f,0.f,-1.f}) * make_rotation_y(PI));

    SimpleMeshData plane5 = transformPositions(plane1, make_translation({0.f,1.f,0.f}) * make_rotation_x(-PI/2.f));
    SimpleMeshData plane6 = transformPositions(plane1, make_translation({0.f,0.f,-1.f}) * make_rotation_x(PI/2.f));


    SimpleMeshData res1 = concatenate(plane1, plane2);
    SimpleMeshData res2 = concatenate(plane3, plane4);
    SimpleMeshData res3 = concatenate(plane5, plane6);

    boxMesh = concatenate(concatenate(res1, res2), res3);
    //boxMesh = concatenate(planeMesh, boxMesh);

}

void plane::drawBox( Mat44f baseMVP, Mat44f translation){
    vao = create_full_vao(boxMesh);

    glBindTexture(GL_TEXTURE_2D, textureID);

    Mat44f MVP = baseMVP * translation;

    glUniformMatrix4fv( 0, 1, GL_TRUE, MVP.v );
    glUniformMatrix4fv( 1, 1, GL_TRUE, translation.v );
    glBindVertexArray( vao );

    glDrawArrays( GL_TRIANGLES, 0, boxMesh.positions.size() );
}

float const concreteCol[] ={ 0.52f, 0.51f, 0.6f };
float const concreteShine(10.f);



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

    
    glUniform3fv( 5, 1, concreteCol );    
    glUniform1f( 7, concreteShine );

    Mat44f MVP = baseMVP * translation;

    glUniformMatrix4fv( 0, 1, GL_TRUE, MVP.v );
    glUniformMatrix4fv( 1, 1, GL_TRUE, translation.v );
    glBindVertexArray( vao );

    glDrawArrays( GL_TRIANGLES, 0, 6 );

}

#endif
