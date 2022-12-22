#include <glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "stb_image_write.h"
#include "stb_image.h"

#include <typeinfo>
#include <stdexcept>
#include <sys/types.h>
#include <dirent.h>

#include <cstdio>
#include <cstdlib>
#include <string>

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
#include "../extra/animation.hpp"
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
#include "loadobj.hpp"
#include "skateboard.hpp"

namespace {
constexpr char const *kWindowTitle = "COMP3811 - Coursework 2";

constexpr float kPi_ = 3.1415926f;

// STATE FROM G3
struct State_ {
    ShaderProgram *prog;
};



camera c;

bool show_window = false;
float startX = 640, startY = 360;
float yaw = -90.f, pitch = 0.f;

float sizeOfFloor = 30.f;

void glfw_callback_error_( int, char const * );

void glfw_callback_key_( GLFWwindow *, int, int, int, int );

void mouse_movement( GLFWwindow *, double, double );

void screenshot( GLFWwindow * );

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
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set up event handling
    glfwSetKeyCallback( window, &glfw_callback_key_ );
    glfwSetCursorPosCallback( window, &mouse_movement );

    // Set up drawing stuff
    glfwMakeContextCurrent( window );
    glfwSwapInterval( 1 );   // V-Sync is on.

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");

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
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
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


    
    //--------------------------TEXTURES-------------------------------------------
    GLuint textureID1 = createTexture("./extra/concrete.png"); //file paths for windows
    GLuint textureID2 = createTexture("./extra/fence.png"); //file paths for windows
    GLuint textureID3 = createTexture("./extra/wood.jpg");
    GLuint textureID4 = createTexture("./assets/skateboard/texture.jpg");


    glActiveTexture( GL_TEXTURE0 );
    //-----------------------------------------------------------------------------

    //-----------------------------------------------------------------------------
    


    //--------------------------------LIGHTS---------------------------------------
    lamp l1;
    l1.createLamp(5.f, { 0.5f, 0.5f, 0.5f}, { 0.2f, 0.2f, 0.2f});
    lamp l2;
    l2.createLamp(5.f, { 0.5f, 0.5f, 0.5f}, { 0.2f, 0.2f, 0.2f});
    lamp l3;
    l3.createLamp(3.f, { 0.5f, 0.5f, 0.5f}, { 0.2f, 0.2f, 0.2f});

    int animationCounter = 0;
    float zLoc = 0.f;
    float sign = 1.f;
    //-----------------------------------------------------------------------------

    //--------------------------FLOOR----------------------------------------------
    plane p1;
    p1.createBox();
    p1.createRamp();
    p1.createComplexRamp();
    //-----------------------------------------------------------------------------

    // ----------------------------BOWL---------------------------------------------
    auto bowl = createFinalForm(
        make_scaling( sizeOfFloor / 9.f, 0.6f, 1.75f ) *
        make_translation( { -2.f, 2.f, 2.f } ) * make_rotation_x( PI / 2.f ) );
    GLuint bowl_vao = create_vao( bowl );
    std::size_t vertexCount = bowl.positions.size();


    // -----------------------------------------------------------------------------

    // RAIL
    auto rail = make_rail( 100, { 0.f, 0.f, 0.f }, make_scaling(2.f,1.75f, 2.f) );
    GLuint railVAO = create_vao( rail );

    // CUBE
    auto cube = make_cube( kIdentity44f );
    GLuint cubeVAO = create_vao( cube );

    // RAMP
    auto ramp = make_ramp( kIdentity44f );
    GLuint rampVAO = create_vao( ramp );
    Mat44f rampBoxModel =
        make_translation( { 6.f, 0.f, -10.f } ) * make_scaling( 6.f, 0.8f, 2.f );



    // SKATEBOARD
    auto skateboardMesh = load_wavefront_obj("./assets/skateboard/skateboard.obj");
    GLuint skateboardVAO = create_obj_vao(skateboardMesh);
    size_t skateboardVertexCount = skateboardMesh.positions.size();
    Mat44f skateboardModel = make_translation({4.f, 0.195f, 2.f}); 
    Mat44f flippedSBModel = make_translation({14.65f, 1.24f, 3.f})  * make_rotation_x( kPi_ ) * make_rotation_z(  3.2 * kPi_/2 ); 

    OGL_CHECKPOINT_ALWAYS();

