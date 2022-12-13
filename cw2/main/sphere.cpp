#include "sphere.hpp"
#include <cstdio>

SimpleMeshData createSphere(Mat44f aPreTransform){
    tri triangle1;
    triangle1.a = Vec3f{1.f, 0.f, 0.f};
    triangle1.b = Vec3f{0.f, 1.f, 0.f};
    triangle1.c = Vec3f{0.f, 0.f, 1.f};

    SimpleMeshData first = createDome(triangle1);
    
    tri triangle2;
    triangle2.a = Vec3f{1.f, 0.f, 0.f};
    triangle2.b = Vec3f{0.f, -1.f, 0.f};
    triangle2.c = Vec3f{0.f, 0.f, 1.f};

    SimpleMeshData second = createDome(triangle2);

    SimpleMeshData ret = concatenate(first, second);

    for ( auto &p : ret.positions ) {
        Vec4f p4{ p.x, p.y, p.z, 1.f };
        Vec4f t = aPreTransform * p4;
        t /= t.w;

        p = Vec3f{ t.x, t.y, t.z };
    }

    return ret;
};   

SimpleMeshData createDome( tri startingTri ){
    std::vector<Vec3f> pos;

    pos = createTriangles(pos, startingTri, 9);

    for ( auto &p : pos ) {
        p = normalizePoints(Vec3f{0.f, 0.f, 0.f}, p, 2.f);
    
    }
    std::vector col( pos.size(), Vec3f{0.f, 1.f, 0.f} );

    return SimpleMeshData{ std::move( pos ), std::move( col ) };
};

tri halfTri(tri input){
    tri ret;

    ret.a = (0.5 * input.a) + (0.5 * input.b);
    ret.b = (0.5 * input.b) + (0.5 * input.c);
    ret.c = (0.5 * input.c) + (0.5 * input.a);

    return ret;
};


std::vector<Vec3f> createTriangles(std::vector<Vec3f> pos, tri abc, int counter){
    counter -= 1;
    if (counter == 0){
        return pos;
    }
    else{
        tri new_tri = halfTri(abc);

        tri triangle1, triangle2, triangle3;

        triangle1.a = abc.a;
        triangle1.b = new_tri.c;
        triangle1.c = new_tri.a;

        triangle2.a = new_tri.a;
        triangle2.b = abc.b;
        triangle2.c = new_tri.b;

        triangle3.a = new_tri.c;
        triangle3.b = new_tri.b;
        triangle3.c = abc.c;

        if (counter == 1){
            pos.emplace_back(new_tri.a);
            pos.emplace_back(new_tri.b);
            pos.emplace_back(new_tri.c);

            pos.emplace_back(triangle1.a);
            pos.emplace_back(triangle1.b);
            pos.emplace_back(triangle1.c);

            pos.emplace_back(triangle2.a);
            pos.emplace_back(triangle2.b);
            pos.emplace_back(triangle2.c);

            pos.emplace_back(triangle3.a);
            pos.emplace_back(triangle3.b);
            pos.emplace_back(triangle3.c);
        }

        pos = createTriangles(pos, triangle1, counter);
        pos = createTriangles(pos, triangle2, counter);
        pos = createTriangles(pos, triangle3, counter);

        return createTriangles(pos, new_tri, counter);
    }
};

Vec3f normalizePoints(Vec3f center, Vec3f point, float radius){
    float dx = point.x - center.x;
    float dy = point.y - center.y;
    float dz = point.z - center.z;

    dx = dx * radius / length(point - center);
    dy = dy * radius / length(point - center);
    dz = dz * radius / length(point - center);

    Vec3f c;
    c.x = center.x + dx;
    c.y = center.y + dy;
    c.z = center.z + dz;
    return c;
};