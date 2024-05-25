#include <canvas.h>

Canvas::Canvas() { }

Canvas::Canvas(unsigned int width, unsigned int height)
{
    this->width = width;
    this->height = height;
    this->pixels = new int[this->width * this->height];
}

Canvas::~Canvas()
{
    delete[] pixels;
}

void Canvas::clearCanvas()
{
    memset(pixels, 0, width * height * sizeof(int));
}

void Canvas::fillCanvas(ColorRGBA color = ColorRGBA())
{
    for (int i = 0; i < width * height; i++) {
        pixels[i] = color.value;
    }
}

void Canvas::setPixel(Vector2Int p, ColorRGBA color) { setPixel(p.x, p.y, color); }

void Canvas::setPixel(int x, int y, ColorRGBA color)
{
    if (x >= 0 && x < width && y >= 0 && y < height) {
        pixels[x + y * width] = color.value;
    }
}

unsigned int Canvas::getPixel(Vector2Int p) { return getPixel(p.x, p.y); }

unsigned int Canvas::getPixel(int x, int y)
{
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return pixels[x + y * width];
    }

    return -1;
}

void Canvas::drawRectangle(int x, int y, int w, int h, ColorRGBA color)
{
    for (int i = x; i <= x + w; i++) {
        setPixel(x + i, y, color);
        setPixel(x + i, y + h, color);
    }
    for (int i = y; i <= y + h; i++) {
        setPixel(x, y + i, color);
        setPixel(x + w, y + i, color);
    }
}

void Canvas::drawLine(Vector2Int p1, Vector2Int p2, ColorRGBA color) { drawLine(p1.x, p1.y, p2.x, p2.y, color); }

void Canvas::drawLine(int x1, int y1, int x2, int y2, ColorRGBA color)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;

    int pos = x1 + y1 * width;

    if (dx > dy) {
        int y = y1;
        int d = dy * 2 - dx;
        for (int x = x1; x != x2; x += sx) {
            setPixel(x, y, color);
            if (d > 0) {
                y += sy;
                d -= dx * 2;
            }
            d += dy * 2;
        }
    } else {
        int x = x1;
        int d = dx * 2 - dy;
        for (int y = y1; y != y2; y += sy) {
            setPixel(x, y, color);
            if (d > 0) {
                x += sx;
                d -= dy * 2;
            }
            d += dx * 2;
        }
    }
}

void Canvas::drawLine_unsafe(int x1, int y1, int x2, int y2, ColorRGBA color)
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? width : -width;

    if (dx > dy) {
        int y = y1;
        int d = dy * 2 - dx;
        for (int pos = x1 + y1 * width; pos != x2 + y2 * width; pos += sx) {
            pixels[pos] = color.value;
            if (d > 0) {
                pos += sy;
                d -= dx * 2;
            }
            d += dy * 2;
        }
    } else {
        int x = x1;
        int d = dx * 2 - dy;
        for (int pos = x1 + y1 * width; pos != x2 + y2 * width; pos += sy) {
            pixels[pos] = color.value;
            if (d > 0) {
                pos += sx;
                d -= dy * 2;
            }
            d += dx * 2;
        }
    }
}

void Canvas::drawCircle(Vector2Int p, int r, ColorRGBA color) { drawCircle(p.x, p.y, r, color); }

void Canvas::drawCircle(int x, int y, int r, ColorRGBA color)
{
    int xx = -r;
    int yy = 0;
    int rr = 2 - 2 * r;
    do {
        setPixel(x - xx, y + yy, color);
        setPixel(x - yy, y - xx, color);
        setPixel(x + xx, y - yy, color);
        setPixel(x + yy, y + xx, color);
        r = rr;
        if (r <= yy) rr += ++yy * 2 + 1;
        if (r > xx || rr > yy) rr += ++xx * 2 + 1;
    } while (xx < 0);
}

void Canvas::floodFill(int x, int y, ColorRGBA color)
{
    unsigned int targetColor = getPixel(x, y);
    if (targetColor == color.value) return;

    Vector2Int *dir = new Vector2Int[4] { Vector2Int(0, 1), Vector2Int(1, -1), Vector2Int(-1, -1), Vector2Int(-1, 1) };
    int *next = new int[width * height * 2];
    int index = 0;

    while (true) {
        setPixel(x, y, color);
        for (int i = 0; i < 4; i++) {
            x += dir[i].x;
            y += dir[i].y;
            if (getPixel(x, y) == targetColor) {
                next[index] = x;
                next[index + 1] = y;
                index += 2;
            }
        }
        if (index == 0) break;
        index -= 2;
        x = next[index];
        y = next[index + 1];
    }

    delete[] next;
    delete[] dir;
}

void Canvas::spanFill(int x, int y, ColorRGBA color)
{
    unsigned int targetColor = getPixel(x, y);
    if (targetColor == color.value) return;

    int *next = new int[width * height * 2];
    int index = 0;

    next[index] = x;
    next[index + 1] = x;
    next[index + 2] = y;
    next[index + 3] = 1;
    index += 4;

    int x1 = x;
    int x2 = x;
        y  = y - 1;
    int dy = -1;

    while (true) {
        x = x1;
        if (getPixel(x, y) == targetColor) {
            while (getPixel(x - 1, y) == targetColor) {
                setPixel(x - 1, y, color);
                x--;
            }
            if (x < x1) {
                next[index] = x;
                next[index + 1] = x1 - 1;
                next[index + 2] = y - dy;
                next[index + 3] = -dy;
                index += 4;
            }
        }

        while (x1 <= x2) {
            while (getPixel(x1, y) == targetColor) {
                setPixel(x1, y, color);
                x1++;
            }
            if (x1 > x) {
                next[index] = x;
                next[index + 1] = x1 - 1;
                next[index + 2] = y + dy;
                next[index + 3] = dy;
                index += 4;
            }
            if (x1 - 1 > x2) {
                next[index] = x2 + 1;
                next[index + 1] = x1 - 1;
                next[index + 2] = y - dy;
                next[index + 3] = -dy;
                index += 4;
            }
            x1++;
            while (x1 < x2 && getPixel(x1, y) != targetColor) x1++;
            x = x1;
        }
        if (index == 0) break;
        index -= 4;
        x1 = next[index];
        x2 = next[index + 1];
        y  = next[index + 2];
        dy = next[index + 3];
    }
}
