#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

using namespace std;

class OBJObject
{
private:
std::vector<unsigned int> indices; //face index
    std::vector <unsigned int >normal_indices; //normal index

    //mine
std::vector<glm::vec3> colors; //rgb store
    float angle;
    float size;
  //  float r,g,b;

public:
    glm::mat4 toWorld;
	OBJObject(const char* filepath);
    
    std::vector<glm::vec3> vertices; //vert
    std::vector<glm::vec3> normals;  //normals

	void parse(const char* filepath);
	void draw(GLuint shaderprogram);
    void spin(float);
    void update();
    
    //mine
    void translation(float x, float y , float z );
    void rotate(float d);
    void Drawable(const char * filepath, OBJObject *& object);
    GLuint VBO, VAO, EBO;
    GLuint normalbuffer;
    
    //lighting
    glm::vec3 position;
    glm::vec3 intensities;
    bool directional = true;
    bool point = false;
    bool spotlight = false;
    
    //Drawables
    void DrawBunny (OBJObject *& object);
    void DrawDragon (OBJObject *& object);
    void DrawBear (OBJObject *& object);
};

#endif