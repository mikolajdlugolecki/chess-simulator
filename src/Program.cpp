#include "Program.h"

void Program::init(GLFWwindow *window)
{
    initShaders();
	glClearColor(0, 1, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetKeyCallback(window, keyCallback);
}

void Program::free(GLFWwindow *window)
{
    freeShaders();
}

void Program::errorCallback(int error, const char *description)
{
    fputs(description, stderr);
}

void Program::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // if(action == GLFW_PRESS){
	// 	if (key == GLFW_KEY_LEFT) 
    //         speed = -PI;
	// 	if (key == GLFW_KEY_RIGHT) 
    //         speed = PI;
	// }
	// if (action == GLFW_RELEASE) {
	// 	if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) 
    //         speed = 0;	
	// }
}

void Program::drawScene(GLFWwindow *window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 1.0f, 50.0f);
	glm::mat4 V = glm::lookAt(glm::vec3(0.0f, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	spLambert->use();
	glUniformMatrix4fv(spLambert->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambert->u("V"), 1, false, glm::value_ptr(V)); 
	glfwSwapBuffers(window);
}

void Program::run(void)
{
    GLFWwindow* window;
	glfwSetErrorCallback(errorCallback);
	if(!glfwInit()){
		fprintf(stderr, "Can't initialize GLFW.\n");
		exit(EXIT_FAILURE);
	}
	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);
	if (!window){
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	GLenum err;
	if((err = glewInit()) != GLEW_OK){
		fprintf(stderr, "Can't initialize GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}
	init(window);
	glfwSetTime(0);
	while (!glfwWindowShouldClose(window)){
		glfwSetTime(0);
		drawScene(window);
		glfwPollEvents();
	}
	free(window);
	glfwDestroyWindow(window);
	glfwTerminate();
}
