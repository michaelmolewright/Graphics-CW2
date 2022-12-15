#include "bowl.hpp"
#include <cstdio>
#include <iostream>

SimpleMeshData createSphere(){
    
    
    //outward facing sphere
    /*tri triangle;
    triangle1.a = Vec3f{1.f, 0.f, 0.f};
    triangle1.b = Vec3f{0.f, 1.f, 0.f};
    triangle1.c = Vec3f{0.f, 0.f, 1.f};*/
    
    //inward faciung sphere
    tri triangle1;
    triangle1.a = Vec3f{1.f, 0.f, 0.f};
    triangle1.b = Vec3f{0.f, -1.f, 0.f};
    triangle1.c = Vec3f{0.f, 0.f, 1.f};

    SimpleMeshData first = createDome(triangle1, kIdentity44f);

    SimpleMeshData second = createDome(triangle1, make_rotation_z( PI / 2.f));

    SimpleMeshData ret = concatenate(first, second);

    return ret;
};   

SimpleMeshData createDome( tri startingTri, Mat44f preTransform ){
    std::vector<Vec3f> pos;

    pos = createTriangles(pos, startingTri, 7);

    for ( auto &p : pos ) {
        p = normalizePoints(middleOfSphere, p, 2.f);
    
    }
    std::vector<Vec3f> normals;

    Vec3f p1;
    for ( int i=0; i < pos.size(); i++ ) {

        p1 = pos.at(i);
        Vec4f p4{ p1.x, p1.y, p1.z, 1.f };
        Vec4f t = preTransform * p4;
        t /= t.w;

        pos.at(i) = Vec3f{ t.x, t.y, t.z };

        normals.emplace_back(normalize(middleOfSphere - pos.at(i)));
    }

    return SimpleMeshData{ std::move( pos ), std::move( normals ) };
};

std::vector<Vec3f> transformPoints(std::vector<Vec3f> input, Mat44f transform){
    for ( auto &p : input ) {
        Vec4f p4{ p.x, p.y, p.z, 1.f };
        Vec4f t = transform * p4;
        t /= t.w;

        p = Vec3f{ t.x, t.y, t.z };
    }
    return input;
};

