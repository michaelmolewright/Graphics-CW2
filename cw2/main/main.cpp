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
#include "../extra/camera.hpp"
#include "floor.hpp"
#include "cylinder.hpp"
#include "half_pipe.hpp"
#include "lamp.hpp"
#include "bowl.hpp"
#include "rail.hpp"

namespace {
constexpr char const *kWindowTitle = "COMP3811 - Coursework 2";

constexpr float kPi_ = 3.1415926f;

// STATE FROM G3
struct State_ {
    ShaderProgram *prog;

    camera c;
};

float startX = 640, startY = 360;
float yaw = -90.f, pitch = 0.f;

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

    // Global GL state
    OGL_CHECKPOINT_ALWAYS();

    // TODO: global GL setup goes here
    glEnable( GL_FRAMEBUFFER_SRGB );
    glEnable( GL_CULL_FACE );
    glClearColor( 0.6f, 0.6f, 0.6f, 0.0f );
    glEnable( GL_DEPTH_TEST );
    // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    OGL_CHECKPOINT_ALWAYS();

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
                          { GL_FRAGMENT_SHADER, "assets/default.frag" } } );

    state.prog = &prog;

    OGL_CHECKPOINT_ALWAYS();

    // TODO: VBO AND VAO setup

    // light position
    GLuint lightVAO = create_light_vao();
    Mat44f lightModel = make_translation( { 2.5f, 0.f, -3.5f } );
    // Mat44f lightModel = kIdentity44f;

    auto post = make_cylinder( true, 100, { 1.f, 0.f, 0.f },
                               // kIdentity44f
                               make_rotation_z( kPi_ / 2 ) *
                                   make_scaling( 2.f, 0.05f, 0.05f ) );
    GLuint postVAO = create_vao( post );

    // RAIL
    auto rail = make_rail( 100, {0.f,0.f,0.f}, make_translation({2.f, 0.f, 0.f}) );
    GLuint railVAO = create_vao( rail );

    

    //--------------------------FLOOR----------------------------------------------

    //-----------------------------------------------------------------------------

    // ----------------------------BOWL---------------------------------------------
    auto bowl = createFinalForm( make_scaling( 1.5f, 1.f, 2.f ) *
                                 make_translation( { 0.f, 0.f, 0.f } ) *
                                 make_rotation_x( kPi_ / 2.f ) );
    GLuint bowl_vao = create_vao( bowl );
    std::size_t vertexCount = bowl.positions.size();
    // -----------------------------------------------------------------------------

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

        // compute MVP matrix
        Mat44f projection = make_perspective_projection(
            45.f * 3.1415926f / 180.f,   // Yes, a proper π would be useful. (
                                         // C++20: mathematical constants)
            fbwidth / float( fbheight ), 0.1f, 100.0f );
        Mat44f view = camMat( state.c.cameraPosition,
                              state.c.cameraPosition + state.c.cameraFront,
                              state.c.cameraUp );

        Mat44f baseMVP = projection * view;

        // Draw scene
        OGL_CHECKPOINT_DEBUG();

        // // TODO: draw frame
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glUseProgram( prog.programId() );

        // UNIFORMS
        float cameraPos[] = { state.c.cameraPosition.x,
                              state.c.cameraPosition.y,
                              state.c.cameraPosition.z };
        glUniform3fv( 2, 1, cameraPos );   // camera position

        draw_lamp( lightVAO, postVAO, baseMVP, kIdentity44f );

        // draw_bowl( vertexCount, bowl_vao, baseMVP, kIdentity44f );

        draw_rail( railVAO, baseMVP, kIdentity44f, rail.positions.size() );


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
    auto *state = static_cast<State_ *>( glfwGetWindowUserPointer( aWindow ) );

    if ( GLFW_KEY_ESCAPE == aKey && GLFW_PRESS == aAction ) {
        glfwSetWindowShouldClose( aWindow, GLFW_TRUE );
        return;
    }
    move( aWindow, state );
}

void mouse_movement( GLFWwindow *aWindow, double xP, double yP ) {
    auto *state = static_cast<State_ *>( glfwGetWindowUserPointer( aWindow ) );

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
    dir.x = cosf( toRadians( yaw ) ) * cosf( toRadians( pitch ) );
    dir.y = sinf( toRadians( pitch ) );
    dir.z = sinf( toRadians( yaw ) ) * cosf( toRadians( pitch ) );

    state->c.cameraFront = normalize( dir );
}
}   // namespace

namespace {
GLFWCleanupHelper::~GLFWCleanupHelper() { glfwTerminate(); }

GLFWWindowDeleter::~GLFWWindowDeleter() {
    if ( window )
        glfwDestroyWindow( window );
}
}   // namespace
