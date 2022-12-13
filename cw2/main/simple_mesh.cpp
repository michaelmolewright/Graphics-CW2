#include "simple_mesh.hpp"

SimpleMeshData concatenate( SimpleMeshData aM, SimpleMeshData const &aN ) {
    aM.positions.insert( aM.positions.end(), aN.positions.begin(),
                         aN.positions.end() );
    aM.colors.insert( aM.colors.end(), aN.colors.begin(), aN.colors.end() );
    return aM;
}

// tests whether a triangle is drawn counter clockwise or not
// returns 1 if counterclockwise, -1 if clockwise
int ccw( Vec3f a, Vec3f b, Vec3f c ) {
    return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
}

// calculate normals for SimpleMeshData faces
std::vector<Vec3f> calculate_normals( SimpleMeshData const &aMeshData) {

    std::vector<Vec3f> normals;

    for (size_t i = 0; i < aMeshData.positions.size(); i += 3) {

        Vec3f a = aMeshData.positions[i];
        Vec3f b = aMeshData.positions[i+1];
        Vec3f c = aMeshData.positions[i+2];

        Vec3f AB = aMeshData.positions[i+1] - aMeshData.positions[i];
        Vec3f AC = aMeshData.positions[i+2] - aMeshData.positions[i];

        Vec3f n = cross_product( AB, AC);
        Vec3f norm = normalize( n );

        // if triangle is clockwise, reverse the normal
        // if ( ccw( a, b, c) ) {
        //     norm *= -1;
        // }

        normals.emplace_back(norm);
        normals.emplace_back(norm);
        normals.emplace_back(norm);
    }
    return normals;
}


GLuint create_vao( SimpleMeshData const &aMeshData ) {
    GLuint positionVBO = 0;
    glGenBuffers( 1, &positionVBO );
    glBindBuffer( GL_ARRAY_BUFFER, positionVBO );
    glBufferData( GL_ARRAY_BUFFER, aMeshData.positions.size() * sizeof( Vec3f ),
                  aMeshData.positions.data(), GL_STATIC_DRAW );

    // GLuint colorVBO = 0;
    // glGenBuffers( 1, &colorVBO );
    // glBindBuffer( GL_ARRAY_BUFFER, colorVBO );
    // glBufferData( GL_ARRAY_BUFFER, aMeshData.colors.size() * sizeof( Vec3f ),
    //               aMeshData.colors.data(), GL_STATIC_DRAW );

    std::vector<Vec3f> normals = calculate_normals( aMeshData );
    GLuint normalVBO = 0;
    glGenBuffers( 1, &normalVBO );
    glBindBuffer( GL_ARRAY_BUFFER, normalVBO );
    glBufferData( GL_ARRAY_BUFFER, normals.size() * sizeof( Vec3f ),
                  normals.data(), GL_STATIC_DRAW );

    GLuint vao = 0;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    glBindBuffer( GL_ARRAY_BUFFER, positionVBO );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 0 );

    // glBindBuffer( GL_ARRAY_BUFFER, colorVBO );
    // glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    // glEnableVertexAttribArray( 1 );

    // NORMALS
    glBindBuffer( GL_ARRAY_BUFFER, normalVBO );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 1 );

    // reset and delete buffers
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glDeleteBuffers( 1, &positionVBO );
    glDeleteBuffers( 1, &normalVBO );
    // glDeleteBuffers( 1, &colorVBO );

    return vao;
}
