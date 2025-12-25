#ifndef GOLAPP_H
#define GOLAPP_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class GolApp
{
public:
    GolApp();
    ~GolApp();

    void run();

protected:
    void init();
    void cleanup();

    GLFWwindow* m_window;
};

#endif // GOLAPP_H
