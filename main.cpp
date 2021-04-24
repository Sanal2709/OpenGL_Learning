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

void interpolate_colors(GLclampf& r, GLclampf& g, GLclampf& b){
  //it's kind we walk around color interpolated triangle
  //all distance == 300
  //step == 1

  const int all_distance = 300;
  const int edge_distance = all_distance/3;
  static int curr_pos = -1;
  if(curr_pos++ >= all_distance -1){
    curr_pos = 0;
  }

  GLclampf left_border_r = 0.0f;
  GLclampf left_border_g = 0.0f;
  GLclampf left_border_b = 0.0f;
  GLclampf right_border_r = 0.0f;
  GLclampf right_border_g = 0.0f;
  GLclampf right_border_b = 0.0f;
  
  if(curr_pos < edge_distance){
    left_border_r = 1.0f;
    right_border_g = 1.0f;
  }
  else if(curr_pos < edge_distance*2){
    left_border_g = 1.0f;
    right_border_b = 1.0f;
  }
  else {
    left_border_b = 1.0f;
    right_border_r = 1.0f;
  }

  int edge_position = curr_pos % edge_distance;

  float coeff = 1 - (float)edge_position / edge_distance;
  r = coeff * left_border_r + (1 - coeff) * right_border_r;
  g = coeff * left_border_g + (1 - coeff) * right_border_g;
  b = coeff * left_border_b + (1 - coeff) * right_border_b;
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

  GLclampf red = 0.0f;
  GLclampf green = 0.0f;
  GLclampf blue = 0.0f;

  while (!glfwWindowShouldClose(window))
  {
    //check events and call callbacks
    glfwPollEvents();

    //draw commands
    interpolate_colors(red, green, blue);
    glClearColor(red, green, blue, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //swap buffers
    glfwSwapBuffers(window);
  }

  glfwTerminate();

  return 0;
}