#pragma once

#include <cstdlib>
#include <color_rgba.h>
#include <vector2_int.h>

class Canvas
{
    public:
        unsigned int width;
        unsigned int height;
        int* pixels = nullptr;

        Canvas(unsigned int _width, unsigned int _height);
        ~Canvas();
        void fillCanvas();
        void fillCanvas(ColorRGBA color);
        void setPixel(Vector2Int p, ColorRGBA color);
        void setPixel(int x, int y, ColorRGBA color);
        void drawRectangle(int x, int y, int w, int h, ColorRGBA color);
        void drawLine(Vector2Int p1, Vector2Int p2, ColorRGBA color);
        void drawLine(int x1, int y1, int x2, int y2, ColorRGBA color);
        void drawLine_unsafe(int x1, int y1, int x2, int y2, ColorRGBA color);
        void drawCircle(Vector2Int p, int r, ColorRGBA color);
        void drawCircle(int x, int y, int r, ColorRGBA color);
};
