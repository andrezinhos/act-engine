#include "mkr.hpp"

bool mkr::createWindowContext(){
    glfwMakeContextCurrent(wmain.main);
    if (flags_active[0] == 1) glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("Error to Load OpenGL Context");
        glfwDestroyWindow(wmain.main);
        glfwTerminate();
        return false;
    }

    const byte* vendor = glGetString(GL_VENDOR);
    const byte* renderer = glGetString(GL_RENDERER);

    printf("[INFO] GPU: %s\n", renderer);
    printf("[INFO] VENDOR: %s\n", vendor);

    return true;
}

void mkr::setWindowPosition(int width, int height){
    if (flags_active[3] == 0){
        mkr::wmain.moni = glfwGetPrimaryMonitor();
        mkr::wmain.mode = glfwGetVideoMode(mkr::wmain.moni);

        int monitorX = wmain.mode->width;
        int monitorY = wmain.mode->height;
        printf("[INFO] MONITOR SIZE: %d | %d \n", monitorX, monitorY);
        
        int centerX = (monitorX/2) - (width/2);
        int centerY = (monitorY/2) - (height/2);
        glfwSetWindowPos(wmain.main, centerX, centerY);
    }
}