    // Main loop
    while ( !glfwWindowShouldClose( window ) ) {
        // Let GLFW process events
        glfwPollEvents();


        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

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

        if (show_window)
        {
            ImGui::Begin("Control Menu");

            ImGui::Text("Press the X key to take a screenshot of the current frame\n\n");

            ImGui::ColorEdit3("Light 1 colour", (float*)&l1.lightColor);

            ImGui::ColorEdit3("Light 2 colour", (float*)&l2.lightColor);

            ImGui::ColorEdit3("Light 3 colour", (float*)&l3.lightColor);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);


            ImGui::End();
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
        glUniform1i(8, GL_FALSE);  // flag for drawing textures

        
        l1.drawLamp(baseMVP, make_translation({-sizeOfFloor/2.f, 0.f, -sizeOfFloor/2.f}), prog.programId(), "light[0]." );
        l2.drawLamp(baseMVP, make_translation({sizeOfFloor/2.f, 0.f, sizeOfFloor/2.f}), prog.programId(), "light[1]." );
        l3.drawLamp(baseMVP, make_translation({-sizeOfFloor/2.f, 0.f, sizeOfFloor/2.f}), prog.programId(), "light[2]." );


        //very simple animation
        /*if (animationCounter % 1000 == 0){
            sign *= -1.f;
        }
        animationCounter += 1;
        
        zLoc += sign * (sizeOfFloor/1000.f);*/

        

        
        setMaterialProperties("concrete");
        
        draw_bowl(
           vertexCount, bowl_vao, baseMVP,
           make_translation( { sizeOfFloor / 2.f, -1.f, sizeOfFloor / 2.f } ) *
               make_scaling( 1.f, 2.5f, 2.f ) );

        draw_rail( railVAO, baseMVP, make_translation( { -1.5f, 0.f, -8.f } )  ,
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
                  make_translation( { -12.f, 0.f, -10.f } ) *
                      make_scaling( 4.f, 0.25f, 7.f ) );
        
        // BIG RAMP
        draw_ramp( rampVAO, baseMVP,
                  make_translation( { 15.f, 0.f, 15.f } ) *
                      make_scaling( 30.f, 2.f, 6.f ) *
                     make_rotation_y( kPi_ ) );


        // animation rail
        setMaterialProperties("shineyMetal");
        draw_rail( railVAO, baseMVP, make_translation({0.32f, 0.f, -1.f}) * make_rotation_y(-PI/2.f) , rail.positions.size() );
        

        //-------------------------------------------------------------------------------------------------
        


        //---------------------------------------DRAWING-TEXTURED-OBJECTS------------------------------
        glUniform1i(8, GL_TRUE);   // flag for drawing textures
        setMaterialProperties("wood");
        //p1.drawRamp(textureID1, baseMVP, make_translation({0.f,10.f,0.f}));


        p1.drawComplexRamp(textureID3, baseMVP, make_translation({5.f,0.f,0.f}) * make_scaling(2.f,0.5f,2.f));

        p1.drawBox(textureID3, baseMVP, make_translation({5.f,0.f,0.f}) * make_scaling(2.f,0.5f,2.f));

        p1.drawComplexRamp(textureID3, baseMVP, make_translation({-8.f,0.f,4.f}) * make_scaling(2.f,1.f,2.f));

        p1.drawBox(textureID3, baseMVP, make_translation({-8.f,0.f,4.f}) * make_scaling(2.f,1.f,2.f));

        setMaterialProperties("skateboard");
        draw_skateboard( textureID4, skateboardVertexCount, skateboardVAO, baseMVP, flippedSBModel );

        //animation space
        Mat44f animationTranslation = skateboardAimation(animationCounter);

        draw_skateboard( textureID4, skateboardVertexCount, skateboardVAO, baseMVP, make_translation({0.f,0.f,-5.f}) * animationTranslation * make_scaling(0.64f, 0.391f, 2.5f) * make_translation({0.5f,0.5f,-0.5f}) * make_rotation_y(PI/2.f ) * make_scaling(1.f/2.5f, 1.f/0.391f, 1.f/0.64f));
        animationCounter += 2;



        setMaterialProperties("concrete");

        p1.drawBox(textureID1, baseMVP, make_translation({-sizeOfFloor/2.f, 0.f, sizeOfFloor/2.f}) * make_rotation_x(-kPi_ / 2.f) * make_scaling(sizeOfFloor, sizeOfFloor, 1.f));

        setMaterialProperties("shinyMetal");
        // INSIDE FENCES
        p1.drawTile(textureID2 , baseMVP, make_translation({-sizeOfFloor/2.f, 0.f, sizeOfFloor/2.f}) * make_rotation_y(kPi_ / 2.f) * make_scaling(sizeOfFloor, 2.f, 1.f));//left
        p1.drawTile(textureID2 , baseMVP, make_translation({sizeOfFloor/2.f, 0.f, -sizeOfFloor/2.f}) * make_rotation_y(-kPi_ / 2.f) * make_scaling(sizeOfFloor, 2.f, 1.f));//right
        p1.drawTile(textureID2 , baseMVP, make_translation({-sizeOfFloor/2.f, 0.f, -sizeOfFloor/2.f}) * make_scaling(sizeOfFloor, 2.f, 1.f));//front

        //OUTSIDE FENCES
        p1.drawTile(textureID2 , baseMVP, make_translation({-sizeOfFloor/2.f, 0.f, -sizeOfFloor/2.f}) * make_rotation_y(-kPi_ / 2.f) * make_scaling(sizeOfFloor, 2.f, 1.f));//left
        p1.drawTile(textureID2 , baseMVP, make_translation({sizeOfFloor/2.f, 0.f, sizeOfFloor/2.f}) * make_rotation_y(kPi_ / 2.f) * make_scaling(sizeOfFloor, 2.f, 1.f));//right
        p1.drawTile(textureID2 , baseMVP, make_translation({sizeOfFloor/2.f, 0.f, -sizeOfFloor/2.f}) * make_scaling(sizeOfFloor, 2.f, 1.f) * make_rotation_y(kPi_));//front
        
        // reset
        glBindVertexArray( 0 );
        glUseProgram( 0 );

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        OGL_CHECKPOINT_DEBUG();

        // Display results
        glfwSwapBuffers( window );
        glfwPollEvents();
    }

