
#include <vector>

#include <cstdlib>
#include <cstdio>

#include "../main/simple_mesh.hpp"

#include "../vmlib/vec3.hpp"
#include "../vmlib/mat44.hpp"

struct tri{
    Vec3f a;
    Vec3f b;
    Vec3f c;
};


SimpleMeshData createSphere();

SimpleMeshData createDome( 
    tri startingTri
);

tri newTri(
    tri input
);

std::vector<Vec3f> createTriangles(
    std::vector<Vec3f> pos, 
    tri abc, 
    int counter
);

Vec3f normalizePoints(
    Vec3f center,
    Vec3f point, 
    float radius
);


SimpleMeshData createFloor();
