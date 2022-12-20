#include <glad.h>
#include <GLFW/glfw3.h>

#include <typeinfo>
#include <stdexcept>

#include <cstdio>
#include <cstdlib>

#include "../support/error.hpp"
#include "../support/program.hpp"
#include "../support/checkpoint.hpp"
#include "../support/debug_output.hpp"

#include "../vmlib/vec4.hpp"
#include "../vmlib/mat44.hpp"

#include "defaults.hpp"
// TAKE OUT
#include "cube.hpp"
//#include "../extra/camera.hpp"
#include "../extra/newCamera.hpp"
#include "../extra/textures.hpp"
#include "floor.hpp"
#include "cylinder.hpp"
#include "half_pipe.hpp"
#include "lamp.hpp"
#include "bowl.hpp"
#include "rail.hpp"
#include "tile.hpp"
#include "material.hpp"
#include "newLamp.hpp"
#include "ramp.hpp"

namespace {
constexpr char const *kWindowTitle = "COMP3811 - Coursework 2";

constexpr float kPi_ = 3.1415926f;

// STATE FROM G3
struct State_ {
    ShaderProgram *prog;
};

camera c;

float startX = 640, startY = 360;
float yaw = -90.f, pitch = 0.f;

float sizeOfFloor = 20.f;

void glfw_callback_error_( int, char const * );

void glfw_callback_key_( GLFWwindow *, int, int, int, int );

void mouse_movement( GLFWwindow *, double, double );

struct GLFWCleanupHelper {
    ~GLFWCleanupHelper();
};
struct GLFWWindowDeleter {
    ~GLFWWindowDeleter();
    GLFWwindow *window;
};
}   // namespace

int main() try {
    // Initialize GLFW
    if ( GLFW_TRUE != glfwInit() ) {
        char const *msg = nullptr;
        int ecode = glfwGetError( &msg );
        throw Error( "glfwInit() failed with '%s' (%d)", msg, ecode );
    }

    // Ensure that we call glfwTerminate() at the end of the program.
    GLFWCleanupHelper cleanupHelper;

    // Configure GLFW and create window
    glfwSetErrorCallback( &glfw_callback_error_ );

    glfwWindowHint( GLFW_SRGB_CAPABLE, GLFW_TRUE );
    glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );

    // glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    glfwWindowHint( GLFW_DEPTH_BITS, 24 );

#if !defined( NDEBUG )
    // When building in debug mode, request an OpenGL debug context. This
    // enables additional debugging features. However, this can carry extra
    // overheads. We therefore do not do this for release builds.
    glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE );
#endif   // ~ !NDEBUG

    GLFWwindow *window =
        glfwCreateWindow( 1280, 720, kWindowTitle, nullptr, nullptr );

    if ( !window ) {
        char const *msg = nullptr;
        int ecode = glfwGetError( &msg );
        throw Error( "glfwCreateWindow() failed with '%s' (%d)", msg, ecode );
    }

    GLFWWindowDeleter windowDeleter{ window };

    // Set up event handling
    State_ state{};
    glfwSetWindowUserPointer( window, &state );
    // glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set up event handling
    glfwSetKeyCallback( window, &glfw_callback_key_ );
    glfwSetCursorPosCallback( window, &mouse_movement );

    // Set up drawing stuff
    glfwMakeContextCurrent( window );
    glfwSwapInterval( 1 );   // V-Sync is on.

    // Initialize GLAD
    // This will load the OpenGL API. We mustn't make any OpenGL calls before
    // this!
    if ( !gladLoadGLLoader( (GLADloadproc)&glfwGetProcAddress ) )
        throw Error( "gladLoaDGLLoader() failed - cannot load GL API!" );

    std::printf( "RENDERER %s\n", glGetString( GL_RENDERER ) );
    std::printf( "VENDOR %s\n", glGetString( GL_VENDOR ) );
    std::printf( "VERSION %s\n", glGetString( GL_VERSION ) );
    std::printf( "SHADING_LANGUAGE_VERSION %s\n",
                 glGetString( GL_SHADING_LANGUAGE_VERSION ) );

    // Ddebug output
#if !defined( NDEBUG )
    setup_gl_debug_output();
