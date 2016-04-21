#include "window.h"
#include "OBJObject.h"

const char* window_title = "GLFW Starter Project";
Cube * cube;

GLint shaderProgram;
OBJObject * object;

//others
OBJObject * bunny;
OBJObject * dragon;
OBJObject * bear;

bool obj_rot = false;
bool obj_trans = false;
bool obj_zoom = false;

double Window::cur_x;
double Window::cur_y;
double Window::prev_x;
double Window::prev_y;
double Window::scrollX;
double Window::scrollY;

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 20.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

void Window::initialize_objects()
{
	//cube = new Cube();
    object = new OBJObject("/Users/Cocona2121/Desktop/CSE 167_1/Assignment 2/assignment 2/assignment 2/dragon.obj");
    
    bunny = new OBJObject("/Users/Cocona2121/Desktop/CSE 167_1/Assignment 2/assignment 2/assignment 2/bunny.obj");
    dragon = new OBJObject("/Users/Cocona2121/Desktop/CSE 167_1/Assignment 2/assignment 2/assignment 2/dragon.obj");
    bear = new OBJObject ("/Users/Cocona2121/Desktop/CSE 167_1/Assignment 2/assignment 2/assignment 2/bear.obj");
    
	// Load the shader program. Similar to the .obj objects, different platforms expect a different directory for files
#ifdef _WIN32 // Windows (both 32 and 64 bit versions)
	shaderProgram = LoadShaders("../shader.vert", "../shader.frag");
#else // Not windows
	shaderProgram = LoadShaders("shader.vert", "shader.frag");
#endif
}

void Window::clean_up()
{
	delete(cube);
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
    printf("Window::create_window()\n");
    
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    
	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}
//FIXME:Must be adjusted
void Window::idle_callback()
{
    if(obj_rot && (cur_x != prev_x && cur_y != prev_y)){
        //        cerr<<"able to rot"<<endl;
        
        glm::vec3 mousePos = mouseMovement(cur_x, cur_y);
        glm::vec3 va = mouseMovement(prev_x, prev_y);
        
        //computing rotation
        float angle = glm::acos(glm::min(1.0f, glm::dot(va,mousePos)));
        
        glm::vec3 rotAxis = glm::cross( va, mousePos );
        //        printf("va: %f %f %f\n", va.x, va.y, va.z);
        //        printf("mousePos: %f %f %f\n", mousePos.x, mousePos.y, mousePos.z);
        //    printf("rotAxis: %f %f %f\n", rotAxis.x, rotAxis.y, rotAxis.z);
        
        //        glm::mat3 C_inverse = glm::inverse(glm::mat3(V) * glm::mat3(object->toWorld));
        //        glm::vec3 rotationObj = C_inverse * rotAxis;
        
        object -> toWorld  = glm::rotate(glm::mat4(1.0f), glm::degrees(angle), rotAxis) * object->toWorld;
        
        prev_x = cur_x;
        prev_y = cur_y;
        
    }
    else if (obj_trans && (cur_x != prev_x && cur_y != prev_y)){
        
       // float cameraX, cameraY = 0.0f;
        //delta values
        float deltaX = cur_x - prev_x;
        float deltaY = prev_y - cur_y;
        
        deltaX /= glm::max(1.0f, glm::abs(object->toWorld[3][2]) + cam_pos.z);
        deltaY /= glm::max(1.0f, glm::abs(object->toWorld[3][2]) + cam_pos.z);
        glm::vec3 delta (deltaX, deltaY, 0.0f);
         
        object->toWorld = glm::translate(glm::mat4(1.0f), delta) * object->toWorld;
    
        prev_x = cur_x;
        prev_y = cur_y;
        
    }

}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);
	
	// Render the cube
   // cube->draw(shaderProgram);
    object->draw(shaderProgram);
    
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
        else if (key == GLFW_KEY_F1){
            //switch model
            object = bunny;
            //scale to same size
            object->toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 2.0f,2.0f));
            //move to center
           // object->toWorld = glm::translate(glm::mat4(1.0f), cam_pos);
        }
        else if (key == GLFW_KEY_F2){
            //switch model
            object = dragon;
            //scale to same size
            //move to center
        }
        else if (key == GLFW_KEY_F3){
            //switch model
            object = bear;
            object-> toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.25f));
            //move to center
        }
        else if (key == GLFW_KEY_R){
        
            object->toWorld = glm::mat4(1.0f);
        }
        else if (key == GLFW_KEY_S && mods == GLFW_MOD_SHIFT){
            object-> toWorld = object->toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
        }
        else if (key == GLFW_KEY_S && mods != GLFW_MOD_SHIFT){
            object-> toWorld = object->toWorld * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 1.5f, 1.5f));
        }
        else if (key == GLFW_KEY_1){
        
            //directional
            
        }
        else if (key == GLFW_KEY_2){
            //point
        }
        else if (key == GLFW_KEY_3){
            //spot
        
        }
        
	}
}

//MY FUNCTIONS
void Window::cursor_callback(GLFWwindow* window, double x, double y){
 
    //record prev points
    cur_x = 2.0 * x;
    cur_y = 2.0 * y;
}


void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods){

    if(action == GLFW_PRESS){
        
        if(button == GLFW_MOUSE_BUTTON_LEFT && mods == GLFW_MOD_SHIFT){
            obj_trans = true;
        }
        else if (button == GLFW_MOUSE_BUTTON_LEFT && mods != GLFW_MOD_SHIFT){
            obj_rot = true;
        }
    }
    else{
        obj_rot = false;
        obj_trans = false;
    }
}

//FIXME::center to be adjusted. Also adjust speed
void Window::scroll_callback(GLFWwindow* window, double xoffset , double yoffset){

    scrollY = - yoffset;
    
    glm::vec3 scroll(0.0f, 0.0f, scrollY);
    object ->toWorld = object->toWorld * glm::translate(glm::mat4(1.0f), scroll);

}


glm::vec3 Window::mouseMovement( int x, int y){

//    cerr<<"MM x: "<<x<<endl;
//     cerr<<"MM y: "<<y<<endl;
    glm::vec3 vector; // Vector v is the synthesized 3D position of the mouse location on the trackball
    
    double d;     // this is the depth of the mouse location: the delta between the plane through the center of the trackball and the z position of the mouse
    

    vector.x = (2.0f * x - Window::width) / Window::width;   // this calculates the mouse X position in trackball coordinates, which range from -1 to +1
    vector.y = (Window::height - 2.0f * y) / Window::height;   // this does the equivalent to the above for the mouse Y position
    
    vector.z = 0.0f;   // initially the mouse z position is set to zero, but this will change below
    
//    //checking
//    cerr<<"vector.x: "<<vector.x<<endl;
//    cerr<<"vector.y: "<<vector.y<<endl;
//    cerr<<"vector.z: "<<vector.z<<endl;
    
    d = glm::length(vector);    // this is the distance from the trackball's origin to the mouse location, without considering depth (=in the plane of the trackball's origin)
    
   // d = glm::min(1.0f, (float)glm::length(vector));
    
    d= (d<1.0f) ? d : 1.0f;   // this limits d to values of 1.0 or less to avoid square roots of negative values in the following line
    
    vector.z = glm::sqrt(1.0f - glm::pow(d, 2.0f));  // this calculates the Z coordinate of the mouse position on the trackball, based on Pythagoras: v.z*v.z + d*d = 1*1
    
    return glm::normalize(vector); // Still need to normalize, since we only capped d, not v.
    
}







