#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

GLuint createTexture( const char *file ) {
    GLuint textureID = 0;
    glGenTextures( 1, &textureID );
    glBindTexture( GL_TEXTURE_2D, textureID );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                     GL_REPEAT );   // set texture wrapping to GL_REPEAT
                                    // (default wrapping method)
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    // set texture filtering parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int w, h, chan;
    unsigned char *idata = stbi_load( file, &w, &h, &chan, 0 );
    if ( idata ) {
        if ( chan == 4 ) {
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA,
                          GL_UNSIGNED_BYTE, idata );
            glGenerateMipmap( GL_TEXTURE_2D );
        } else if ( chan == 3 ) {
            glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB,
                          GL_UNSIGNED_BYTE, idata );
            glGenerateMipmap( GL_TEXTURE_2D );
        }

        // glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
        std::cout << stbi_failure_reason() << std::endl;
    }
    free( idata );

    // glGenerateMipmapes( GL_TEXTURE_2D );

    return textureID;
}