#endif   // ~ !NDEBUG

    //-----------GLOBAL-GL-SETUP-------------------------
    OGL_CHECKPOINT_ALWAYS();

    glEnable( GL_FRAMEBUFFER_SRGB );
    glEnable( GL_CULL_FACE );
    glClearColor( 0.26f, 0.75f, 0.98f, 0.0f );
    glEnable( GL_DEPTH_TEST );
    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    OGL_CHECKPOINT_ALWAYS();
    //-----------------------------------------------------



    // Get actual framebuffer size.
    // This can be different from the window size, as standard window
    // decorations (title bar, borders, ...) may be included in the window size
    // but not be part of the drawable surface area.
    int iwidth, iheight;
    glfwGetFramebufferSize( window, &iwidth, &iheight );

    glViewport( 0, 0, iwidth, iheight );

    // Other initialization & loading
    // TODO: load shaders
    ShaderProgram prog( { { GL_VERTEX_SHADER, "assets/default.vert" },
                          { GL_FRAGMENT_SHADER, "assets/workingThreeLights.frag" } } );
                        //   { GL_FRAGMENT_SHADER, "assets/default.frag" } } );

    state.prog = &prog;

    OGL_CHECKPOINT_ALWAYS();


    //--------------------RAIL----------------------------------------------------
    auto rail = make_rail( 100, {0.f,0.f,0.f}, kIdentity44f);
    GLuint railVAO = create_vao( rail );
    //----------------------------------------------------------------------------


    // CUBE
    GLuint cubeVAO = create_cube_vao();
    
    //--------------------------TEXTURES-------------------------------------------
    // REMEMBER TO CHANGE THIS TO RELATIVE PATH
    // GLuint textureID1 = createTexture("C:\\Users\\mikey\\Documents\\graphics\\graphics_cw2\\cw2\\extra\\concrete.png"); //file paths for windows
    // GLuint textureID2 = createTexture("C:\\Users\\mikey\\Documents\\graphics\\graphics_cw2\\cw2\\extra\\fence.png"); //file paths for windows
    // RELATIVE PATHS - YET TO TEST FOR MIKEY
    GLuint textureID1 = createTexture("./extra/concrete.png"); //file paths for windows
    GLuint textureID2 = createTexture("./extra/fence.png"); //file paths for windows
    glActiveTexture( GL_TEXTURE0 );
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    


    //--------------------------------LIGHTS---------------------------------------
    lamp l1;
    l1.createLamp(5.f, { 0.5f, 0.5f, 0.5f}, { 0.2f, 0.2f, 0.2f}, { 0.2f, 0.2f, 0.2f});
    lamp l2;
    l2.createLamp(5.f, { 0.5f, 0.5f, 0.5f}, { 0.2f, 0.2f, 0.2f}, { 0.5f, 0.5f, 0.5f});
    lamp l3;
    l3.createLamp(5.f, { 0.5f, 0.5f, 0.5f}, { 0.2f, 0.2f, 0.2f}, { 1.f, 1.f, 1.f});

    int animationCounter = 0;
    float zLoc = 0.f;
    float sign = 1.f;
    //-----------------------------------------------------------------------------

    //--------------------------FLOOR----------------------------------------------
    GLuint tileVAO = createTextureTileVao();
    plane p1;
    p1.createBox(textureID1);
    //-----------------------------------------------------------------------------

    // ----------------------------BOWL---------------------------------------------
    auto bowl = createFinalForm(
        make_scaling( sizeOfFloor / 9.f, 0.6f, 1.75f ) *
        make_translation( { -2.f, 2.f, 2.f } ) * make_rotation_x( PI / 2.f ) );
    GLuint bowl_vao = create_vao( bowl );
    std::size_t vertexCount = bowl.positions.size();
    // -----------------------------------------------------------------------------
    // RAIL
    auto rail = make_rail( 100, { 0.f, 0.f, 0.f }, kIdentity44f );
    GLuint railVAO = create_vao( rail );

    // CUBE
    auto cube = make_cube( kIdentity44f );
    GLuint cubeVAO = create_vao( cube );

    // RAMP
    auto ramp = make_ramp( kIdentity44f );
    GLuint rampVAO = create_vao( ramp );
    Mat44f rampBoxModel =
        make_translation( { 4.f, 0.f, -6.f } ) * make_scaling( 4.f, 0.5f, 1.f );

    OGL_CHECKPOINT_ALWAYS();

    // Main loop
    while ( !glfwWindowShouldClose( window ) ) {
        // Let GLFW process events
        glfwPollEvents();

        // Check if window was resized.
        float fbwidth, fbheight;
        {
            int nwidth, nheight;
            glfwGetFramebufferSize( window, &nwidth, &nheight );

            fbwidth = float( nwidth );
            fbheight = float( nheight );

            if ( 0 == nwidth || 0 == nheight ) {
                // Window minimized? Pause until it is unminimized.
                // This is a bit of a hack.
                do {
                    glfwWaitEvents();
                    glfwGetFramebufferSize( window, &nwidth, &nheight );
                } while ( 0 == nwidth || 0 == nheight );
            }
            glViewport( 0, 0, nwidth, nheight );
        }

        c.updatePosition();

        // compute baseMVP matrix i.e Proj * view
        Mat44f projection = make_perspective_projection(60.f * 3.1415926f / 180.f, fbwidth / float( fbheight ), 0.1f, 100.0f );

        Mat44f view = camMat( c.cameraPosition, c.cameraPosition + c.cameraFront, c.cameraUp );

        Mat44f baseMVP = projection * view;

        OGL_CHECKPOINT_DEBUG();

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glUseProgram( prog.programId() );

        // UNIFORMS
        float cameraPos[] = { c.cameraPosition.x, c.cameraPosition.y,
                              c.cameraPosition.z };
        glUniform3fv( 2, 1, cameraPos );   // camera position

        //-------------------------------------DRAWING-STARTS-HERE-----------------------------------------

        //------------------------------------DRAWING-NON-TEXTURED-OBJECTS---------------------------------
        glUniform1i(10, GL_FALSE);  // flag for drawing textures

        
        l1.drawLamp(baseMVP, make_translation({-sizeOfFloor/2.f, 0.f, -sizeOfFloor/2.f}), prog.programId(), "light[0]." );
        l2.drawLamp(baseMVP, make_translation({sizeOfFloor/2.f, 0.f, sizeOfFloor/2.f}), prog.programId(), "light[1]." );


        //very simple animation
        if (animationCounter % 1000 == 0){
            sign *= -1.f;
        }
        animationCounter += 1;
        
        zLoc += sign * (sizeOfFloor/2000.f);

        l2.drawLamp(baseMVP, make_translation({0.f, 0.f, zLoc}), prog.programId(), "light[2]." );

        
        setMaterialProperties("concrete");
        draw_bowl( vertexCount, bowl_vao, baseMVP, make_translation({sizeOfFloor/2.f, 0.f, sizeOfFloor/2.f}));
        glUniform1i( 10, GL_FALSE );

        draw_lamp( lightVAO, postVAO, baseMVP,
                   make_translation( { 0.f, 0.f, 0.f } ) );

        draw_bowl(
            vertexCount, bowl_vao, baseMVP,
            make_translation( { sizeOfFloor / 2.f, 0.f, sizeOfFloor / 2.f } ) *
                make_scaling( 1.f, 1.6f, 1.f ) );

        draw_rail( railVAO, baseMVP, make_translation( { -1.5f, 0.f, -8.f } ),
                   rail.positions.size() );

        // RAMP BOX
        draw_cube( cubeVAO, baseMVP, rampBoxModel );
        draw_ramp( rampVAO, baseMVP,
                   rampBoxModel * make_translation( { 1.f, 0.f, 0.f } ) *
                       make_rotation_y( kPi_ ) );
        draw_ramp( rampVAO, baseMVP,
                   rampBoxModel * make_translation( { 0.f, 0.f, 1.f } ) );

        // BOX
        draw_cube( cubeVAO, baseMVP,
                   make_translation( { -8.f, 0.f, -6.f } ) *
                       make_scaling( 4.f, 0.25f, 3.f ) );

        setMaterialProperties("shineyMetal");
        draw_rail( railVAO, baseMVP, make_translation({-3.f, 0.f, -4.f}), rail.positions.size() );
        // BIG RAMP
        draw_ramp( rampVAO, baseMVP,
                   make_translation( { 10.f, 0.f, 10.f } ) *
                       make_scaling( 20.f, 1.92f, 6.f ) *
                       make_rotation_y( kPi_ ) );

        draw_cube( cubeVAO, baseMVP, make_translation({2.f, 0.5f, -3.f}) );

        //-------------------------------------------------------------------------------------------------
        


        //---------------------------------------DRAWING-TEXTURED-OBJECTS------------------------------
        glUniform1i(10, GL_TRUE);   // flag for drawing textures

        setMaterialProperties("concrete");
        drawTile(textureID1 , baseMVP, make_translation({-sizeOfFloor/2.f, 0.f, sizeOfFloor/2.f}) * make_rotation_x(-kPi_ / 2.f) * make_scaling(sizeOfFloor, sizeOfFloor, 1.f) , tileVAO);

        p1.drawBox(baseMVP, make_translation({0.f,0.f,0.f}));

        setMaterialProperties("shinyMetal");
        // INSIDE FENCES
        drawTile(textureID2 , baseMVP, make_translation({-sizeOfFloor/2.f, 0.f, sizeOfFloor/2.f}) * make_rotation_y(kPi_ / 2.f) * make_scaling(sizeOfFloor, 2.f, 1.f), tileVAO);//left
        drawTile(textureID2 , baseMVP, make_translation({sizeOfFloor/2.f, 0.f, -sizeOfFloor/2.f}) * make_rotation_y(-kPi_ / 2.f) * make_scaling(sizeOfFloor, 2.f, 1.f) , tileVAO);//right
        drawTile(textureID2 , baseMVP, make_translation({-sizeOfFloor/2.f, 0.f, -sizeOfFloor/2.f}) * make_scaling(sizeOfFloor, 2.f, 1.f) , tileVAO);//front

        //OUTSIDE FENCES
        drawTile(textureID2 , baseMVP, make_translation({-sizeOfFloor/2.f, 0.f, -sizeOfFloor/2.f}) * make_rotation_y(-kPi_ / 2.f) * make_scaling(sizeOfFloor, 2.f, 1.f) , tileVAO);//left
        drawTile(textureID2 , baseMVP, make_translation({sizeOfFloor/2.f, 0.f, sizeOfFloor/2.f}) * make_rotation_y(kPi_ / 2.f) * make_scaling(sizeOfFloor, 2.f, 1.f), tileVAO);//right
        drawTile(textureID2 , baseMVP, make_translation({sizeOfFloor/2.f, 0.f, -sizeOfFloor/2.f}) * make_scaling(sizeOfFloor, 2.f, 1.f) * make_rotation_y(kPi_), tileVAO);//front
        
        // reset
        glBindVertexArray( 0 );
        glUseProgram( 0 );

        OGL_CHECKPOINT_DEBUG();

        // Display results
        glfwSwapBuffers( window );
        glfwPollEvents();
    }

    // Cleanup.
    state.prog = nullptr;
    // TODO: additional cleanup

    return 0;
} catch ( std::exception const &eErr ) {
    std::fprintf( stderr, "Top-level Exception (%s):\n",
                  typeid( eErr ).name() );
    std::fprintf( stderr, "%s\n", eErr.what() );
    std::fprintf( stderr, "Bye.\n" );
    return 1;
}

