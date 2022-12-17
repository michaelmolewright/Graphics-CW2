#include "cube.hpp"
#include "cylinder.hpp"
#include "bowl.hpp"



float const mypi = 3.1415926f;
// POST UNIFORMS
float const postAmb[] =  { 0.f, 0.f, 0.f };
float const postDiff[] = { 0.01f, 0.01f, 0.01f };
float const postSpec[] = { 0.5f, 0.5f, 0.5f };
float const postShin( 16.f );

// LIGHT UNIFORMS
Vec3f lightPositionVector{ 0.f, 2.f, 0.f };
Vec4f lightPosVec4f{ lightPositionVector.x, lightPositionVector.y, lightPositionVector.z, 1.f };


float const lightAmb[] = { 0.2f, 0.2f, 0.2f };
float const lightIncoming[] = { 1.0f, 1.f, 1.0f };


Mat44f basicLightModel = make_translation( lightPositionVector )
                        * make_scaling( 0.2f, 0.2f, 0.2f );

SimpleMeshData createLamp(float height){
    SimpleMeshData post = make_cylinder( true, 100, { 1.f, 0.f, 0.f }, make_scaling(0.25f, height, 0.25f) * make_rotation_z(mypi/2.f) );
    SimpleMeshData sphereLight = createSphere( make_translation({0.f,height,0.f}) * make_scaling(0.4f, 0.4f, 0.4f) * make_translation({0.f,2.f,0.f} ));
    SimpleMeshData finalLamp = concatenate(post, sphereLight);
    return finalLamp;
}


GLuint create_light_vao() {
    // LIGHT CUBE
    GLuint cubeVBO = 0;
    glGenBuffers( 1, &cubeVBO );
    glBindBuffer( GL_ARRAY_BUFFER, cubeVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( cubePositions ), cubePositions,
                  GL_STATIC_DRAW );

    GLuint lightVAO = 0;
    glGenVertexArrays( 1, &lightVAO );
    glBindVertexArray( lightVAO );
    glBindBuffer( GL_ARRAY_BUFFER, cubeVBO );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ),
                           (void *)0 );
    glEnableVertexAttribArray( 0 );

    // reset and delete buffers
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glDeleteBuffers( 1, &cubeVBO );

    return lightVAO;
}

// DRAWS LAMP WHEREEVER aPreTransform IS BUT A NEW LIGHT POSITION ALSO NEEDS SPECIFIED
void draw_lamp( GLuint lightVAO, GLuint postVAO, Mat44f MVP, Mat44f aPreTransform ) {
    
    Mat44f lightModel = aPreTransform * basicLightModel;
    Mat44f lightMVP = MVP * lightModel;

    Mat44f postModel = aPreTransform;
    Mat44f postMVP = MVP * postModel;

    Vec4f newLightPos = aPreTransform * lightPosVec4f;
    float const lightPos[] = { newLightPos.x,
                                      newLightPos.y,
                                      newLightPos.z };
    // float const lightPos[] = { lightPositionVector.x, lightPositionVector.y, lightPositionVector.z };

    float const lightColor [] = { 1.f, 1.f, 1.f };
    float const bulbColor [] = { 1.f, 1.f, 1.f };
    glUniform3fv( 3, 1, lightPos );    // light pos
    glUniform3fv( 4, 1, lightColor );    // light color
    glUniform3fv( 5, 1, bulbColor );    // object color

    glUniform1f( 6, 1.f ); // set emissive term

    // glUniform3fv( 4, 1, lightAmb );    // amb
    // glUniform3fv( 5, 1, lightIncoming );   // incoming light value

    // LIGHT CUBE
    glBindVertexArray( lightVAO );
    glUniformMatrix4fv( 0, 1, GL_TRUE, lightMVP.v );  //lightMVP
    glUniformMatrix4fv( 1, 1, GL_TRUE, lightModel.v ); //lightModel
    // glUniform1f( 10, 1.f ); // emmissive = 1 for light
    glDrawArrays( GL_TRIANGLES, 0, 6 * 2 * 3 );

    glUniform1f( 6, .0001f ); // set emissive term

    // // LAMPPOST
    // glUniform3fv( 6, 1, postAmb );    // amb
    // glUniform3fv( 7, 1, postDiff );   // diff
    // glUniform3fv( 8, 1, postSpec );   // spec
    // glUniform1f( 9, postShin );      // shin
    // glUniform1f( 10, 0.001f ); // set emissive term


    float const railColor [] = { 0.2f, 0.2f, 0.2f };
    glUniform3fv( 5, 1, railColor );    // object color


    glBindVertexArray( postVAO );
    glUniformMatrix4fv( 0, 1, GL_TRUE, postMVP.v );
    glUniformMatrix4fv( 1, 1, GL_TRUE, postModel.v );
    glDrawArrays( GL_TRIANGLES, 0, 1200 );

}

void draw_lamp_new( int vertexCount ,GLuint postVAO, Mat44f MVP, Mat44f aPreTransform ){
    
    
    float lightPosition[] = { 0.f, 10.8f, 0.f };
    float lightAmbient[] = { 1.f, 1.f, 1.f};
    float lightDiffuse[] = { 0.2f, 0.2f, 0.2f};
    float lightSpecular[] = { 1.f, 1.f, 1.f};

    glUniform3fv(11,1, lightPosition);
        
    glUniform3fv(12,1, lightAmbient);
    
    glUniform3fv( 13, 1, lightDiffuse );
    
    glUniform3fv(14,1, lightSpecular);

    Mat44f newMVP = MVP * aPreTransform;
    glBindVertexArray( postVAO );
    glUniformMatrix4fv( 0, 1, GL_TRUE, newMVP.v );
    glUniformMatrix4fv( 1, 1, GL_TRUE, aPreTransform.v );
    glDrawArrays( GL_TRIANGLES, 0, vertexCount );
}



