constexpr float const kFloorPositions[] = {
    // coords				normals
    -1.f, 0.f, -1.f, 0.f, 1.f, 0.f,  -1.f, 0.f, 1.f,
    0.f,  1.f, 0.f,  1.f, 0.f, -1.f, 0.f,  1.f, 0.f,

    1.f,  0.f, -1.f, 0.f, 1.f, 0.f,  -1.f, 0.f, 1.f,
    0.f,  1.f, 0.f,  1.f, 0.f, 1.f,  0.f,  1.f, 0.f,
};

static float const floorAmb[] = { 0.35f, 0.20725f, 0.20725f };
static float const floorDiff[] = { 1.f, 0.829f, 0.829f };
static float const floorSpec[] = { 0.296648f, 0.296648f, 0.296648f };
static float const floorShin = 0.048f * 128;
// model matrix
Mat44f floorModel = make_scaling( 6.f, 1.f, 6.f );

GLuint create_floor_vao() {
    GLuint floorVBO = 0;
    glGenBuffers( 1, &floorVBO );
    glBindBuffer( GL_ARRAY_BUFFER, floorVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( kFloorPositions ), kFloorPositions,
                  GL_STATIC_DRAW );

    GLuint floorVAO = 0;
    glGenVertexArrays( 1, &floorVAO );
    glBindVertexArray( floorVAO );

    glBindBuffer( GL_ARRAY_BUFFER, floorVBO );
    // positions
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ),
                           (void *)0 );
    glEnableVertexAttribArray( 0 );
    // normals
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ),
                           (void *)( 3 * sizeof( float ) ) );
    glEnableVertexAttribArray( 1 );

    // reset and delete buffers
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glDeleteBuffers( 1, &floorVBO );

    return floorVAO;
}

void draw_floor( GLuint vao, Mat44f MVP ) {
    glUniformMatrix4fv( 0, 1, GL_TRUE, MVP.v );
    glUniformMatrix4fv( 1, 1, GL_TRUE, floorModel.v );   // model matrix

    // material props
    glUniform3fv( 6, 1, floorAmb );    // amb
    glUniform3fv( 7, 1, floorDiff );   // diff
    glUniform3fv( 8, 1, floorSpec );   // spec
    glUniform1f( 9, floorShin );      // shin

    // FLOOR
    glBindVertexArray( vao );
    glDrawArrays( GL_TRIANGLES, 0, 6 );
}