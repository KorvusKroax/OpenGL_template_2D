#pragma once

#include <open_gl.h>
#include <canvas.h>

#include <iostream>
#include <chrono>

class Game
{
    private:
        void loop(OpenGL *openGL, Canvas *canvas);
        void processInput(OpenGL *openGL);
        void processDisplay(Canvas *canvas);

    public:
        int x, y;

        Game();
        ~Game();
};
