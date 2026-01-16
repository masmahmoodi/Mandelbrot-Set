#include "ComplexPlane.h"
#include <cmath>
#include <sstream>

using namespace sf;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
    : m_vArray(Points, pixelWidth * pixelHeight),
      m_state(State::CALCULATING),
      m_pixel_size(pixelWidth, pixelHeight),
      m_plane_center(0.f, 0.f),
      m_plane_size(BASE_WIDTH, BASE_HEIGHT)
{
    m_aspectRatio = static_cast<float>(pixelHeight) / pixelWidth;
    m_plane_size.y = BASE_HEIGHT * m_aspectRatio;
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const {
    target.draw(m_vArray);
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i pixel) {
    float xRatio = static_cast<float>(pixel.x) / m_pixel_size.x;
    float yRatio = static_cast<float>(pixel.y) / m_pixel_size.y;

    float x = m_plane_center.x - m_plane_size.x / 2 + xRatio * m_plane_size.x;
    float y = m_plane_center.y + m_plane_size.y / 2 - yRatio * m_plane_size.y;

    return {x, y};
}

size_t ComplexPlane::countIterations(Vector2f coord) {
    std::complex<double> c(coord.x, coord.y);
    std::complex<double> z(0, 0);
    size_t i = 0;

    while (std::abs(z) < 2.0 && i < MAX_ITER) {
        z = z * z + c;
        i++;
    }
    return i;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b) {
    if (count >= MAX_ITER) {
        r = g = b = 0;
        return;
    }

    float t = static_cast<float>(count) / static_cast<float>(MAX_ITER);
    r = static_cast<Uint8>(9 * (1 - t) * t * t * t * 255);
    g = static_cast<Uint8>(15 * (1 - t) * (1 - t) * t * t * 255);
    b = static_cast<Uint8>(8.5f * (1 - t) * (1 - t) * (1 - t) * t * 255);
}


void ComplexPlane::computeRows(int startRow, int endRow) {
    for (int y = startRow; y < endRow; y++) {
        for (int x = 0; x < m_pixel_size.x; x++) {
            int index = x + y * m_pixel_size.x;
            m_vArray[index].position = {(float)x, (float)y};

            Vector2f coord = mapPixelToCoords({x, y});
            size_t count = countIterations(coord);

            Uint8 r, g, b;
            iterationsToRGB(count, r, g, b);
            m_vArray[index].color = Color(r, g, b);
        }
    }
}


void ComplexPlane::updateRender() {
    if (m_state != State::CALCULATING) return;

    int rows = m_pixel_size.y;

   
    std::thread t1(&ComplexPlane::computeRows, this, 0, rows / 4);
    std::thread t2(&ComplexPlane::computeRows, this, rows / 4, rows / 2);
    std::thread t3(&ComplexPlane::computeRows, this, rows / 2, 3 * rows / 4);
    std::thread t4(&ComplexPlane::computeRows, this, 3 * rows / 4, rows);

  
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    m_state = State::DISPLAYING;
}

void ComplexPlane::zoomIn() {
    m_plane_size.x *= BASE_ZOOM;
    m_plane_size.y *= BASE_ZOOM;
    m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut() {
    m_plane_size.x /= BASE_ZOOM;
    m_plane_size.y /= BASE_ZOOM;
    m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel) {
    m_plane_center = mapPixelToCoords(mousePixel);
    m_state = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i pixel) {
    m_mouseLocation = mapPixelToCoords(pixel);
}

void ComplexPlane::loadText(Text& text) {
    std::stringstream ss;
    ss << "Mandelbrot Set\n";
    ss << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")\n";
    ss << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")\n";
    ss << "Left: Zoom In\nRight: Zoom Out";
    text.setString(ss.str());
}
