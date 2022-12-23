#ifndef tile
#define tile

#include <vector>

#include <cstdlib>
#include <cstdio>
#include "../vmlib/vec3.hpp"
#include "../vmlib/mat44.hpp"
#include "simple_mesh.hpp"

class plane {
  public:
    SimpleMeshData planeMesh;
    SimpleMeshData boxMesh;
    SimpleMeshData rampMesh;
    SimpleMeshData complexRampMesh;
    SimpleMeshData triangleMesh;
    SimpleMeshData rampCornerMesh;

    GLuint textureID;
    GLuint vao;

    void createTile();
    void createBox();
    void createRamp();
    void createComplexRamp();
    void createTriangle();
    void createRampCorner();
    void drawBox( GLuint ID, Mat44f baseMVP, Mat44f translation );
    void drawTile( GLuint ID, Mat44f baseMVP, Mat44f translation );
    void drawRamp( GLuint ID, Mat44f baseMVP, Mat44f translation );
    void drawComplexRamp( GLuint ID, Mat44f baseMVP, Mat44f translation );
};

void plane::createTile() {
    std::vector<Vec3f> positions;
    std::vector<Vec3f> normals;
    std::vector<Vec2f> textureCoord;

    positions.emplace_back( Vec3f{ 0.f, 1.f, 0.f } );
    positions.emplace_back( Vec3f{ 0.f, 0.f, 0.f } );
    positions.emplace_back( Vec3f{ 1.f, 0.f, 0.f } );

    positions.emplace_back( Vec3f{ 0.f, 1.f, 0.f } );
    positions.emplace_back( Vec3f{ 1.f, 0.f, 0.f } );
    positions.emplace_back( Vec3f{ 1.f, 1.f, 0.f } );

    normals.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );
    normals.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );
    normals.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );
    normals.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );
    normals.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );
    normals.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );

    textureCoord.emplace_back( Vec2f{ 0.f, 1.f } );
    textureCoord.emplace_back( Vec2f{ 0.f, 0.f } );
    textureCoord.emplace_back( Vec2f{ 1.f, 0.f } );
    textureCoord.emplace_back( Vec2f{ 0.f, 1.f } );
    textureCoord.emplace_back( Vec2f{ 1.f, 0.f } );
    textureCoord.emplace_back( Vec2f{ 1.f, 1.f } );

    planeMesh = SimpleMeshData{ std::move( positions ), std::move( normals ),
                                std::move( textureCoord ) };
}

void plane::createTriangle() {
    std::vector<Vec3f> positions;
    std::vector<Vec3f> normals;
    std::vector<Vec2f> textureCoord;

    positions.emplace_back( Vec3f{ 0.f, 1.f, 0.f } );
    positions.emplace_back( Vec3f{ 0.f, 0.f, 0.f } );
    positions.emplace_back( Vec3f{ 1.f, 0.f, 0.f } );

    normals.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );
    normals.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );
    normals.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );

    textureCoord.emplace_back( Vec2f{ 0.f, 1.f } );
    textureCoord.emplace_back( Vec2f{ 0.f, 0.f } );
    textureCoord.emplace_back( Vec2f{ 1.f, 0.f } );

    triangleMesh = SimpleMeshData{ std::move( positions ), std::move( normals ),
                                   std::move( textureCoord ) };
}

void plane::createBox() {
    createTile();

    SimpleMeshData plane1 = planeMesh;
    SimpleMeshData plane2 =
        transformPositions( plane1, make_translation( { 1.f, 0.f, 0.f } ) *
                                        make_rotation_y( PI / 2.f ) );

    SimpleMeshData plane3 =
        transformPositions( plane1, make_translation( { 0.f, 0.f, -1.f } ) *
                                        make_rotation_y( -PI / 2.f ) );
    SimpleMeshData plane4 =
        transformPositions( plane1, make_translation( { 1.f, 0.f, -1.f } ) *
                                        make_rotation_y( PI ) );

    SimpleMeshData plane5 =
        transformPositions( plane1, make_translation( { 0.f, 1.f, 0.f } ) *
                                        make_rotation_x( -PI / 2.f ) );
    SimpleMeshData plane6 =
        transformPositions( plane1, make_translation( { 0.f, 0.f, -1.f } ) *
                                        make_rotation_x( PI / 2.f ) );

    SimpleMeshData res1 = concatenate( plane1, plane2 );
    SimpleMeshData res2 = concatenate( plane3, plane4 );
    SimpleMeshData res3 = concatenate( plane5, plane6 );

    boxMesh = concatenate( concatenate( res1, res2 ), res3 );
}

