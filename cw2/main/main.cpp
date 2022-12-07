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

namespace {
constexpr char const *kWindowTitle = "COMP3811 - Coursework 2";

constexpr float kPi_ = 3.1415926f;

// STATE FROM G3
struct State_ {
    ShaderProgram *prog;

    ShaderProgram *lighting;

    camera c;
};

float dt = 0.0f;
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
    //glEnable( GL_CULL_FACE );
    glClearColor( 0.f, 0.f, 0.f, 0.0f );
    glEnable( GL_DEPTH_TEST );

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
    // lighting shader
    ShaderProgram lighting( { { GL_VERTEX_SHADER, "assets/light.vert" },
                              { GL_FRAGMENT_SHADER, "assets/light.frag" } } );

    state.prog = &prog;
    state.lighting = &lighting;
    // state.camControl.radius = 10.f;

    OGL_CHECKPOINT_ALWAYS();

    // animation state
    // take out later
    auto last = Clock::now();
    float angle = 0.f;

    // TODO: VBO AND VAO setup
    // CUBE - now contains positions and normals
    GLuint cubeVBO = 0;
    glGenBuffers( 1, &cubeVBO );
    glBindBuffer( GL_ARRAY_BUFFER, cubeVBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( kCubePositions ), kCubePositions,
                  GL_STATIC_DRAW );

    GLuint cubeVAO = 0;
    glGenVertexArrays( 1, &cubeVAO );
    glBindVertexArray( cubeVAO );

    glBindBuffer( GL_ARRAY_BUFFER, cubeVBO );
    // positions
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ),
                           (void *)0 );
    glEnableVertexAttribArray( 0 );
    // normals
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ),
                           (void *)( 3 * sizeof( float ) ) );
    glEnableVertexAttribArray( 1 );

    // glBindBuffer( GL_ARRAY_BUFFER, colorVBO );
    // glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    // glEnableVertexAttribArray( 1 );

    // LIGHT CUBE
    GLuint lightVAO = 0;
    glGenVertexArrays( 1, &lightVAO );
    glBindVertexArray( lightVAO );
    glBindBuffer( GL_ARRAY_BUFFER, cubeVBO );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( float ),
                           (void *)0 );
    glEnableVertexAttribArray( 0 );

    // reset and delete buffers
    glBindVertexArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glDeleteBuffers( 1, &cubeVBO );

    // uniform colour for cube
    static float const uColor[] = { 1.0f, 0.5f, 0.31f };
    // lighting uniform data
    static float const lightColor[] = { 1.f, 1.f, 1.f };
    static float const lightPos[] = { 5.f, 1.0f, 2.0f };
    // translation for lighting cube
    Mat44f lightTranslate = make_translation( { 1.2f, 1.0f, 2.0f } );
    Mat44f lightScaling = make_scaling( 0.2f, 0.2f, 0.2f );

    OGL_CHECKPOINT_ALWAYS();

    // Main loop
    while ( !glfwWindowShouldClose( window ) ) {
        // Let GLFW process events
        // glfwPollEvents();

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

        // Update state
        auto const now = Clock::now();
        dt = std::chrono::duration_cast<Secondsf>( now - last ).count();
        last = now;

        angle += dt * kPi_ * 0.3f;
        if ( angle >= 2.f * kPi_ )
            angle -= 2.f * kPi_;

        // Update: compute matrices
        // TODO: define and compute projCameraWorld matrix
        Mat44f world2camera = make_translation( { 0.f, 0.f, -5.f } );
        Mat44f projection = make_perspective_projection(
            45.f * 3.1415926f / 180.f,   // Yes, a proper π would be useful. (
                                         // C++20: mathematical constants)
            fbwidth / float( fbheight ), 0.1f, 100.0f );
        Mat44f view = lookAt( state.c.cameraPosition,
                              state.c.cameraPosition + state.c.cameraFront,
                              state.c.cameraUp );
        Mat44f projCameraWorld = projection * view * world2camera;

        // lighting cube matrix
        Mat44f lightCubeMVP =
            projection * view * world2camera * lightTranslate * lightScaling;

        // Draw scene
        OGL_CHECKPOINT_DEBUG();

        // // TODO: draw frame
        // CUBE
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glUseProgram( prog.programId() );
        glBindVertexArray( cubeVAO );
        // pass in matrix as uniform data
        glUniformMatrix4fv( 0, 1, GL_TRUE, projCameraWorld.v );
        // light uniforms
        glUniform3fv( 1, 1, lightColor );
        glUniform3fv( 2, 1, lightPos );
        glUniformMatrix4fv( 4, 1, GL_TRUE, kIdentity44f.v ); // model matrix - we dont move cube so pass in identity matrix for now
        // uniform colour
        glUniform3fv( 3, 1, uColor );
        // 6 sides * 2 triangles * 3 vertices
        glDrawArrays( GL_TRIANGLES, 0, 6 * 2 * 3 );

        // change to lighting shader and draw light
        glUseProgram( lighting.programId() );
        glBindVertexArray( lightVAO );
        // lighting matrix
        glUniformMatrix4fv( 0, 1, GL_TRUE, lightCubeMVP.v );
        // 6 sides * 2 triangles * 3 vertices
        glDrawArrays( GL_TRIANGLES, 0, 6 * 2 * 3 );

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
    state.lighting = nullptr;
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
