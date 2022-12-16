#include <cstdlib>
#include <cstdio>
#include "../vmlib/vec3.hpp"
#include "../vmlib/mat44.hpp"


//-----------CONCRETE--------------
float concrete_ambient[] ={ 0.05f,0.05f,0.05f,1.0f };
float concrete_diffuse[] ={ 0.5f,0.5f,0.5f,1.0f};
float concrete_specular[] ={ 0.7f,0.7f,0.7f,1.0f};
float concrete_shine = 10.0f;
//---------------------------------

//-----------shinyMetal--------------
float shinyMetal_ambient[] = {0.25f, 0.25f, 0.25f };
float shinyMetal_diffuse[] = {0.4f, 0.4f, 0.4f };
float shinyMetal_specular[] = {0.774597f, 0.774597f, 0.774597f};
float shinyMetal_shine = 76.8f;
//---------------------------------

//-----------yellow_rubber--------------
float yellow_rubber_ambient[] ={ 0.05f,0.05f,0.0f,1.0f };
float yellow_rubber_diffuse[] ={0.5f,0.5f,0.4f,1.0f};
float yellow_rubber_specular[] ={0.7f,0.7f,0.04f,1.0f };
float yellow_rubber_shine = 10.0f;
//-----------------------------------

void setMaterialProperties(std::string material){
    if (material == "concrete"){
        glUniform3fv(15,1, concrete_ambient);
        glUniform3fv(16,1, concrete_diffuse);
        glUniform3fv(17,1, concrete_specular);
        glUniform1f(18, concrete_shine);
    }
    else if (material == "shinyMetal"){
        glUniform3fv(15,1, shinyMetal_ambient);
        glUniform3fv(16,1, shinyMetal_diffuse);
        glUniform3fv(17,1, shinyMetal_specular);
        glUniform1f(18, shinyMetal_shine);
    }
    else if (material == "yellowRubber"){
        glUniform3fv(15,1, yellow_rubber_ambient);
        glUniform3fv(16,1, yellow_rubber_diffuse);
        glUniform3fv(17,1, yellow_rubber_specular);
        glUniform1f(18, yellow_rubber_shine);
    }
}