void plane::createRamp() {
    createTile();
    createTriangle();

    SimpleMeshData plane1 = transformPositions(
        planeMesh,
        make_rotation_x( -PI / 4.f ) * make_scaling( 1.f, 1.414213562f, 1.f ) );
    SimpleMeshData plane2 =
        transformPositions( planeMesh, make_translation( { 0.f, 1.f, -1.f } ) *
                                           make_rotation_x( -PI ) );
    SimpleMeshData plane3 =
        transformPositions( planeMesh, make_translation( { 0.f, 0.f, -1.f } ) *
                                           make_rotation_x( PI / 2.f ) );

    rampMesh = concatenate( concatenate( plane1, plane2 ), plane3 );

    SimpleMeshData tri1 = transformPositions(
        triangleMesh,
        make_translation( { 0.f, 0.f, -1.f } ) * make_rotation_y( -PI / 2.f ) );

    SimpleMeshData tri2 = transformPositions(
        triangleMesh, make_translation( { 1.f, 0.f, -1.f } ) *
                          make_rotation_x( PI / 2.f ) *
                          make_rotation_y( PI / 2.f ) );

    rampMesh = concatenate( concatenate( rampMesh, tri1 ), tri2 );
}

void plane::createRampCorner() {
    std::vector<Vec3f> positions;
    std::vector<Vec3f> normals;
    std::vector<Vec2f> textureCoord;

    positions.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );
    positions.emplace_back( Vec3f{ 1.f, 0.f, 0.f } );
    positions.emplace_back( Vec3f{ 0.f, 1.f, 0.f } );
    positions.emplace_back( Vec3f{ 0.f, 1.f, 0.f } );
    positions.emplace_back( Vec3f{ 0.f, 0.f, 0.f } );
    positions.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );
    positions.emplace_back( Vec3f{ 0.f, 1.f, 0.f } );
    positions.emplace_back( Vec3f{ 1.f, 0.f, 0.f } );
    positions.emplace_back( Vec3f{ 0.f, 0.f, 0.f } );
    positions.emplace_back( Vec3f{ 0.f, 0.f, 0.f } );
    positions.emplace_back( Vec3f{ 1.f, 0.f, 0.f } );
    positions.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );

    normals.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );
    normals.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );
    normals.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );
    normals.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );
    normals.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );
    normals.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );
    normals.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );
    normals.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );
    normals.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );
    normals.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );
    normals.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );
    normals.emplace_back( Vec3f{ 0.f, 0.f, 1.f } );

    textureCoord.emplace_back( Vec2f{ 0.f, 0.f } );
    textureCoord.emplace_back( Vec2f{ 1.f, 0.f } );
    textureCoord.emplace_back( Vec2f{ 0.5f, 1.f } );
    textureCoord.emplace_back( Vec2f{ 0.f, 0.f } );
    textureCoord.emplace_back( Vec2f{ 1.f, 0.f } );
    textureCoord.emplace_back( Vec2f{ 0.5f, 1.f } );
    textureCoord.emplace_back( Vec2f{ 0.f, 0.f } );
    textureCoord.emplace_back( Vec2f{ 1.f, 0.f } );
    textureCoord.emplace_back( Vec2f{ 0.5f, 1.f } );
    textureCoord.emplace_back( Vec2f{ 0.f, 0.f } );
    textureCoord.emplace_back( Vec2f{ 1.f, 0.f } );
    textureCoord.emplace_back( Vec2f{ 0.5f, 1.f } );

    rampCornerMesh =
        SimpleMeshData{ std::move( positions ), std::move( normals ),
                        std::move( textureCoord ) };
}