std::vector<Vec3f> transformNormals(std::vector<Vec3f> input, Mat44f transform){
    for ( auto &p : input ) {
        Vec4f p4{ p.x, p.y, p.z, 1.f };
        Vec4f t = transform * p4;
        t /= t.w;

        p = Vec3f{ t.x, t.y, t.z };
        //p = normalize(p);
    }
    return input;
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
        triangle1.b = new_tri.a;
        triangle1.c = new_tri.c;

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

SimpleMeshData createFloor(Mat44f preTransform){
    
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
    
    topPos1 = sortVecs(topPos1);

    for ( auto &p : topPos1 ){
        if (index == 0){    
            tempP1 = p;
            index = 1;
        }
        else{

            floorTris.emplace_back(tempP1);
            floorTris.emplace_back(p);
            floorTris.emplace_back(corner1);
            tempP1 = p;
        }
    }
    index = 0;
    topPos2 = sortVecs(topPos2);
    for ( auto &p : topPos2 ){
        if (index == 0){    
            tempP1 = p;
            index = 1;
        }
        else{

            floorTris.emplace_back(tempP1);
            floorTris.emplace_back(p);
            floorTris.emplace_back(corner2);
            tempP1 = p;
        }
    }
    std::vector normals( floorTris.size(), Vec3f{0.f, 0.f, -1.f} );

    SimpleMeshData floor = SimpleMeshData{ std::move( floorTris ), std::move( normals ) };

    a = concatenate(a, floor);

    a.positions = transformPoints(a.positions, preTransform);
    a.normals = transformNormals(a.normals, preTransform);
    
    
    return a;
}

std::vector<Vec3f> sortVecs(std::vector<Vec3f> input){
    Vec3f temp;

    for(unsigned int i = 0; i < input.size(); i++){
        for(unsigned int j = i+1; j < input.size(); j++){
            if(input.at(j).y < input.at(i).y) {
                temp = input.at(i);
                input.at(i) = input.at(j);
                input.at(j) = temp;
            }
        }
    }
    return input;
}

SimpleMeshData make_bowl_cylinder( std::size_t aSubdivs, Mat44f aPreTransform, float width ) {

    std::vector<Vec3f> pos;
    std::vector<Vec3f> norms;

    float prevY = std::cos( 0.f );
    float prevZ = std::sin( 0.f );

    Vec3f middleLeft = {0.f,0.f,0.f};
    Vec3f middleRight = {width,0.f,0.f};

    for ( std::size_t i = 0; i < aSubdivs; ++i ) {
        float const angle = ( i + 1 ) / float( aSubdivs ) * 3.1415926f;

        float y = std::cos( angle );
        float z = std::sin( angle );

        pos.emplace_back( Vec3f{ 0.f, prevY, prevZ } );
        pos.emplace_back( Vec3f{ width, prevY, prevZ } );
        pos.emplace_back( Vec3f{ 0.f, y, z } );

        norms.emplace_back( -normalize(middleLeft  - Vec3f{ 0.f, prevY, prevZ }) );
        norms.emplace_back( -normalize(middleRight - Vec3f{ width, prevY, prevZ }) );
        norms.emplace_back( -normalize(middleLeft  - Vec3f{ 0.f, y, z }) );

        pos.emplace_back( Vec3f{ 0.f, y, z } );
        pos.emplace_back( Vec3f{ width, prevY, prevZ } );
        pos.emplace_back( Vec3f{ width, y, z } );

        norms.emplace_back( -normalize(middleLeft  - Vec3f{ 0.f, y, z }) );
        norms.emplace_back( -normalize(middleRight - Vec3f{ width, prevY, prevZ }) );
        norms.emplace_back( -normalize(middleRight - Vec3f{ width, y, z }) );

        
        prevY = y;
        prevZ = z;
    }

    // pre transform each point
    pos = transformPoints(pos, aPreTransform);
    norms = transformNormals(norms, aPreTransform);

    return SimpleMeshData{ std::move( pos ), std::move( norms ) };
}

SimpleMeshData createTile( Mat44f preTransform ){

    std::vector<Vec3f> pos;

    pos.emplace_back(Vec3f{0.f, 1.f, 0.f});
    pos.emplace_back(Vec3f{0.f, 0.f, 0.f});
    pos.emplace_back(Vec3f{1.f, 0.f, 0.f});

    pos.emplace_back(Vec3f{0.f, 1.f, 0.f});
    pos.emplace_back(Vec3f{1.f, 0.f, 0.f});
    pos.emplace_back(Vec3f{1.f, 1.f, 0.f});

    std::vector col( pos.size(), Vec3f{0.5f, 0.f, 0.5f} );

    for ( auto &p : pos ) {
        Vec4f p4{ p.x, p.y, p.z, 1.f };
        Vec4f t = preTransform * p4;
        t /= t.w;

        p = Vec3f{ t.x, t.y, t.z };
    }

    return SimpleMeshData{ std::move( pos ), std::move( col ) };
}


SimpleMeshData createFinalForm(Mat44f preTransform){

    float length = 5.f;

    //SimpleMeshData ret = createFloor(kIdentity44f);
    
    SimpleMeshData floor1 = createFloor(kIdentity44f);

    SimpleMeshData floor2 = createFloor(make_rotation_z(PI) * make_translation({length, 0.f, 0.f}));make_scaling(1.f,2.f,2.f) * make_translation({-length, 0.f, 0.f});

    SimpleMeshData cyclinder = make_bowl_cylinder( 200, make_scaling(1.f,2.f,2.f) * make_translation({-length, 0.f, 0.f}), length);

    SimpleMeshData ret = concatenate(floor1, floor2);
    ret = concatenate(ret, cyclinder);

    SimpleMeshData tile1 = createTile(make_translation({-7.f,-2.f,2.f}) * make_scaling(9.f,4.f,1.f));
    ret = concatenate(ret, tile1);

    SimpleMeshData tile2 = createTile( make_rotation_y(PI/2.f) * make_translation({-2.f,-2.f, 2.f}) * make_scaling(2.f,4.f,1.f));
    ret = concatenate(ret, tile2);

    SimpleMeshData tile3 = createTile( make_translation({-7.f,-2.f,0.f}) * make_rotation_y(-PI/2.f) * make_scaling(2.f,4.f,1.f));
    ret = concatenate(ret, tile3);

    SimpleMeshData tile4 = createTile( make_translation({-7.f,2.f, 2.f}) * make_rotation_x(-PI/2.f) * make_scaling(9.f,2.f,1.f));
    ret = concatenate(ret, tile4);

    SimpleMeshData tile5 = createTile( make_translation({-7.f,-2.f, 0.f}) * make_rotation_x(PI/2.f) * make_scaling(9.f,2.f,1.f));
    ret = concatenate(ret, tile5);

    ret.positions = transformPoints(ret.positions, preTransform);

    ret.normals = transformNormals(ret.normals, preTransform);



    
    return ret;
}

void draw_bowl(std::size_t size, GLuint vao, Mat44f MVP, Mat44f transform){
    //glUniform3fv( 6, 1, bowlAmb );    // amb
    //glUniform3fv( 7, 1, bowlDiff );   // diff
    //glUniform3fv( 8, 1, bowlSpec );   // spec
    //glUniform1f( 9, bowlShin );      // shin

    //Mat44f newMVP = MVP * transform;

    glUniformMatrix4fv(0, 1, GL_TRUE, MVP.v);
    glUniformMatrix4fv(1, 1, GL_TRUE, transform.v);

    glBindVertexArray( vao );
    glDrawArrays( GL_TRIANGLES, 0, size );
}
