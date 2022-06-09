#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "dependeces/Common/Initialize.h"
#include <glm/glm.hpp>

using namespace glm;
GLFWwindow *window;
#include "dependeces/Common/objloader.hpp"
#include "dependeces/Common/shader.hpp"

#include "src/Rain.h"
#include "src/Smoke.h"
#include "src/Fog.h"


int main() {
    Rain app;
    app.setUp();
    do {
        app.update();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    app.clean();
    return 0;
}
