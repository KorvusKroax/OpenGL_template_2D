#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>

class OpenGL
{
    private:
        unsigned int quadVAO;
        unsigned int quadVBO;
        unsigned int framebuffer;
        unsigned int textureColorbuffer;
        unsigned int renderbuffer;
        Shader *quadShader;

        int init();
        void terminate();

    public:
        unsigned int width;
        unsigned int height;
        float pixelScale;
        int *pixels = nullptr;
        const char *title;
        GLFWwindow *window;

        OpenGL(unsigned int width, unsigned int height, float pixelScale, int *pixels, const char *title = "OpenGL - 2D window");
        ~OpenGL();
        void update();
        bool shouldClose();
};
