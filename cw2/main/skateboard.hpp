float const sbColor [] = { 0.6f, 0.2f, 0.6f };

void draw_skateboard(GLuint textureID, std::size_t size, GLuint vao, Mat44f MVP, Mat44f model){

    glBindTexture(GL_TEXTURE_2D, textureID);    

    Mat44f sbMVP = MVP * model;

    glBindVertexArray( vao );
    glUniformMatrix4fv( 0, 1, GL_TRUE, sbMVP.v );
    glUniformMatrix4fv( 1, 1, GL_TRUE, model.v );
    
    glUniform3fv( 5, 1, sbColor );    // object color
    glUniform1f( 7, 1.f );
    glDrawArrays( GL_TRIANGLES, 0, size );
}