namespace {
void glfw_callback_error_( int aErrNum, char const *aErrDesc ) {
    std::fprintf( stderr, "GLFW error: %s (%d)\n", aErrDesc, aErrNum );
}

void glfw_callback_key_( GLFWwindow *aWindow, int aKey, int, int aAction,
                         int ) {

    if ( GLFW_KEY_ESCAPE == aKey && GLFW_PRESS == aAction ) {
        glfwSetWindowShouldClose( aWindow, GLFW_TRUE );
        return;
    }

    c.movement(aKey, aAction); //camera movement
}

void mouse_movement( GLFWwindow *aWindow, double xP, double yP ) {
    
    float xoffset = xP - startX;
    float yoffset = startY - yP;
    startX = xP;
    startY = yP;

    float sensitivity = 0.07f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if ( pitch > 89.0f )
        pitch = 89.0f;
    if ( pitch < -89.0f )
        pitch = -89.0f;

    Vec3f dir;

    // 0.01745329251 is degrees to radians conversion
    dir.x = cosf( yaw * 0.01745329251 ) * cosf( pitch * 0.01745329251 );
    dir.y = sinf( pitch * 0.01745329251 );
    dir.z = sinf( yaw * 0.01745329251 ) * cosf( pitch * 0.01745329251 );

    //state->c.cameraFront = normalize( dir );
    c.cameraFront = normalize( dir );
}
}   // namespace

namespace {
GLFWCleanupHelper::~GLFWCleanupHelper() { glfwTerminate(); }

GLFWWindowDeleter::~GLFWWindowDeleter() {
    if ( window )
        glfwDestroyWindow( window );
}
}   // namespace