void plane::createComplexRamp() {
    createRampCorner();
    SimpleMeshData ramp1 = transformPositions( rampMesh, kIdentity44f );
    SimpleMeshData ramp2 =
        transformPositions( rampMesh, make_translation( { -1.f, 0.f, -2.f } ) *
                                          make_rotation_y( -PI / 2.f ) );
    SimpleMeshData ramp3 =
        transformPositions( rampMesh, make_translation( { 2.f, 0.f, -1.f } ) *
                                          make_rotation_y( PI / 2.f ) );
    SimpleMeshData ramp4 =
        transformPositions( rampMesh, make_translation( { 1.f, 0.f, -3.f } ) *
                                          make_rotation_y( -PI ) );

    SimpleMeshData rampRes1 = concatenate( ramp1, ramp2 );
    SimpleMeshData rampRes2 = concatenate( ramp3, ramp4 );

    SimpleMeshData rampResult = concatenate( rampRes1, rampRes2 );

    SimpleMeshData corner1 = transformPositions(
        rampCornerMesh,
        make_translation( { 0.f, 0.f, -1.f } ) * make_rotation_y( -PI / 2.f ) );
    SimpleMeshData corner2 = transformPositions(
        rampCornerMesh, make_translation( { 1.f, 0.f, -1.f } ) );
    SimpleMeshData corner3 = transformPositions(
        rampCornerMesh,
        make_translation( { 0.f, 0.f, -2.f } ) * make_rotation_y( PI ) );
    SimpleMeshData corner4 = transformPositions(
        rampCornerMesh,
        make_translation( { 1.f, 0.f, -2.f } ) * make_rotation_y( PI / 2.f ) );

    SimpleMeshData cornerRes1 = concatenate( corner1, corner2 );
    SimpleMeshData cornerRes2 = concatenate( corner3, corner4 );

    SimpleMeshData cornerResult = concatenate( cornerRes1, cornerRes2 );

    complexRampMesh = concatenate( rampResult, cornerResult );
    complexRampMesh = transformPositions(
        complexRampMesh, make_translation( { 0.f, 0.f, 1.f } ) );
}

void plane::drawBox( GLuint ID, Mat44f baseMVP, Mat44f translation ) {
    textureID = ID;
    vao = create_full_vao( boxMesh );

    glBindTexture( GL_TEXTURE_2D, textureID );

    Mat44f MVP = baseMVP * translation;

    glUniformMatrix4fv( 0, 1, GL_TRUE, MVP.v );
    glUniformMatrix4fv( 1, 1, GL_TRUE, translation.v );
    glBindVertexArray( vao );

    glDrawArrays( GL_TRIANGLES, 0, boxMesh.positions.size() );
}

void plane::drawTile( GLuint ID, Mat44f baseMVP, Mat44f translation ) {
    textureID = ID;
    vao = create_full_vao( planeMesh );

    glBindTexture( GL_TEXTURE_2D, textureID );

    Mat44f MVP = baseMVP * translation;

    glUniformMatrix4fv( 0, 1, GL_TRUE, MVP.v );
    glUniformMatrix4fv( 1, 1, GL_TRUE, translation.v );
    glBindVertexArray( vao );

    glDrawArrays( GL_TRIANGLES, 0, planeMesh.positions.size() );
}

void plane::drawRamp( GLuint ID, Mat44f baseMVP, Mat44f translation ) {
    textureID = ID;
    vao = create_full_vao( rampMesh );

    glBindTexture( GL_TEXTURE_2D, textureID );

    Mat44f MVP = baseMVP * translation;

    glUniformMatrix4fv( 0, 1, GL_TRUE, MVP.v );
    glUniformMatrix4fv( 1, 1, GL_TRUE, translation.v );
    glBindVertexArray( vao );

    glDrawArrays( GL_TRIANGLES, 0, rampMesh.positions.size() );
}

void plane::drawComplexRamp( GLuint ID, Mat44f baseMVP, Mat44f translation ) {
    textureID = ID;
    vao = create_full_vao( complexRampMesh );

    glBindTexture( GL_TEXTURE_2D, textureID );

    Mat44f MVP = baseMVP * translation;

    glUniformMatrix4fv( 0, 1, GL_TRUE, MVP.v );
    glUniformMatrix4fv( 1, 1, GL_TRUE, translation.v );
    glBindVertexArray( vao );

    glDrawArrays( GL_TRIANGLES, 0, complexRampMesh.positions.size() );
}

#endif
