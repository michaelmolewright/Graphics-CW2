#include "simple_mesh.hpp"

// calculate normals for SimpleMeshData faces
std::vector<Vec3f> calculate_normals( SimpleMeshData const &aMeshData ) {

    std::vector<Vec3f> normals;

    for ( size_t i = 0; i < aMeshData.positions.size(); i += 3 ) {

        Vec3f a = aMeshData.positions[i];
        Vec3f b = aMeshData.positions[i + 1];
        Vec3f c = aMeshData.positions[i + 2];

        Vec3f AB = b - a;
        Vec3f AC = c - a;

        Vec3f n = cross_product( AC, AB );
        Vec3f norm = -normalize( n );

        normals.emplace_back( norm );
        normals.emplace_back( norm );
        normals.emplace_back( norm );
    }
    return normals;
}

std::vector<Vec3f>
calculate_reversed_normals( SimpleMeshData const &aMeshData ) {

    std::vector<Vec3f> normals;

    for ( size_t i = 0; i < aMeshData.positions.size(); i += 3 ) {

        Vec3f a = aMeshData.positions[i];
        Vec3f b = aMeshData.positions[i + 1];
        Vec3f c = aMeshData.positions[i + 2];

        Vec3f AB = b - a;
        Vec3f AC = c - a;

        Vec3f n = cross_product( AC, AB );
        Vec3f norm = -normalize( n );

        normals.emplace_back( -1 * norm );
        normals.emplace_back( -1 * norm );
        normals.emplace_back( -1 * norm );
    }
    return normals;
}

SimpleMeshData concatenate( SimpleMeshData aM, SimpleMeshData const &aN ) {
    aM.positions.insert( aM.positions.end(), aN.positions.begin(),
                         aN.positions.end() );
    aM.normals.insert( aM.normals.end(), aN.normals.begin(), aN.normals.end() );
    aM.textureCoord.insert( aM.textureCoord.end(), aN.textureCoord.begin(),
                            aN.textureCoord.end() );
    return aM;
}

GLuint create_vao( SimpleMeshData const &aMeshData ) {
    // POSITIONS
    GLuint positionVBO = 0;
    glGenBuffers( 1, &positionVBO );
    glBindBuffer( GL_ARRAY_BUFFER, positionVBO );
    glBufferData( GL_ARRAY_BUFFER, aMeshData.positions.size() * sizeof( Vec3f ),
                  aMeshData.positions.data(), GL_STATIC_DRAW );

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

    // NORMALS
    glBindBuffer( GL_ARRAY_BUFFER, normalVBO );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 1 );

    // reset and delete buffers
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glDeleteBuffers( 1, &positionVBO );
    glDeleteBuffers( 1, &normalVBO );

    return vao;
}

GLuint reversedNormalsVAO( SimpleMeshData const &aMeshData ) {
    GLuint positionVBO = 0;
    glGenBuffers( 1, &positionVBO );
    glBindBuffer( GL_ARRAY_BUFFER, positionVBO );
    glBufferData( GL_ARRAY_BUFFER, aMeshData.positions.size() * sizeof( Vec3f ),
                  aMeshData.positions.data(), GL_STATIC_DRAW );

    std::vector<Vec3f> normals = calculate_reversed_normals( aMeshData );
    GLuint normalVBO = 0;
    glGenBuffers( 1, &normalVBO );
    glBindBuffer( GL_ARRAY_BUFFER, normalVBO );
    glBufferData( GL_ARRAY_BUFFER, normals.size() * sizeof( Vec3f ),
                  normals.data(), GL_STATIC_DRAW );

    // TEX COORDS
    GLuint textureVBO = 0;
    glGenBuffers( 1, &textureVBO );
    glBindBuffer( GL_ARRAY_BUFFER, textureVBO );
    glBufferData( GL_ARRAY_BUFFER,
                  aMeshData.textureCoord.size() * sizeof( Vec2f ),
                  aMeshData.textureCoord.data(), GL_STATIC_DRAW );

    GLuint vao = 0;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    glBindBuffer( GL_ARRAY_BUFFER, positionVBO );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 0 );

    glBindBuffer( GL_ARRAY_BUFFER, normalVBO );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 1 );

    glBindBuffer( GL_ARRAY_BUFFER, textureVBO );
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 2 );

    // reset and delete buffers
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glDeleteBuffers( 1, &positionVBO );
    glDeleteBuffers( 1, &normalVBO );
    glDeleteBuffers( 1, &textureVBO );

    return vao;
}

