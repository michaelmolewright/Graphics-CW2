#include "cylinder.hpp"

// POST UNIFORMS
float const railAmb[] = { 0.f, 0.f, 0.f };
float const railDiff[] = { 0.01f, 0.01f, 0.01f };
float const railSpec[] = { 0.5f, 0.5f, 0.5f };
float const railShin( 16.f );

constexpr float pi = 3.1415926f;

Mat44f legs_scaling = make_scaling( 0.3f, 0.03f, 0.03f );

SimpleMeshData make_rail( std::size_t aSubdivs, Vec3f aColor,
                          Mat44f aPreTransform ) {

    std::vector<Vec3f> pos;

    auto leg1 = make_cylinder( true, aSubdivs, { 1.f, 0.f, 0.f },
                               // kIdentity44f
                               make_translation( { .5f, 0.f, 0.f } ) *
                                   make_rotation_z( pi / 2 ) * legs_scaling );
    auto leg2 = make_cylinder( true, aSubdivs, { 1.f, 0.f, 0.f },
                               // kIdentity44f
                               make_translation( { 1.5f, 0.f, 0.f } ) *
                                   make_rotation_z( pi / 2 ) * legs_scaling );
    auto leg3 = make_cylinder( true, aSubdivs, { 1.f, 0.f, 0.f },
                               // kIdentity44f
                               make_translation( { 2.5f, 0.f, 0.f } ) *
                                   make_rotation_z( pi / 2 ) * legs_scaling );

    auto main = make_cylinder( true, aSubdivs, { 1.f, 0.f, 0.f },
                               // kIdentity44f
                               make_translation( { 0.f, 0.3f, 0.f } ) *
                                   make_scaling( 3.f, 0.05f, 0.05f ) );

    // auto xarrow = concatenate( std::move( xcyl ), xcone );
    auto legs = concatenate( std::move( leg1 ), std::move( leg2 ) );
    legs = concatenate( std::move( legs ), std::move( leg3 ) );

    auto rail = concatenate( std::move( legs ), std::move( main ) );

    std::vector col( rail.positions.size(), aColor );

    // pre transform each point
    for ( auto &p : rail.positions ) {
        Vec4f p4{ p.x, p.y, p.z, 1.f };
        Vec4f t = aPreTransform * p4;
        t /= t.w;

        p = Vec3f{ t.x, t.y, t.z };
    }

    rail.colors = col;

    return rail;
}

void draw_rail( GLuint vao, Mat44f MVP, Mat44f model, size_t vertexCount ) {

    Mat44f railMVP = MVP * model;
    // LAMPPOST
    glUniform3fv( 6, 1, railAmb );    // amb
    glUniform3fv( 7, 1, railDiff );   // diff
    glUniform3fv( 8, 1, railSpec );   // spec
    glUniform1f( 9, railShin );      // shin

    glBindVertexArray( vao );
    glUniformMatrix4fv( 0, 1, GL_TRUE, railMVP.v );
    glUniformMatrix4fv( 1, 1, GL_TRUE, model.v );
    glDrawArrays( GL_TRIANGLES, 0, vertexCount );

}