#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

// Window dimensions
const GLuint WINDOW_WIDTH = (800);
const GLuint WINDOW_HEIGHT = (600);

// Shaders
const GLchar *vertexShaderSource = "#version 330 core\n"
                                   "layout (location = 0) in vec3 position;\n"
                                   "void main()\n"
                                   "{\n"
                                   "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
                                   "}\0";
const GLchar *fragmentShaderSource = "#version 330 core\n"
                                     "out vec4 color;\n"
                                     "void main()\n"
                                     "{\n"
                                     "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                     "}\n\0";

void esc_key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
  // close window by ESC
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

void interpolate_colors(GLclampf &r, GLclampf &g, GLclampf &b)
{
  //it's kind we walk around color interpolated triangle
  //all distance == 300
  //step == 1

  const int all_distance = 300;
  const int edge_distance = all_distance / 3;
  static int curr_pos = -1;
  if (curr_pos++ >= all_distance - 1)
  {
    curr_pos = 0;
  }

  GLclampf left_border_r = 0.0f;
  GLclampf left_border_g = 0.0f;
  GLclampf left_border_b = 0.0f;
  GLclampf right_border_r = 0.0f;
  GLclampf right_border_g = 0.0f;
  GLclampf right_border_b = 0.0f;

  if (curr_pos < edge_distance)
  {
    left_border_r = 1.0f;
    right_border_g = 1.0f;
  }
  else if (curr_pos < edge_distance * 2)
  {
    left_border_g = 1.0f;
    right_border_b = 1.0f;
  }
  else
  {
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

  // Build and compile our shader program
  // Vertex shader
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // Check for compile time errors
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  // Fragment shader
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  // Check for compile time errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }
  // Link shaders
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // Check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Set up vertex data (and buffer(s)) and attribute pointers
  GLfloat vertices[] = {
      -0.9f, -0.5f, 0.0f, // Left
      -0.3f, -0.5f, 0.0f,  // Right
      -0.3f, 0.5f, 0.0f    // Top
  };

  GLfloat vertices2[] = {
      0.9f, 0.9f, 0.0f,   // Верхний правый угол
      0.9f, -0.9f, 0.0f,  // Нижний правый угол
      0.5f, -0.9f, 0.0f, // Нижний левый угол
      0.5f, 0.9f, 0.0f   // Верхний левый угол
  };
  GLuint indices[] = {
      // Помните, что мы начинаем с 0!
      0, 1, 3, // Первый треугольник
      1, 2, 3  // Второй треугольник
  };
  GLuint VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

  glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

  GLuint EBO;
  glGenBuffers(1, &EBO);
  GLuint VBO2, VAO2;
  glGenVertexArrays(1, &VAO2);
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // 1. Привязываем VAO
  glBindVertexArray(VAO2);
  // 2. Копируем наши вершины в буфер для OpenGL
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
  // 3. Копируем наши индексы в в буфер для OpenGL
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  // 3. Устанавливаем указатели на вершинные атрибуты
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  // 4. Отвязываем VAO (НЕ EBO)
  glBindVertexArray(0);


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

    // Draw our first triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(VAO2);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);


    //swap buffers
    glfwSwapBuffers(window);
  }

  // Properly de-allocate all resources once they've outlived their purpose
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glfwTerminate();

  return 0;
}