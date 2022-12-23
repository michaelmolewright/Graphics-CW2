#ifndef newLamp
#define newLamp

#include "cube.hpp"
#include "cylinder.hpp"
#include "bowl.hpp"
#include "material.hpp"

class lamp {
  public:
    GLuint postVAO;
    GLuint sphereVAO;
    float height;
    float sphereScalar = 0.3f;
    Vec3f lightPosition = { 0.f, 0.f, 0.f };
    Vec3f lightAmbient = { 0.5f, 0.5f, 0.5f };
    Vec3f lightColor = { 0.2f, 0.2f, 0.2f };
    SimpleMeshData post;
    SimpleMeshData sphereLight;

    void createLamp( float height, Vec3f ambientinput, Vec3f colorInput );
    void drawLamp( Mat44f MVP, Mat44f transform, int ID, std::string a );
};

void lamp::createLamp( float height, Vec3f ambientinput, Vec3f colorInput ) {
    lightPosition.y = height;
    lightAmbient = ambientinput;
    lightColor = colorInput;

    post = make_cylinder( true, 100, { 1.f, 0.f, 0.f },
                          make_scaling( 0.2f, height, 0.2f ) *
                              make_rotation_z( PI / 2.f ) );
    postVAO = create_vao( post );
    sphereLight = createSphere(
        make_translation( { 0.f, height - ( 2.f * sphereScalar ), 0.f } ) *
        make_scaling( sphereScalar, sphereScalar, sphereScalar ) *
        make_translation( { 0.f, 2.f, 0.f } ) );
    sphereVAO = reversedNormalsVAO( sphereLight );
}

void lamp::drawLamp( Mat44f MVP, Mat44f transform, int ID, std::string a ) {
    Mat44f newMVP = MVP * transform;

    // update Light Position with transform
    Vec4f updateLightPos = transform * Vec4f{ lightPosition.x, lightPosition.y,
                                              lightPosition.z, 1.f };
    updateLightPos /= updateLightPos.w;

    // create float arrays from vec3s
    float new_lightPosition[] = { updateLightPos.x, updateLightPos.y,
                                  updateLightPos.z };
    float new_lightAmbient[] = { lightAmbient.x, lightAmbient.y,
                                 lightAmbient.z };
    float new_lightColor[] = { lightColor.x, lightColor.y, lightColor.z };

    GLuint loc;
    std::string input;

    input = a + "position";
    loc = glGetUniformLocation( ID, input.c_str() );
    glUniform3fv( loc, 1, new_lightPosition );

    input = a + "ambient";
    loc = glGetUniformLocation( ID, input.c_str() );
    glUniform3fv( loc, 1, new_lightAmbient );

    input = a + "color";
    loc = glGetUniformLocation( ID, input.c_str() );
    glUniform3fv( loc, 1, new_lightColor );

    // draw sphere light
    setMaterialProperties( "lampMaterial" );
    glBindVertexArray( sphereVAO );
    glUniformMatrix4fv( 0, 1, GL_TRUE, newMVP.v );
    glUniformMatrix4fv( 1, 1, GL_TRUE, transform.v );
    glDrawArrays( GL_TRIANGLES, 0, sphereLight.positions.size() );

    // draw Post
    setMaterialProperties( "railMetal" );
    glBindVertexArray( postVAO );
    glUniformMatrix4fv( 0, 1, GL_TRUE, newMVP.v );
    glUniformMatrix4fv( 1, 1, GL_TRUE, transform.v );
    glDrawArrays( GL_TRIANGLES, 0, post.positions.size() );
}

#endif