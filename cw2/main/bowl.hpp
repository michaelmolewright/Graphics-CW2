
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

const float PI = 3.1415926f;


SimpleMeshData createSphere();

SimpleMeshData createDome( 
    tri startingTri,
    Mat44f preTransform
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


SimpleMeshData createFloor(Mat44f preTransform);

std::vector<Vec3f> sortVecs(std::vector<Vec3f> input);

SimpleMeshData make_bowl_cylinder( std::size_t aSubdivs, Vec3f aColor, Mat44f aPreTransform, float width );

SimpleMeshData createTile(Vec3f bl, Vec3f br, Vec3f tl, Vec3f tr, Mat44f preTransform);

SimpleMeshData createFinalForm(Mat44f preTransform);