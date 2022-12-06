#ifndef CUBE_HPP_6874B39C_112D_4D34_BD85_AB81A730955B
#define CUBE_HPP_6874B39C_112D_4D34_BD85_AB81A730955B

// This defines the vertex data for a colored unit cube.

constexpr float const kCubePositions[] = {
	+1.f, +1.f, -1.f,	0.f, 1.f,
	-1.f, +1.f, -1.f,	0.f, 0.f,	
	-1.f, +1.f, +1.f,	1.f, 0.f,
	+1.f, +1.f, -1.f,	0.f, 1.f,
	-1.f, +1.f, +1.f,	1.f, 0.f,
	+1.f, +1.f, +1.f,	1.f, 1.f,

	+1.f, -1.f, +1.f,	0.f, 1.f,
	+1.f, +1.f, +1.f,	0.f, 0.f,
	-1.f, +1.f, +1.f,	1.f, 0.f,
	+1.f, -1.f, +1.f,	0.f, 1.f,
	-1.f, +1.f, +1.f,	1.f, 0.f,
	-1.f, -1.f, +1.f,	1.f, 1.f,

	-1.f, -1.f, +1.f,	0.f, 1.f,
	-1.f, +1.f, +1.f,	0.f, 0.f,
	-1.f, +1.f, -1.f,	1.f, 0.f,
	-1.f, -1.f, +1.f,	0.f, 1.f,
	-1.f, +1.f, -1.f,	1.f, 0.f,
	-1.f, -1.f, -1.f,	1.f, 1.f,

	-1.f, -1.f, -1.f,	0.f, 1.f,
	+1.f, -1.f, -1.f,	0.f, 0.f,
	+1.f, -1.f, +1.f,	1.f, 0.f,
	-1.f, -1.f, -1.f,	0.f, 1.f,
	+1.f, -1.f, +1.f,	1.f, 0.f,
	-1.f, -1.f, +1.f,	1.f, 1.f,

	+1.f, -1.f, -1.f,	0.f, 1.f,
	+1.f, +1.f, -1.f,	0.f, 0.f,
	+1.f, +1.f, +1.f,	1.f, 0.f,
	+1.f, -1.f, -1.f,	0.f, 1.f,
	+1.f, +1.f, +1.f,	1.f, 0.f,
	+1.f, -1.f, +1.f,	1.f, 1.f,

	-1.f, -1.f, -1.f,	0.f, 1.f,
	-1.f, +1.f, -1.f,	0.f, 0.f,
	+1.f, +1.f, -1.f,	1.f, 0.f,
	-1.f, -1.f, -1.f,	0.f, 1.f,
	+1.f, +1.f, -1.f,	1.f, 0.f,
	+1.f, -1.f, -1.f,	1.f, 1.f,
};

constexpr float const kCubeColors[] = {
	+1.f, +0.f, +0.f,
	+1.f, +0.f, +0.f,
	+1.f, +0.f, +0.f,
	+1.f, +0.f, +0.f,
	+1.f, +0.f, +0.f,
	+1.f, +0.f, +0.f,

	+0.f, +1.f, +0.f,
	+0.f, +1.f, +0.f,
	+0.f, +1.f, +0.f,
	+0.f, +1.f, +0.f,
	+0.f, +1.f, +0.f,
	+0.f, +1.f, +0.f,

	+0.f, +0.f, +1.f,
	+0.f, +0.f, +1.f,
	+0.f, +0.f, +1.f,
	+0.f, +0.f, +1.f,
	+0.f, +0.f, +1.f,
	+0.f, +0.f, +1.f,

	+1.f, +0.f, +1.f,
	+1.f, +0.f, +1.f,
	+1.f, +0.f, +1.f,
	+1.f, +0.f, +1.f,
	+1.f, +0.f, +1.f,
	+1.f, +0.f, +1.f,

	+1.f, +1.f, +0.f,
	+1.f, +1.f, +0.f,
	+1.f, +1.f, +0.f,
	+1.f, +1.f, +0.f,
	+1.f, +1.f, +0.f,
	+1.f, +1.f, +0.f,

	+0.f, +1.f, +1.f,
	+0.f, +1.f, +1.f,
	+0.f, +1.f, +1.f,
	+0.f, +1.f, +1.f,
	+0.f, +1.f, +1.f,
	+0.f, +1.f, +1.f
};

GLuint createCubeVBO(){
	GLuint positionVBO = 0;
    glGenBuffers( 1, &positionVBO );
    glBindBuffer( GL_ARRAY_BUFFER, positionVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( kCubePositions ), kCubePositions,
                  GL_STATIC_DRAW );

    GLuint colorVBO = 0;
    glGenBuffers( 1, &colorVBO );
    glBindBuffer( GL_ARRAY_BUFFER, colorVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( kCubeColors ), kCubeColors,
                  GL_STATIC_DRAW );

    GLuint cubeVAO = 0;
    glGenVertexArrays( 1, &cubeVAO );
    glBindVertexArray( cubeVAO );

    glBindBuffer( GL_ARRAY_BUFFER, positionVBO );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, (5 * sizeof(float)), 0 );
    glEnableVertexAttribArray( 0 );

	glBindBuffer( GL_ARRAY_BUFFER, positionVBO );
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)) );
    glEnableVertexAttribArray( 0 );

    glBindBuffer( GL_ARRAY_BUFFER, colorVBO );
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( 1 );

    // reset and delete buffers
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glDeleteBuffers( 1, &positionVBO );
    glDeleteBuffers( 1, &colorVBO );
	return cubeVAO;
}

//static_assert( sizeof(kCubeColors) == sizeof(kCubePositions),
//	"Size of cube colors and cube positions do not match. Both are 3D vectors."
//);

#endif // CUBE_HPP_6874B39C_112D_4D34_BD85_AB81A730955B
