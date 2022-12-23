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

void draw_ramp( GLuint vao, Mat44f baseMVP, Mat44f model ) {
    Mat44f rampMVP = baseMVP * model;

    glUniformMatrix4fv( 0, 1, GL_TRUE, rampMVP.v );
    glUniformMatrix4fv( 1, 1, GL_TRUE, model.v );   // model matrix


    glBindVertexArray( vao );
    glDrawArrays( GL_TRIANGLES, 0, 24);
}

