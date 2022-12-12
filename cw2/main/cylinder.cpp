#include "cylinder.hpp"
#include <cstdio>

SimpleMeshData make_cylinder( bool aCapped, std::size_t aSubdivs, Vec3f aColor,
                              Mat44f aPreTransform ) {

    std::vector<Vec3f> pos;

    float prevY = std::cos( 0.f );
    float prevZ = std::sin( 0.f );

    for ( std::size_t i = 0; i < aSubdivs; ++i ) {
        float const angle = ( i + 1 ) / float( aSubdivs ) * 2.f * 3.1415926f;

        float y = std::cos( angle );
        float z = std::sin( angle );

        pos.emplace_back( Vec3f{ 0.f, prevY, prevZ } );
        pos.emplace_back( Vec3f{ 0.f, y, z } );
        pos.emplace_back( Vec3f{ 1.f, prevY, prevZ } );

        pos.emplace_back( Vec3f{ 0.f, y, z } );
        pos.emplace_back( Vec3f{ 1.f, y, z } );
        pos.emplace_back( Vec3f{ 1.f, prevY, prevZ } );
        prevY = y;
        prevZ = z;
    }

    // caps
    if ( aCapped ) {
        float initialSize = pos.size();
        // midpoints are 0,0,0 and 1,0,0
        for ( std::size_t i = 0; i < initialSize; i += 6 ) {
            pos.emplace_back( pos[i] );
            pos.emplace_back( pos[i + 1] );
            // x=0 midpoint
            pos.emplace_back( Vec3f{ 0.f, 0.f, 0.f } );

            pos.emplace_back( pos[i + 4] );
            pos.emplace_back( pos[i + 5] );
            // x=1 midpoint
            pos.emplace_back( Vec3f{ 1.f, 0.f, 0.f } );
        }
    }

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

