#ifndef MATERIALS_HEADER_GUARD
#define MATERIALS_HEADER_GUARD

#include <cstdlib>
#include <cstdio>
#include "../vmlib/vec3.hpp"
#include "../vmlib/mat44.hpp"


//-----------CONCRETE--------------
float concrete_ambient[] ={ 0.05f,0.05f,0.05f,1.0f };
float concrete_diffuse[] ={ 0.5f,0.5f,0.5f,1.0f};
float concrete_specular[] ={ 0.7f,0.7f,0.7f,1.0f};
float concrete_shine = 6.0f;
float concrete_emmisive = 0.1f;
//---------------------------------

//-----------shinyMetal--------------
float shinyMetal_ambient[] = {0.23125f, 0.23125f, 0.23125f };
float shinyMetal_diffuse[] = {0.2775f, 0.2775f, 0.2775f };
float shinyMetal_specular[] = {0.774597f, 0.774597f, 0.774597f};
float shinyMetal_shine = 89.6f;
float shinyMetal_emmisive = 0.001f;
//---------------------------------

//-----------yellow_rubber--------------
float yellow_rubber_ambient[] ={ 0.05f,0.05f,0.0f,1.0f };
float yellow_rubber_diffuse[] ={0.5f,0.5f,0.4f,1.0f};
float yellow_rubber_specular[] ={0.7f,0.7f,0.04f,1.0f };
float yellow_rubber_shine = 10.0f;
float yellow_rubber_emmisive = 0.1f;
//-----------------------------------

//-----------lamp-----------------
float lamp_ambient[] = {0.8f, 0.8f, 0.8f };
float lamp_diffuse[] = {0.8f, 0.8f, 0.8f };
float lamp_specular[] = {0.1f, 0.1f, 0.1f};
float lamp_shine = 100.f;
float lamp_emmisive = 0.0001f;
//---------------------------------

//-----------wood-----------------
float wood_ambient[] = {0.2f, 0.2f, 0.2f };
float wood_diffuse[] = {0.6f, 0.45f, 0.2f };
float wood_specular[] = {0.34f, 0.34f, 0.34f};
float wood_shine = 2.f;
float wood_emmisive = 0.05f;
//---------------------------------

void setMaterialProperties(std::string material){
    if (material == "concrete"){
        glUniform3fv(3,1, concrete_ambient);
        glUniform3fv(4,1, concrete_diffuse);
        glUniform3fv(5,1, concrete_specular);
        glUniform1f(6, concrete_shine);
        glUniform1f(7, concrete_emmisive);
    }
    else if (material == "shinyMetal"){
        glUniform3fv(3,1, shinyMetal_ambient);
        glUniform3fv(4,1, shinyMetal_diffuse);
        glUniform3fv(5,1, shinyMetal_specular);
        glUniform1f(6, shinyMetal_shine);
        glUniform1f(7, shinyMetal_emmisive);
    }
    else if (material == "yellowRubber"){
        glUniform3fv(3,1, yellow_rubber_ambient);
        glUniform3fv(4,1, yellow_rubber_diffuse);
        glUniform3fv(5,1, yellow_rubber_specular);
        glUniform1f(6, yellow_rubber_shine);
        glUniform1f(7, yellow_rubber_emmisive);
    }
    else if (material == "lampMaterial"){
        glUniform3fv(3,1, lamp_ambient);
        glUniform3fv(4,1, lamp_diffuse);
        glUniform3fv(5,1, lamp_specular);
        glUniform1f(6, lamp_shine);
        glUniform1f(7, lamp_emmisive);
    }
    else if (material == "wood"){
        glUniform3fv(3,1, wood_ambient);
        glUniform3fv(4,1, wood_diffuse);
        glUniform3fv(5,1, wood_specular);
        glUniform1f(6, wood_shine);
        glUniform1f(7, wood_emmisive);
    }
}

#endif