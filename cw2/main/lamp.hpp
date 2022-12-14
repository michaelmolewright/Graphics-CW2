#include "cube.hpp"
#include "cylinder.hpp"

// lamp post material data
static float const postAmb[] = { 0.f, 0.f, 0.f };
static float const postDiff[] = { 0.01f, 0.01f, 0.01f };
static float const postSpec[] = { 0.5f, 0.5f, 0.5f };
static float const postShin = 16.f;



void draw_lamp( GLuint lightVAO, GLuint postVAO, Mat44f aPreTransform ) {
    
    // light model is also light position



    Mat44f basicLightModel = make_translation( { 0.f, 2.f, 0.f } )
                        * make_scaling( 0.2f, 0.2f, 0.2f );

}

