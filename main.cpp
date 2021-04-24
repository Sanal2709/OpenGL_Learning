#include <GLFW/glfw3.h>

int main(){
  //Инициализация GLFW
  glfwInit();
  //Настройка GLFW
  //Задается минимальная требуемая версия OpenGL.
  //Мажорная
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  //Минорная
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //Установка профайла для которого создается контекст
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  //Выключение возможности изменения размера окна
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  return 0;
}