#include "cube.hpp"
#include "cylinder.hpp"

// POST UNIFORMS
float const postAmb[] =  { 0.f, 0.f, 0.f };
float const postDiff[] = { 0.01f, 0.01f, 0.01f };
float const postSpec[] = { 0.5f, 0.5f, 0.5f };
float const postShin( 16.f );

// LIGHT UNIFORMS
Vec3f lightPositionVector{ 0.f, 2.f, 0.f };
float const lightPos[] = { lightPositionVector.x,
                                      lightPositionVector.y,
                                      lightPositionVector.z };
float const lightAmb[] = { 0.2f, 0.2f, 0.2f };
float const lightIncoming[] = { 1.0f, 1.0f, 1.0f };


Mat44f basicLightModel = make_translation( { 0.f, 2.f, 0.f } )
                        * make_scaling( 0.2f, 0.2f, 0.2f );

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


    glUniform3fv( 3, 1, lightPos );    // light pos
    glUniform3fv( 4, 1, lightAmb );    // amb
    glUniform3fv( 5, 1, lightIncoming );   // incoming light value
    glUniform1f( 10, 0.001f ); // emissive term

    // LAMPPOST
    glUniform3fv( 6, 1, postAmb );    // amb
    glUniform3fv( 7, 1, postDiff );   // diff
    glUniform3fv( 8, 1, postSpec );   // spec
    glUniform1f( 9, postShin );      // shin

    glBindVertexArray( postVAO );
    glUniformMatrix4fv( 0, 1, GL_TRUE, postMVP.v );
    glUniformMatrix4fv( 1, 1, GL_TRUE, postModel.v );
    glDrawArrays( GL_TRIANGLES, 0, 1200 );

    // LIGHT CUBE
    glBindVertexArray( lightVAO );
    glUniformMatrix4fv( 0, 1, GL_TRUE, lightMVP.v );  
    glUniformMatrix4fv( 1, 1, GL_TRUE, lightModel.v ); 
    glUniform1f( 10, 1.f ); // emmissive = 1 for light
    glDrawArrays( GL_TRIANGLES, 0, 6 * 2 * 3 );


}

