#include "../vmlib/vec3.hpp"
#include "../vmlib/vec4.hpp"
#include "../vmlib/mat44.hpp"

class camera {
  public:
    Vec3f cameraPosition = { 0.f, 5.f, 20.f };
    Vec3f cameraFront = { 0.f, 0.f, -1.f };
    Vec3f cameraUp = { 0.f, 1.f, 0.f };
    float fastSpeed = 0.2f;
    float slowSpeed = 0.03f;
    float normalSpeed = 0.1f;
    float cameraSpeed = normalSpeed;

    bool moveForward = false;
    bool moveRight = false;
    bool moveLeft = false;
    bool moveBackward = false;
    bool moveUp = false;
    bool moveDown = false;
    bool fast = false;
    bool slow = false;

    void movement( int aKey, int aAction ) {
        if ( aKey == GLFW_KEY_W ) {
            if ( GLFW_PRESS == aAction ) {
                moveForward = true;
            }
            if ( GLFW_RELEASE == aAction ) {
                moveForward = false;
            }
        }
        if ( aKey == GLFW_KEY_S ) {
            if ( GLFW_PRESS == aAction ) {
                moveBackward = true;
            }
            if ( GLFW_RELEASE == aAction ) {
                moveBackward = false;
            }
        }
        if ( aKey == GLFW_KEY_A ) {
            if ( GLFW_PRESS == aAction ) {
                moveLeft = true;
            }
            if ( GLFW_RELEASE == aAction ) {
                moveLeft = false;
            }
        }
        if ( aKey == GLFW_KEY_D ) {
            if ( GLFW_PRESS == aAction ) {
                moveRight = true;
            }
            if ( GLFW_RELEASE == aAction ) {
                moveRight = false;
            }
        }
        if ( aKey == GLFW_KEY_Q ) {
            if ( GLFW_PRESS == aAction ) {
                moveUp = true;
            }
            if ( GLFW_RELEASE == aAction ) {
                moveUp = false;
            }
        }
        if ( aKey == GLFW_KEY_E ) {
            if ( GLFW_PRESS == aAction ) {
                moveDown = true;
            }
            if ( GLFW_RELEASE == aAction ) {
                moveDown = false;
            }
        }
        if ( aKey == GLFW_KEY_LEFT_CONTROL ) {
            if ( GLFW_PRESS == aAction ) {
                slow = true;
            }
            if ( GLFW_RELEASE == aAction ) {
                slow = false;
            }
        }
        if ( aKey == GLFW_KEY_LEFT_SHIFT ) {
            if ( GLFW_PRESS == aAction ) {
                fast = true;
            }
            if ( GLFW_RELEASE == aAction ) {
                fast = false;
            }
        }
    }

    void updatePosition() {
        if ( moveForward ) {
            cameraPosition += cameraSpeed * cameraFront;
        }
        if ( moveBackward ) {
            cameraPosition -= cameraSpeed * cameraFront;
        }
        if ( moveRight ) {
            cameraPosition +=
                normalize( cross_product( cameraFront, cameraUp ) ) *
                cameraSpeed;
        }
        if ( moveLeft ) {
            cameraPosition -=
                normalize( cross_product( cameraFront, cameraUp ) ) *
                cameraSpeed;
        }
        if ( moveDown ) {
            cameraPosition -= cameraUp * cameraSpeed;
        }
        if ( moveUp ) {
            cameraPosition += cameraUp * cameraSpeed;
        }
        if ( fast || slow ) {
            if ( fast && slow ) {
                cameraSpeed = normalSpeed;
            } else if ( fast ) {
                cameraSpeed = fastSpeed;
            } else if ( slow ) {
                cameraSpeed = slowSpeed;
            }
        } else {
            cameraSpeed = normalSpeed;
        }
    };
};