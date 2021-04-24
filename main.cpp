#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (600)

void esc_key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
  // close window by ESC
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

int main()
{
  //Init GLFW
  glfwInit();
  //Set up GLFW
  //Set minimal version required OpenGL.
  //major
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  //minor
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //Set profile for context
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //Turn of window resize
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, esc_key_callback);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return -1;
  }

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);

  glViewport(0, 0, width, height);

  GLclampf red = 0.2f;
  GLclampf green = 0.3f;
  GLclampf blue = 0.4f;

  while (!glfwWindowShouldClose(window))
  {
    //check events and call callbacks
    glfwPollEvents();

    //draw commands
    glClearColor(red, green, blue, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //swap buffers
    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}