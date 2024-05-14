#include <open_gl.h>
#include <canvas.h>

#include <chrono>
#include <iostream>

Canvas *canvas;
Vector2Int pos;

void processInput();
void processDisplay();

int main()
{
    canvas = new Canvas(320, 200);
    openGL_initWindowed(canvas->width, canvas->height, canvas->pixels, 3);
    // openGL_initFullscreen(canvas->width, canvas->height, canvas->pixels, 3);
    // openGL_initFullscreen(&canvas);

    pos = Vector2Int(200, 50);

    while (!openGL_shouldClose())
    {
        processInput();
        processDisplay();

        openGL_update();

        // std::cout << "deltaTime: " << deltaTime << std::endl;
    }

    openGL_terminate();
    return 0;
}

void processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (pos.y < canvas->height - 1) pos.y++;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (pos.x < canvas->width - 1) pos.x++;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        if (pos.y > 0) pos.y--;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (pos.x > 0) pos.x--;
    }

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    mouseX = mouseX / pixelScale;
    mouseY = canvas->height - mouseY / pixelScale;

    pos = Vector2Int((int)mouseX, (int)mouseY);

    // std::cout << "x" << mouseX << ", y" << mouseY << std::endl;
}

void processDisplay()
{
    auto startTime = std::chrono::high_resolution_clock::now();

    canvas->fillCanvas(ColorRGBA(0, 50, 50, 255));
    // canvas->setPixel(pos, ColorRGBA(255, 255, 255, 255));
    canvas->drawCircle(pos, 20, ColorRGBA(0, 255, 255, 255));
    // canvas->drawLine(pos.x, pos.y, canvas->width * .5f, canvas->height * .5f, ColorRGBA(255, 255, 0, 255));

    auto finishTime = std::chrono::high_resolution_clock::now();

    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime);
    std::cout << microseconds.count() << "µs (" << (float)microseconds.count() / 1000 << "ms)" << std::endl;
}
