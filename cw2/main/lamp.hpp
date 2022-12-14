#include "cube.hpp"
#include "cylinder.hpp"




void draw_lamp( GLuint lightVAO, GLuint postVAO, Mat44f aPreTransform ) {
    
    // light model is also light position



    Mat44f basicLightModel = make_translation( { 0.f, 2.f, 0.f } )
                        * make_scaling( 0.2f, 0.2f, 0.2f );

}

class Lamp {
   private:
        float postAmb[3];
        float postDiff[3];
        float postSpec[3];
        float postShin;
        Vec3f lightPos;

   public:
        Lamp( Vec3f lightPos )
            : postAmb { 0.f, 0.f, 0.f },
            postDiff { 0.01f, 0.01f, 0.01f },
            postSpec { 0.5f, 0.5f, 0.5f },
            postShin( 16.f ),
            lightPos(lightPos)
        {}

        void draw_lamp( GLuint lightVAO, GLuint postVAO, Mat44f aPreTransform ) {

        }
};

