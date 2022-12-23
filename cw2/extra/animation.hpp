#ifndef animation_H
#define animation_H

#include "../vmlib/vec3.hpp"
#include "../vmlib/mat44.hpp"

float newPI = 3.1415926f;

Mat44f linearMotion( float distance, float aniLen, float counter,
                     Vec3f direction ) {
    float step = distance / aniLen;
    return make_translation( direction * ( step * counter ) );
}

Mat44f ollie( float distance, float height, float aniLen, float counter ) {
    float length = 1.f;
    float rotAmount = -newPI / 4.f;
    float stepDist = distance / aniLen;
    float stepHeight = height / aniLen;
    float stepRot = rotAmount / aniLen;
    if ( counter < ( aniLen / 2 ) )
        return make_translation(
                   { 0.f, stepHeight * counter, stepDist * counter } ) *
               make_translation( { 0.f, 0.f, -length } ) *
               make_rotation_x( stepRot * ( counter * 2 ) ) *
               make_translation( { 0.f, 0.f, length } );
    else
        return make_translation(
                   { 0.f, stepHeight * counter, stepDist * counter } ) *
               make_translation( { 0.f, 0.f, -length } ) *
               make_rotation_x(
                   rotAmount -
                   ( stepRot * ( ( counter - ( aniLen / 2 ) ) * 2 ) ) ) *
               make_translation( { 0.f, 0.f, length } );
}

Mat44f kickflip( float distance, float fall, float aniLen, float counter ) {
    float rotAmount = 2 * newPI;
    float stepDist = distance / aniLen;
    float stepFall = fall / aniLen;
    float stepRot = rotAmount / aniLen;

    // for the skateboard remember to chang the z by half of the width hand
    // height x and y
    return make_translation(
               { 0.5f, -stepFall * counter + 0.5f, stepDist * counter } ) *
           make_rotation_z( stepRot * counter ) *
           make_translation( { -0.5f, -0.5f, 0.f } );
}

Mat44f turnAround( float radius, float aniLen, float counter ) {
    float rotAmount = -newPI;
    float stepRot = rotAmount / aniLen;

    // for the skateboard remember to chang the z by half of the width hand
    // height x and y
    return make_translation( { -radius, 0.f, 0.f } ) *
           make_rotation_y( stepRot * counter ) *
           make_translation( { radius, 0.f, 0.f } );
}

Mat44f skateboardAimation( float &counter, float totalTime ) {
    if ( counter >= totalTime ) {
        counter = 0.f;
    }
    float segment = totalTime / 10.f;
    Mat44f res = kIdentity44f;
    if ( counter < segment )
        res = linearMotion( 3.f, segment, counter, { 0.f, 0.f, 1.f } );
    else if ( counter < ( segment * 2 ) ) {
        res = ollie( 2.f, 0.5f, segment, counter - segment );
        res = make_translation( { 0.f, 0.f, 3.f } ) * res;
    } else if ( counter < ( segment * 3 ) ) {
        res = linearMotion( 3.f, segment, counter - ( segment * 2 ),
                            { 0.f, 0.f, 1.f } );
        res = make_translation( { 0.f, 0.5f, 5.f } ) * res;
    } else if ( counter < ( segment * 4 ) ) {
        res = ollie( 2.f, 0.7f, segment, counter - ( segment * 3 ) );
        res = make_translation( { 0.f, 0.5f, 8.f } ) * res;
    } else if ( counter < ( segment * 5 ) ) {
        res = kickflip( 2.f, 1.2f, segment, counter - ( segment * 4 ) );
        res = make_translation( { 0.f, 1.2f, 10.f } ) * res;
    } else if ( counter < ( segment * 6 ) ) {
        res = turnAround( 1.5f, segment, counter - ( segment * 5 ) );
        res = make_translation( { 0.f, 0.f, 12.f } ) * res;
    } else if ( counter < ( segment * 9 ) ) {
        res = linearMotion( 12.f, ( segment * 3 ), counter - ( segment * 6 ),
                            { 0.f, 0.f, -1.f } );
        res = make_translation( { -3.f, 0.f, 12.f } ) * res *
              make_rotation_y( newPI );
    } else if ( counter < totalTime ) {
        res = turnAround( -1.5f, segment, counter - ( segment * 9 ) );
        res = make_translation( { -3.f, 0.f, 0.f } ) * res *
              make_rotation_y( newPI );
    }

    return res * make_translation( { 0.f, 0.f, 1.f } );
}
#endif