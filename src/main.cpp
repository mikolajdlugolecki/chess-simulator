#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main(void)
{
    GLFWwindow* window;
    if(!glfwInit()){
        std::cerr<<"Can't initalize GLWF."<<std::endl;
        return EXIT_FAILURE;
    }
    window = glfwCreateWindow(1280, 960, "OpenGL game", NULL, NULL);
    if(!window){
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    GLenum err;
    if((err = glewInit()) != GLEW_OK){
        std::cerr<<"Cant' initalize GLEW: "<<glewGetErrorString(err)<<std::endl;
        return EXIT_FAILURE;
    }
    while(!glfwWindowShouldClose(window)){
        glClearColor(0,0.8,1,1);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}

