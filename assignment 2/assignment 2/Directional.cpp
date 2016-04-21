#include "Directional.h"

void Directional::bind(GLuint shaderProgram)
{
    GLuint lightAttrID = glGetUniformLocation(shaderProgram,
                                              "dirLight.on");
    glUniform1i(lightAttrID, on);
    lightAttrID = glGetUniformLocation(shaderProgram,
                                       "dirLight.dir");
    glUniform3fv(lightAttrID, 1, &direction[0]);
}