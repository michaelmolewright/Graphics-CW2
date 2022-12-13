#include "sphere.hpp"
#include <cstdio>

SimpleMeshData createSphere(){
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

    /*for ( auto &p : ret.positions ) {
        Vec4f p4{ p.x, p.y, p.z, 1.f };
        Vec4f t = aPreTransform * p4;
        t /= t.w;

        p = Vec3f{ t.x, t.y, t.z };
    }*/

    return ret;
};   

SimpleMeshData createDome( tri startingTri ){
    std::vector<Vec3f> pos;

    pos = createTriangles(pos, startingTri, 7);

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

SimpleMeshData createFloor(){
    
    SimpleMeshData a = createSphere();

    Vec3f corner1 = {2.f, 2.f, 0.f};
    Vec3f corner2 = {2.f, -2.f, 0.f};
    std::vector<Vec3f> topPos1;
    std::vector<Vec3f> topPos2;

    for ( auto &p : a.positions ){
        if ( (p.z == 0.f) && (p.y > 0.f) ){
            topPos1.emplace_back(p);
        }
        else if( (p.z == 0.f) && (p.y < 0.f) ){
            topPos2.emplace_back(p);
        }
    }
    std::vector<Vec3f> floorTris;
    int index = 0;
    Vec3f tempP1;

    for(unsigned int i = 0; i < topPos1.size() - 1; i++)
    {
        if (i == 0){    
            tempP1 = topPos1.at(i);
        }
        else{

            floorTris.emplace_back(tempP1);
            floorTris.emplace_back(topPos1.at(i));
            floorTris.emplace_back(corner1);
            tempP1 = topPos1.at(i);
        }
    }
    index = 0;
    Vec3f tempP2;
    /*for ( auto &p : topPos2 ){
        if (index == 0){    
            tempP2 = p;
            index = 1;
        }
        else{

            floorTris.emplace_back(tempP2);
            floorTris.emplace_back(p);
            floorTris.emplace_back(corner2);
            tempP2 = p;
        }
    }*/
    std::vector col( floorTris.size(), Vec3f{1.f, 0.f, 0.f} );

    SimpleMeshData floor = SimpleMeshData{ std::move( floorTris ), std::move( col ) };

    a = concatenate(a, floor);
    
    
    return a;
}