GLuint create_full_vao( SimpleMeshData const &aMeshData ) {
    GLuint positionVBO = 0;
    glGenBuffers( 1, &positionVBO );
    glBindBuffer( GL_ARRAY_BUFFER, positionVBO );
    glBufferData( GL_ARRAY_BUFFER, aMeshData.positions.size() * sizeof( Vec3f ),
                  aMeshData.positions.data(), GL_STATIC_DRAW );

    std::vector<Vec3f> normals = calculate_normals( aMeshData );
    GLuint normalVBO = 0;
    glGenBuffers( 1, &normalVBO );
    glBindBuffer( GL_ARRAY_BUFFER, normalVBO );
    glBufferData( GL_ARRAY_BUFFER, normals.size() * sizeof( Vec3f ),
                  normals.data(), GL_STATIC_DRAW );

    GLuint textureVBO = 0;
    glGenBuffers( 1, &textureVBO );
    glBindBuffer( GL_ARRAY_BUFFER, textureVBO );
    glBufferData( GL_ARRAY_BUFFER,
                  aMeshData.textureCoord.size() * sizeof( Vec2f ),
                  aMeshData.textureCoord.data(), GL_STATIC_DRAW );

    GLuint vao = 0;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    glBindBuffer( GL_ARRAY_BUFFER, positionVBO );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 0 );

    glBindBuffer( GL_ARRAY_BUFFER, normalVBO );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 1 );

    glBindBuffer( GL_ARRAY_BUFFER, textureVBO );
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 2 );

    // reset and delete buffers
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glDeleteBuffers( 1, &positionVBO );
    glDeleteBuffers( 1, &normalVBO );
    glDeleteBuffers( 1, &textureVBO );

    return vao;
}

SimpleMeshData transformPositions( SimpleMeshData mesh, Mat44f transform ) {
    SimpleMeshData res;
    std::vector<Vec3f> positions;
    res.textureCoord = mesh.textureCoord;
    for ( auto &p : mesh.positions ) {
        Vec4f p4{ p.x, p.y, p.z, 1.f };
        Vec4f t = transform * p4;
        t /= t.w;

        positions.emplace_back( Vec3f{ t.x, t.y, t.z } );
    }
    res.positions = positions;
    return res;
}

GLuint create_obj_vao( SimpleMeshData const &aMeshData ) {
    GLuint positionVBO = 0;
    glGenBuffers( 1, &positionVBO );
    glBindBuffer( GL_ARRAY_BUFFER, positionVBO );
    glBufferData( GL_ARRAY_BUFFER, aMeshData.positions.size() * sizeof( Vec3f ),
                  aMeshData.positions.data(), GL_STATIC_DRAW );

    GLuint normalVBO = 0;
    glGenBuffers( 1, &normalVBO );
    glBindBuffer( GL_ARRAY_BUFFER, normalVBO );
    glBufferData( GL_ARRAY_BUFFER, aMeshData.normals.size() * sizeof( Vec3f ),
                  aMeshData.normals.data(), GL_STATIC_DRAW );

    GLuint textureVBO = 0;
    glGenBuffers( 1, &textureVBO );
    glBindBuffer( GL_ARRAY_BUFFER, textureVBO );
    glBufferData( GL_ARRAY_BUFFER,
                  aMeshData.textureCoord.size() * sizeof( Vec2f ),
                  aMeshData.textureCoord.data(), GL_STATIC_DRAW );

    GLuint vao = 0;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    glBindBuffer( GL_ARRAY_BUFFER, positionVBO );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 0 );

    glBindBuffer( GL_ARRAY_BUFFER, normalVBO );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 1 );

    glBindBuffer( GL_ARRAY_BUFFER, textureVBO );
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 2 );

    // reset and delete buffers
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glDeleteBuffers( 1, &positionVBO );
    glDeleteBuffers( 1, &normalVBO );
    return vao;
}
