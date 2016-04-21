#include "OBJObject.h"
#include "Window.h"

OBJObject::OBJObject(const char *filepath) 
{
	toWorld = glm::mat4(1.0f);
    parse(filepath);
    
    //populating the buffer
    // Create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glGenBuffers(1,&normalbuffer); //mine
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(GLfloat),(GLvoid*)0);

    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer); //mine
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals[0]), &normals[0], GL_STATIC_DRAW); //mine
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 3*sizeof(glm::vec3), (GLvoid*)0); //mine
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]) , &indices[0], GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    
}

void OBJObject::parse(const char *filepath) 
{
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
    FILE* fp;     // file pointer
    float x,y,z;  // vertex coordinates
    float r,g,b;  // vertex color
    int c1,c2;    // characters read from file
   // int matches;

    fp = fopen(filepath,"rb");  // make the file name configurable so you can load other files
    
    if (fp == NULL) { cerr << "error loading file" << endl; exit(-1); }  // just in case the file can't be found or is corrupt

    while(!feof(fp)){
        
        c1 = fgetc(fp);
        
        if (c1 != 'v' && c1!='f')
            continue;
        
        c2 = fgetc(fp);

        if ((c1=='v') && (c2==' '))
        {
            glm::vec3 vertex;
            glm::vec3 temp_color;

          fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);

            vertex.x = x;
            vertex.y = y;
            vertex.z = z;
            vertices.push_back(vertex);
            
            temp_color.x = glm::normalize(r);
            temp_color.y = glm::normalize(g);
            temp_color.z = glm::normalize(b);
            colors.push_back((temp_color));
            
        }
        else if((c1 == 'v') && (c2=='n')) {
            //cerr<<"normal"<<endl;
            glm::vec3 normal;

            fscanf(fp, "%f %f %f ", &x, &y, &z);
            normal.x = x;
            normal.y = y;
            normal.z = z;
          
            normals.push_back(normal);
          //  cerr<<"display normal.x "<<&normals[100]<<endl;
            
        }
        else if ((c1 == 'f') && (c2 == ' ')){

            unsigned int vertexIndex[3], normalIndex[3];
         
            fscanf(fp, "%u//%u %u//%u %u//%u", &vertexIndex[0],  &normalIndex[0], &vertexIndex[1],  &normalIndex[1],  &vertexIndex[2],  &normalIndex[2] );

            indices.push_back(vertexIndex[0]-1);
            indices.push_back(vertexIndex[1]-1);
            indices.push_back(vertexIndex[2]-1);
        }
        else{

            
        }

    }
    fclose(fp);   // make sure you don't forget to close the file when done
   printf("Finished parsing %s\n", filepath);
}

//meshing data
void OBJObject::draw(GLuint shaderProgram)
{
    //printf("starting the drawing func.\n");
    // Calculate combination of the model (toWorld), view (camera inverse), and perspective matrices
    glm::mat4 MVP = Window::P * Window::V * toWorld;

    // removed from the language. The user is expected to supply this matrix to the shader when using modern OpenGL.
    GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    
    /* WHERE MY CODE STARTS*/
    glm::mat4 camera = Window::V;
    GLuint cameraID = glGetUniformLocation(shaderProgram, "camera");
    glUniformMatrix4fv(cameraID, 1, GL_FALSE, &camera[0][0]);
    
    glm::mat4 model = toWorld;
    GLuint modelID = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
    
    glm::mat4 projection = Window::P;
    GLuint projectionID = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projection[0][0]);
    
    //
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    GLuint lightAttrID = glGetUniformLocation(shaderProgram, "light.position");
    glUniform3fv(lightAttrID, 1, &position[0]);
    
    intensities = glm::vec3(1.0f, 1.0f, 1.0f);
    lightAttrID = glGetUniformLocation(shaderProgram, "light.intensities");
    glUniform3fv(lightAttrID, 1, &intensities[0]);
    
    bool on = true;
    lightAttrID = glGetUniformLocation(shaderProgram, "light.on");
    glUniform1i(lightAttrID, on);
    
    //materials
    //amb
    //diff
    //spec
    //f shiny
    
    /*********** END *********/
    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (GLint) indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
}

void OBJObject:: update(){

     //spin(0.01f);
}

void OBJObject::spin(float deg)
{
    this->angle += deg;
    if (this->angle > 360.0f || this->angle < -360.0f) this->angle = 0.0f;
    
   toWorld = toWorld * glm::rotate(glm::mat4(1.0f), deg, glm::vec3(0.0f, 1.0f, 0.0f));
}



