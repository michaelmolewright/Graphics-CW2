
#include <vector>

#include <cstdlib>
#include <cstdio>

#include "../main/simple_mesh.hpp"

#include "../vmlib/vec3.hpp"
#include "../vmlib/mat44.hpp"
static Vec3f const middleOfSphere = {0.f, 0.f, 0.f};
static float const bowlAmb[] = { 0.25f, 0.20725f, 0.20725f };
static float const bowlDiff[] = { 1.f, 0.829f, 0.829f };
static float const bowlSpec[] = { 0.296648f, 0.296648f, 0.296648f };
static float const bowlShin = 0.088f * 128;

static float const bowlCol[] ={ 0.25f, 0.25f, 0.25f };
static float const bowlShine = 4.f;

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

SimpleMeshData make_bowl_cylinder( std::size_t aSubdivs, Mat44f aPreTransform, float width );

SimpleMeshData createTile(Vec3f bl, Vec3f br, Vec3f tl, Vec3f tr, Mat44f preTransform);

SimpleMeshData createFinalForm(Mat44f preTransform);



void draw_bowl(std::size_t size, GLuint vao, Mat44f MVP, Mat44f transform);

std::vector<Vec3f> transformPoints(std::vector<Vec3f> input, Mat44f transform);

std::vector<Vec3f> transformNormals(std::vector<Vec3f> input, Mat44f transform);