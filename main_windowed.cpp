// #include <open_gl.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.h>

#include <canvas.h>
#include <dynamic_array.h>

#include <chrono>
#include <iostream>

const unsigned int CANVAS_WIDTH = 320;
const unsigned int CANVAS_HEIGHT = 200;
const float PIXEL_SCALE = 3;



Canvas *canvas;
Vector2Int pos;
DynamicArray dots;

bool mousePressed = false;



void processInput(GLFWwindow *window);
void processDisplay();

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE); // frame on/off

    canvas = new Canvas(CANVAS_WIDTH, CANVAS_HEIGHT);

    GLFWwindow *window = glfwCreateWindow(canvas->width * PIXEL_SCALE, canvas->height * PIXEL_SCALE, "OpenGL 2D window", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    unsigned int quadVAO;
    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);

    unsigned int quadVBO;
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

    float quadVertices[] = {
        // positions    // texCoords
        -1.0f,  1.0f,   0.0f, 1.0f,     // left top
        -1.0f, -1.0f,   0.0f, 0.0f,     // left bottom
        1.0f, -1.0f,   1.0f, 0.0f,     // right bottom

        -1.0f,  1.0f,   0.0f, 1.0f,     // left top
        1.0f, -1.0f,   1.0f, 0.0f,     // right bottom
        1.0f,  1.0f,   1.0f, 1.0f      // right top
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, canvas->width, canvas->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, canvas->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    unsigned int renderbuffer;
    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, canvas->width, canvas->height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Shader quadShader = Shader("shaders/canvas.vert", "shaders/canvas.frag");

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        processDisplay();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);

        quadShader.use();
        glBindVertexArray(quadVAO);
        glDisable(GL_DEPTH_TEST);

        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, canvas->width, canvas->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, canvas->pixels);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // currentTime = static_cast<float>(glfwGetTime());
        // deltaTime = currentTime - lastTime;
        // lastTime = currentTime;
        // std::cout << "deltaTime: " << deltaTime << std::endl;
    }

    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteRenderbuffers(1, &renderbuffer);

    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }


    // // keyboard
    // if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) pos.y++;
    // if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) pos.x++;
    // if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) pos.y--;
    // if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) pos.x--;


    // mouse
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    pos = Vector2Int(
        int(mouseX / PIXEL_SCALE),
        int(canvas->height - mouseY / PIXEL_SCALE)
    );

    if (!mousePressed) {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            dots.add(pos.x);
            dots.add(pos.y);
            mousePressed = true;
        }
    } else {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
            mousePressed = false;
        }
    }

    std::cout << dots.getSize() << std::endl;
}

void processDisplay()
{
    canvas->clearCanvas();
    // canvas->fillCanvas(ColorRGBA(0, 50, 50, 255));
    canvas->setPixel(pos, ColorRGBA(255, 255, 255, 255));
    canvas->drawCircle(pos, 20, ColorRGBA(0, 255, 255, 255));
    canvas->drawLine(pos.x, pos.y, canvas->width * .5f, canvas->height * .5f, ColorRGBA(255, 255, 0, 255));
    for (unsigned int i = 0; i < dots.getSize(); i += 2) {
        int x = dots.getValue(i);
        int y = dots.getValue(i + 1);
        canvas->setPixel(x, y, ColorRGBA(200, 150, 100, 255));
    }
}

// void _processDisplay()
// {
//     auto startTime = std::chrono::high_resolution_clock::now();
//     processDisplay();
//     auto finishTime = std::chrono::high_resolution_clock::now();
//     auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime);
//     std::cout << microseconds.count() << "µs (" << (float)microseconds.count() / 1000 << "ms)" << std::endl;
// }
