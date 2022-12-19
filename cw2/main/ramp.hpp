#include "simple_mesh.hpp"

// vertices - normals calculated from simplemeshdata create_vao
constexpr float const rampPositions[] = {
    // side 1
    0.f, 1.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 0.f, 1.f,
    // side 2
    1.f, 0.f, 1.f,
    1.f, 0.f, 0.f,
    1.f, 1.f, 0.f,
    //bottom
    1.f, 0.f, 1.f,
    0.f, 0.f, 1.f,
    0.f, 0.f, 0.f,

    0.f, 0.f, 0.f,   
    1.f, 0.f, 0.f,
    1.f, 0.f, 1.f,
    // back 
    1.f, 0.f, 0.f,
    0.f, 0.f, 0.f,
    0.f, 1.f, 0.f,

    0.f, 1.f, 0.f,
    1.f, 1.f, 0.f,
    1.f, 0.f, 0.f,
    // ramp
    0.f, 1.f, 0.f,
    0.f, 0.f, 1.f,
    1.f, 0.f, 1.f,

    1.f, 0.f, 1.f,
    1.f, 1.f, 0.f,
    0.f, 1.f, 0.f,
};

// material data
static float const rampAmb[] = { 0.25f, 0.20725f, 0.20725f };
static float const rampDiff[] = { 0.1f, 0.1f, 0.1f };
static float const rampSpec[] = { 0.296648f, 0.296648f, 0.296648f };
static float const rampShin = 4.f;


SimpleMeshData make_ramp( Mat44f aPreTransform ) { 

    std::vector<Vec3f> pos;

    // add each triangle from cube positions to pos
    for (size_t i = 0; i < sizeof(rampPositions); i += 3 ) {
        Vec3f tri = { rampPositions[i], rampPositions[i+1], rampPositions[i+2] };

        pos.emplace_back( tri );
    }

    // pre transform each point
    for ( auto &p : pos ) {
        Vec4f p4{ p.x, p.y, p.z, 1.f };
        Vec4f t = aPreTransform * p4;
        t /= t.w;

        p = Vec3f{ t.x, t.y, t.z };
    }

    return SimpleMeshData{ std::move( pos ) };
}

SimpleMeshData make_ramp_box( Mat44f aPreTransform ) { 

    std::vector<Vec3f> pos;

    auto firstRamp = make_ramp( make_translation({ 1.f, 0.f, 0.f }));

    auto box = make_cube( kIdentity44f );
 
    auto secondRamp = make_ramp( make_translation({ -1.f, 0.f, 0.f }) * make_rotation_y( 3.1415926f ) );

    auto halfRampBox = concatenate( std::move(firstRamp), std::move(box) );
    auto rampBox = concatenate( std::move(halfRampBox), std::move(secondRamp) );


    // pre transform each point
    for ( auto &p : rampBox.positions ) {
        Vec4f p4{ p.x, p.y, p.z, 1.f };
        Vec4f t = aPreTransform * p4;
        t /= t.w;

        p = Vec3f{ t.x, t.y, t.z };
    }

    return SimpleMeshData{ std::move( rampBox.positions ) };
}




void draw_ramp( GLuint vao, Mat44f baseMVP, Mat44f model ) {
    Mat44f cubeMVP = baseMVP * model;

    glUniformMatrix4fv( 0, 1, GL_TRUE, cubeMVP.v );
    glUniformMatrix4fv( 1, 1, GL_TRUE, model.v );   // model matrix

    glUniform3fv( 5, 1, rampDiff );    
    glUniform1f( 7, rampShin );

    // material props
    // glUniform3fv( 6, 1, cubeAmb );    // amb
    // glUniform3fv( 7, 1, cubeDiff );   // diff
    // glUniform3fv( 8, 1, cubeSpec );   // spec
    // glUniform1f( 9, cubeShin );      // shin

    glBindVertexArray( vao );
    glDrawArrays( GL_TRIANGLES, 0, 24);
}

void draw_ramp_box( GLuint vao, Mat44f baseMVP, Mat44f model ) {
    Mat44f cubeMVP = baseMVP * model;

    glUniformMatrix4fv( 0, 1, GL_TRUE, cubeMVP.v );
    glUniformMatrix4fv( 1, 1, GL_TRUE, model.v );   // model matrix

    glUniform3fv( 5, 1, rampDiff );    
    glUniform1f( 7, rampShin );

    // material props
    // glUniform3fv( 6, 1, cubeAmb );    // amb
    // glUniform3fv( 7, 1, cubeDiff );   // diff
    // glUniform3fv( 8, 1, cubeSpec );   // spec
    // glUniform1f( 9, cubeShin );      // shin

    glBindVertexArray( vao );
    glDrawArrays( GL_TRIANGLES, 0, 24 + 24 + 36);
}


