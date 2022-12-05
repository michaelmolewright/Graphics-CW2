#include "../vmlib/vec3.hpp"
#include "../vmlib/vec4.hpp"
#include "../vmlib/mat44.hpp"


struct camera {
    Vec3f cameraPosition = {0.f, 0.f, 3.f};
    Vec3f cameraFront = {0.f,0.f,-1.f};
    Vec3f cameraUp = {0.f,1.f,0.f};
    float cameraSpeed = 0.05f;
};

void updatePosition(GLFWwindow *aWindow, camera c)
{
    if (glfwGetKey(aWindow, GLFW_KEY_W) == GLFW_PRESS)
        c.cameraPosition += c.cameraSpeed * c.cameraFront;
    if (glfwGetKey(aWindow, GLFW_KEY_S) == GLFW_PRESS)
        c.cameraPosition -= c.cameraSpeed * c.cameraFront;
    if (glfwGetKey(aWindow, GLFW_KEY_A) == GLFW_PRESS)
        c.cameraPosition -= normalize(cross_product(c.cameraFront, c.cameraUp)) * c.cameraSpeed;
    if (glfwGetKey(aWindow, GLFW_KEY_D) == GLFW_PRESS)
        c.cameraPosition += normalize(cross_product(c.cameraFront, c.cameraUp)) * c.cameraSpeed;
};