    // Cleanup.
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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

    if ( GLFW_KEY_M == aKey && GLFW_PRESS == aAction ) {
        show_window = !show_window;
        if (show_window){
            glfwSetInputMode( aWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
        }
        else{
            glfwSetInputMode( aWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
        }
        return;
    }

    if ( GLFW_KEY_X == aKey && GLFW_PRESS == aAction ) {
        screenshot( aWindow );
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
    dir.x = cosf( yaw * 0.01745329251f ) * cosf( pitch * 0.01745329251f );
    dir.y = sinf( pitch * 0.01745329251f );
    dir.z = sinf( yaw * 0.01745329251f ) * cosf( pitch * 0.01745329251f );


    if (!show_window){
        c.cameraFront = normalize( dir );
    }
}

void screenshot( GLFWwindow *window ) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    std::string savePath = "./screenshots/screenshot";

    // get number of files in screenshots
    DIR *screenshots;
    size_t i = 0;
    struct dirent *file;     
    screenshots = opendir ("./screenshots/");

    if (screenshots != NULL)
    {
        while ( (file = readdir(screenshots)) )
        i++;
        (void) closedir(screenshots);
    }
    else
        perror ("Couldn't open screenshots folder");

    // take away two (for . and ..)
    i -= 1;
    savePath += std::to_string(i) + ".png";

    int numChannels = 3; // rgb
    int stride = numChannels * width;

    // ensure stride is a multiple of 4 to save image
    if ( stride % 4 ) {
        stride += 4 - stride % 4;
    }

    // store framebuffer pixels into buffer
    int bufferSize = stride * height;
    std::vector<char> buffer( bufferSize );
    glPixelStorei( GL_PACK_ALIGNMENT, 4 );
    glReadBuffer( GL_FRONT );
    glReadPixels( 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data() );

    // save png
    stbi_flip_vertically_on_write(true);
    if ( stbi_write_png(savePath.c_str(), width, height, numChannels, buffer.data(), stride) ) {
        printf("\nScreenshot saved to %s \n", savePath.c_str());
        return;
    }
    printf("\nError attempting to save screenshot \n");
}

}   // namespace

namespace {
GLFWCleanupHelper::~GLFWCleanupHelper() { glfwTerminate(); }

GLFWWindowDeleter::~GLFWWindowDeleter() {
    if ( window )
        glfwDestroyWindow( window );
}
}   // namespace
