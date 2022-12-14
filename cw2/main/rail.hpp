#include "cylinder.hpp"

// POST UNIFORMS
float const postAmb[] =  { 0.f, 0.f, 0.f };
float const postDiff[] = { 0.01f, 0.01f, 0.01f };
float const postSpec[] = { 0.5f, 0.5f, 0.5f };
float const postShin( 16.f );

constexpr float kPi_ = 3.1415926f;

SimpleMeshData make_half_pipe( std::size_t aSubdivs, Vec3f aColor,
                              Mat44f aPreTransform ) {

    std::vector<Vec3f> pos;

    auto mainRail = make_cylinder( true, 100, { 1.f, 0.f, 0.f },
                                // kIdentity44f
                                make_rotation_z( kPi_/2 ) *
                                make_scaling( 2.f, 0.05f,  0.05f )
                                 );

    std::vector col( pos.size(), aColor );

    // pre transform each point
    for ( auto &p : pos ) {
        Vec4f p4{ p.x, p.y, p.z, 1.f };
        Vec4f t = aPreTransform * p4;
        t /= t.w;

        p = Vec3f{ t.x, t.y, t.z };
    }

    
    return SimpleMeshData{ std::move( pos ), std::move( col ) };
}