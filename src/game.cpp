#include <game.h>

Game::Game()
{
    Canvas canvas = Canvas(320, 200);
    OpenGL openGL = OpenGL(canvas.width, canvas.height, 4, canvas.pixels);

    while (!openGL.shouldClose())
    {
        loop(&openGL, &canvas);
        openGL.update();
    }
}

Game::~Game() { }

void Game::loop(OpenGL *openGL, Canvas *canvas)
{
    processInput(openGL);
    processDisplay(canvas);
}

void Game::processInput(OpenGL *openGL)
{
    if (glfwGetKey(openGL->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(openGL->window, true);

    // if (glfwGetKey(openGL->window, GLFW_KEY_UP) == GLFW_PRESS)  y++;
    // if (glfwGetKey(openGL->window, GLFW_KEY_RIGHT) == GLFW_PRESS) x++;
    // if (glfwGetKey(openGL->window, GLFW_KEY_DOWN) == GLFW_PRESS) y--;
    // if (glfwGetKey(openGL->window, GLFW_KEY_LEFT) == GLFW_PRESS) x--;

    double mouseX, mouseY;
    glfwGetCursorPos(openGL->window, &mouseX, &mouseY);

    x = int(mouseX / openGL->pixelScale);
    y = int(openGL->height - mouseY / openGL->pixelScale);
}

void Game::processDisplay(Canvas *canvas)
{
    // auto startTime = std::chrono::high_resolution_clock::now();

    canvas->clearCanvas();

    canvas->drawCircle(canvas->width >> 1, canvas->height >> 1, 20, ColorRGBA(0, 255, 255, 255));
    // canvas->floodFill(canvas->width >> 1, canvas->height >> 1, ColorRGBA(0, 100, 100, 255));
    canvas->spanFill(canvas->width >> 1, canvas->height >> 1, ColorRGBA(0, 100, 100, 255));

    canvas->drawCircle(x, y, 5, ColorRGBA(255, 0, 0, 255));

    // auto finishTime = std::chrono::high_resolution_clock::now();
    // auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finishTime - startTime);
    // std::cout << microseconds.count() << "µs (" << (float)microseconds.count() / 1000 << "ms)" << std::endl;
}
