#include "../vmlib/vec3.hpp"
#include "../vmlib/vec4.hpp"
#include "../vmlib/mat44.hpp"


const float PIconv = 0.01745329251; 
const float shiftMult = 4.f;
const float ctrlMult = 0.5;

struct camera {
    Vec3f cameraPosition = {0.f, 2.f, 3.f};
    Vec3f cameraFront = {0.f,0.f,-1.f};
    Vec3f cameraUp = {0.f,1.f,0.f};
    float cameraSpeed = 0.2f;
};

float toRadians(float degs)
{
    return degs * PIconv;
};

void move(GLFWwindow* aWindow, auto* state)
{   
    //implement keys + shift multiplyer
    if (glfwGetKey(aWindow, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(aWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        state->c.cameraPosition += state->c.cameraSpeed * state->c.cameraFront * shiftMult;
        return;
    }
    else if (glfwGetKey(aWindow, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(aWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        state->c.cameraPosition -= state->c.cameraSpeed * state->c.cameraFront * shiftMult;
        return;
    }
    else if (glfwGetKey(aWindow, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(aWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        state->c.cameraPosition -= normalize(cross_product(state->c.cameraFront, state->c.cameraUp)) * state->c.cameraSpeed * shiftMult;
        return;
    }
    else if (glfwGetKey(aWindow, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(aWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        state->c.cameraPosition += normalize(cross_product(state->c.cameraFront, state->c.cameraUp)) * state->c.cameraSpeed * shiftMult;
        return;
    }
    else if (glfwGetKey(aWindow, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(aWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        state->c.cameraPosition += normalize(cross_product(cross_product(state->c.cameraFront, state->c.cameraUp), state->c.cameraFront)) * state->c.cameraSpeed * shiftMult; 
        return;
    }
    else if (glfwGetKey(aWindow, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(aWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        state->c.cameraPosition -= normalize(cross_product(cross_product(state->c.cameraFront, state->c.cameraUp), state->c.cameraFront)) * state->c.cameraSpeed * shiftMult;
        return;
    }

    //implement keys + control
    else if (glfwGetKey(aWindow, GLFW_KEY_W) == GLFW_PRESS && glfwGetKey(aWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        state->c.cameraPosition += state->c.cameraSpeed * state->c.cameraFront * ctrlMult;
        return;
    }
    else if (glfwGetKey(aWindow, GLFW_KEY_S) == GLFW_PRESS && glfwGetKey(aWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        state->c.cameraPosition -= state->c.cameraSpeed * state->c.cameraFront * ctrlMult;
        return;
    }
    else if (glfwGetKey(aWindow, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(aWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        state->c.cameraPosition -= normalize(cross_product(state->c.cameraFront, state->c.cameraUp)) * state->c.cameraSpeed * ctrlMult;
        return;
    }
    else if (glfwGetKey(aWindow, GLFW_KEY_D) == GLFW_PRESS && glfwGetKey(aWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        state->c.cameraPosition += normalize(cross_product(state->c.cameraFront, state->c.cameraUp)) * state->c.cameraSpeed * ctrlMult;
        return;
    }
    else if (glfwGetKey(aWindow, GLFW_KEY_Q) == GLFW_PRESS && glfwGetKey(aWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        state->c.cameraPosition += normalize(cross_product(cross_product(state->c.cameraFront, state->c.cameraUp), state->c.cameraFront)) * state->c.cameraSpeed * ctrlMult;
        return;
    }
    else if (glfwGetKey(aWindow, GLFW_KEY_E) == GLFW_PRESS && glfwGetKey(aWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        state->c.cameraPosition -= normalize(cross_product(cross_product(state->c.cameraFront, state->c.cameraUp), state->c.cameraFront)) * state->c.cameraSpeed * ctrlMult;
        return;
    }
    //implement just keys
    else if (glfwGetKey(aWindow, GLFW_KEY_W) == GLFW_PRESS){
        state->c.cameraPosition += state->c.cameraSpeed * state->c.cameraFront;
        return;
    }
    else if (glfwGetKey(aWindow, GLFW_KEY_S) == GLFW_PRESS){
        state->c.cameraPosition -= state->c.cameraSpeed * state->c.cameraFront;
        return;
    }
    else if (glfwGetKey(aWindow, GLFW_KEY_A) == GLFW_PRESS){
        state->c.cameraPosition -= normalize(cross_product(state->c.cameraFront, state->c.cameraUp)) * state->c.cameraSpeed;
        return;
    }
    else if (glfwGetKey(aWindow, GLFW_KEY_D) == GLFW_PRESS){
        state->c.cameraPosition += normalize(cross_product(state->c.cameraFront, state->c.cameraUp)) * state->c.cameraSpeed;
        return;
    }
    else if (glfwGetKey(aWindow, GLFW_KEY_Q) == GLFW_PRESS){
        state->c.cameraPosition += normalize(cross_product(cross_product(state->c.cameraFront, state->c.cameraUp), state->c.cameraFront)) * state->c.cameraSpeed;
        return;
    }
    else if (glfwGetKey(aWindow, GLFW_KEY_E) == GLFW_PRESS){
        state->c.cameraPosition -= normalize(cross_product(cross_product(state->c.cameraFront, state->c.cameraUp), state->c.cameraFront)) * state->c.cameraSpeed;
        return;
    }
};