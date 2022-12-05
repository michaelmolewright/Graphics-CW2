#include "../vmlib/vec3.hpp"
#include "../vmlib/vec4.hpp"
#include "../vmlib/mat44.hpp"

const float PIconv = 0.01745329251; 
struct camera {
    Vec3f cameraPosition = {0.f, 0.f, 3.f};
    Vec3f cameraFront = {0.f,0.f,-1.f};
    Vec3f cameraUp = {0.f,1.f,0.f};
    float cameraSpeed = 0.05f;
};

float toRadians(float degs)
{
    return degs * PIconv;
};