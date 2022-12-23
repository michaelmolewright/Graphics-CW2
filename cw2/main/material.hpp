#ifndef MATERIALS_HEADER_GUARD
#define MATERIALS_HEADER_GUARD

#include <cstdlib>
#include <cstdio>
#include "../vmlib/vec3.hpp"
#include "../vmlib/mat44.hpp"


//-----------CONCRETE--------------
float concrete_ambient[] ={ 0.05f,0.05f,0.05f,1.0f };
float concrete_diffuse[] ={ 0.5f,0.5f,0.5f,1.0f};
float concrete_specular[] ={ 0.4f,0.4f,0.4f,1.0f};
float concrete_shine = 2.0f;
float concrete_emmisive = 0.1f;
//---------------------------------

//-----------shinyMetal--------------
float shinyMetal_ambient[] = {0.23125f, 0.23125f, 0.23125f };
float shinyMetal_diffuse[] = {0.2775f, 0.2775f, 0.2775f };
float shinyMetal_specular[] = {0.774597f, 0.774597f, 0.774597f};
float shinyMetal_shine = 89.6f;
float shinyMetal_emmisive = 0.001f;
//---------------------------------

//-----------railMetal--------------
float railMetal_ambient[] = { 0.01f, 0.01f, 0.01f };
float railMetal_diffuse[] = { 0.01f, 0.01f, 0.01f };
float railMetal_specular[] = { 0.632741f, 0.628634f, 0.646435f };
float railMetal_shine = 100.f;
float railMetal_emmisive = 0.001f;

//-----------yellow_rubber--------------
float yellow_rubber_ambient[] ={ 0.05f,0.05f,0.0f,1.0f };
float yellow_rubber_diffuse[] ={0.5f,0.5f,0.4f,1.0f};
float yellow_rubber_specular[] ={0.7f,0.7f,0.04f,1.0f };
float yellow_rubber_shine = 10.0f;
float yellow_rubber_emmisive = 0.1f;
//-----------------------------------

//-----------lamp-----------------
float lamp_ambient[] = {0.1f, 0.1f, 0.1f };
float lamp_diffuse[] = {0.1f, 0.1f, 0.1f };
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

//-----------skateboard-----------------
float skateboard_ambient[] = { 0.f, 0.f, 0.f };
float skateboard_diffuse[] = { 0.698039f, 0.698039f, 0.698039f };
float skateboard_specular[] = { 0.34f, 0.34f, 0.34f };
float skateboard_shine = 2.f;
float skateboard_emmisive = 0.05f;
//---------------------------------

//-----------mainlyDif-----------------
float mainlyDif_ambient[] = { 0.2f, 0.2f, 0.2f };
float mainlyDif_diffuse[] = { 0.8f, 0.8f, 0.8f };
float mainlyDif_specular[] = { 0.1f, 0.1f, 0.1f };
float mainlyDif_shine = 50.f;
float mainlyDif_emmisive = 0.001f;
//---------------------------------

//-----------mainlySpec-----------------
float mainlySpec_ambient[] = { 0.2f, 0.2f, 0.2f };
float mainlySpec_diffuse[] = { 0.1f, 0.1f, 0.1f };
float mainlySpec_specular[] = { 0.8f, 0.8f, 0.8f };
float mainlySpec_shine = 50.f;
float mainlySpec_emmisive = 0.001f;
//---------------------------------

//-----------mainlyEmi-----------------
float mainlyEmi_ambient[] = { 0.2f, 0.2f, 0.2f };
float mainlyEmi_diffuse[] = { 0.1f, 0.1f, 0.1f };
float mainlyEmi_specular[] = { 0.1f, 0.1f, 0.1f };
float mainlyEmi_shine = 50.f;
float mainlyEmi_emmisive = 0.2f;
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
    else if (material == "skateboard"){
        glUniform3fv(3,1, skateboard_ambient);
        glUniform3fv(4,1, skateboard_diffuse);
        glUniform3fv(5,1, skateboard_specular);
        glUniform1f(6, skateboard_shine);
        glUniform1f(7, skateboard_emmisive);
    }
    else if (material == "railMetal"){
        glUniform3fv(3,1, railMetal_ambient);
        glUniform3fv(4,1, railMetal_diffuse);
        glUniform3fv(5,1, railMetal_specular);
        glUniform1f(6, railMetal_shine);
        glUniform1f(7, railMetal_emmisive);
    }
    else if (material == "mainlyDif"){
        glUniform3fv(3,1, mainlyDif_ambient);
        glUniform3fv(4,1, mainlyDif_diffuse);
        glUniform3fv(5,1, mainlyDif_specular);
        glUniform1f(6, mainlyDif_shine);
        glUniform1f(7, mainlyDif_emmisive);
    }
    else if (material == "mainlySpec"){
        glUniform3fv(3,1, mainlySpec_ambient);
        glUniform3fv(4,1, mainlySpec_diffuse);
        glUniform3fv(5,1, mainlySpec_specular);
        glUniform1f(6, mainlySpec_shine);
        glUniform1f(7, mainlySpec_emmisive);
    }
    else if (material == "mainlyEmi"){
        glUniform3fv(3,1, mainlyEmi_ambient);
        glUniform3fv(4,1, mainlyEmi_diffuse);
        glUniform3fv(5,1, mainlyEmi_specular);
        glUniform1f(6, mainlyEmi_shine);
        glUniform1f(7, mainlyEmi_emmisive);
    }
}

#endif