#include <open_gl.h>
#include <canvas.h>

#include <iostream>

const unsigned int CANVAS_WIDTH = 320;
const unsigned int CANVAS_HEIGHT = 200;
const float PIXEL_SCALE = 3;

Canvas* canvas;
Vector2Int pos;

void processInput();
void processDisplay();

int main()
{
    canvas = new Canvas(CANVAS_WIDTH, CANVAS_HEIGHT);
    openGL_initWindowed(canvas->width, canvas->height, canvas->pixels, PIXEL_SCALE);
    // openGL_initFullscreen(canvas->width, canvas->height, canvas->pixels, PIXEL_SCALE);

    // openGL_initFullscreen(&canvas);

    pos = Vector2Int(200, 50);

    while (!openGL_shouldClose())
    {
        processInput();
        processDisplay();

        openGL_update();

        std::cout << deltaTime << std::endl;
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
}

void processDisplay()
{
    canvas->fillCanvas();
    canvas->setPixel(pos, ColorRGBA(255, 255, 255, 255));
    canvas->drawCircle(pos, 20, ColorRGBA(0, 255, 255, 255));
    canvas->drawLine(pos.x, pos.y, canvas->width * .5f, canvas->height * .5f, ColorRGBA(255, 255, 0, 255));
}
