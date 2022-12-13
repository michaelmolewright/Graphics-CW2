#include "cylinder.hpp"
#include <cstdio>


constexpr float kPi_ = 3.1415926f;

SimpleMeshData make_half_pipe( std::size_t aSubdivs, Vec3f aColor,
                              Mat44f aPreTransform ) {

    std::vector<Vec3f> pos;

    float prevY = std::cos( 0.f );
    float prevZ = std::sin( 0.f );

    for ( std::size_t i = 0; i < aSubdivs/2; ++i ) {
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

    // top edge and bottom edge of half cyl
    Vec3f top1 = pos[0];
    Vec3f top2 = pos[2];
    Vec3f bot1 = pos[ pos.size()-3 ];
    Vec3f bot2 = pos[ pos.size()-2 ];

    // sides
    float initialSize = pos.size();
    // midpoints are 0,0,0 and 1,0,0
    for ( std::size_t i = 0; i < initialSize/2; i += 6 ) {
        pos.emplace_back( Vec3f{ 0.f, 1.f, 1.f } );
        pos.emplace_back( pos[i + 1] );
        pos.emplace_back( pos[i] );

        pos.emplace_back( pos[i + 4] );
        pos.emplace_back( pos[i + 5] );
        pos.emplace_back( Vec3f{ 1.f, 1.f, 1.f } );
    }
    for ( std::size_t i = initialSize/2; i < initialSize; i += 6 ) {
        pos.emplace_back( Vec3f{ 0.f, -1.f, 1.f } );
        pos.emplace_back( pos[i + 1] );
        pos.emplace_back( pos[i] );

        pos.emplace_back( pos[i + 4] );
        pos.emplace_back( pos[i + 5] );
        pos.emplace_back( Vec3f{ 1.f, -1.f, 1.f } );
    }

    // end of halfpipe    
    pos.emplace_back( top1 );
    pos.emplace_back( top2 );
    pos.emplace_back( Vec3f{ 0.f, 1.f, 1.f } );

    pos.emplace_back( Vec3f{ 1.f, 1.f, 1.f } );
    pos.emplace_back( Vec3f{ 0.f, 1.f, 1.f } );
    pos.emplace_back( top2 );

    // other end of halfpipe    
    pos.emplace_back( Vec3f{ 0.f, -1.f, 1.f } );
    pos.emplace_back( bot2 );
    pos.emplace_back( bot1 );

    pos.emplace_back( bot2 );
    pos.emplace_back( Vec3f{ 0.f, -1.f, 1.f } );
    pos.emplace_back( Vec3f{ 1.f, -1.f, 1.f } );

    // bottom of halfpipe
    pos.emplace_back( Vec3f{ 1.f, -1.f, 1.f } );
    pos.emplace_back( Vec3f{ 0.f, -1.f, 1.f } );
    pos.emplace_back( Vec3f{ 0.f, 1.f, 1.f } );

    pos.emplace_back( Vec3f{ 0.f, 1.f, 1.f } );
    pos.emplace_back( Vec3f{ 1.f, 1.f, 1.f } );
    pos.emplace_back( Vec3f{ 1.f, -1.f, 1.f } );

    

    std::vector col( pos.size(), aColor );

    // pre transform each point
    for ( auto &p : pos ) {
        Vec4f p4{ p.x, p.y, p.z, 1.f };
        Vec4f t = aPreTransform * 
            // rotations for halfpipe to face up
            make_rotation_z( 3 * kPi_ / 2 ) * 
            make_rotation_y( 3 * kPi_ / 2 ) *
            p4;
        t /= t.w;

        p = Vec3f{ t.x, t.y, t.z };
    }
    
    return SimpleMeshData{ std::move( pos ), std::move( col ) };
}

