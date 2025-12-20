// ComplexPlane.cpp
#include "ComplexPlane.h"
#include <cmath>
#include <sstream>

using namespace sf;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
    : m_vArray(Points, pixelWidth * pixelHeight),
      m_state(State::CALCULATING),
      m_pixel_size(pixelWidth, pixelHeight),
      m_plane_center(0.f, 0.f),
      m_plane_size(BASE_WIDTH, BASE_HEIGHT),
      m_zoomCount(0)
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

    float x = m_plane_center.x - m_plane_size.x / 2.f + xRatio * m_plane_size.x;
    float y = m_plane_center.y - m_plane_size.y / 2.f + yRatio * m_plane_size.y;

    return Vector2f(x, y);
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

    float t = static_cast<float>(count) / MAX_ITER;
    r = static_cast<Uint8>(9 * (1 - t) * t * t * t * 255);
    g = static_cast<Uint8>(15 * (1 - t) * (1 - t) * t * t * 255);
    b = static_cast<Uint8>(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
}

void ComplexPlane::renderRows(int yStart, int yEnd) { // extra credit
    for (int y = yStart; y < yEnd; y++) {
        for (int x = 0; x < m_pixel_size.x; x++) {
            int index = y * m_pixel_size.x + x;

            m_vArray[index].position = Vector2f(static_cast<float>(x), static_cast<float>(y));

            Vector2f coord = mapPixelToCoords(Vector2i(x, y));
            size_t count = countIterations(coord);

            Uint8 r, g, b;
            iterationsToRGB(count, r, g, b);
            m_vArray[index].color = Color(r, g, b);
        }
    }
}

void ComplexPlane::updateRender() {
    if (m_state != State::CALCULATING) return;

    unsigned int numThreads = std::thread::hardware_concurrency(); // extra credit
    if (numThreads == 0) numThreads = 4; // extra credit
    if (numThreads > static_cast<unsigned int>(m_pixel_size.y)) numThreads = m_pixel_size.y; // extra credit

    std::vector<std::thread> threads; // extra credit
    threads.reserve(numThreads); // extra credit

    int rowsPerThread = m_pixel_size.y / static_cast<int>(numThreads); // extra credit
    int extra = m_pixel_size.y % static_cast<int>(numThreads); // extra credit

    int yStart = 0; // extra credit
    for (unsigned int t = 0; t < numThreads; t++) { // extra credit
        int yEnd = yStart + rowsPerThread + (t < static_cast<unsigned int>(extra) ? 1 : 0); // extra credit
        threads.emplace_back(&ComplexPlane::renderRows, this, yStart, yEnd); // extra credit
        yStart = yEnd; // extra credit
    }

    for (auto& th : threads) th.join(); // extra credit

    m_state = State::DISPLAYING;
}

void ComplexPlane::zoomIn() {
    m_zoomCount++;
    float scale = std::pow(BASE_ZOOM, m_zoomCount);
    m_plane_size.x = BASE_WIDTH * scale;
    m_plane_size.y = BASE_HEIGHT * m_aspectRatio * scale;
    m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut() {
    m_zoomCount--;
    float scale = std::pow(BASE_ZOOM, m_zoomCount);
    m_plane_size.x = BASE_WIDTH * scale;
    m_plane_size.y = BASE_HEIGHT * m_aspectRatio * scale;
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
    ss << "Center: (" << m_plane_center.x << "," << m_plane_center.y << ")\n";
    ss << "Cursor: (" << m_mouseLocation.x << "," << m_mouseLocation.y << ")\n";
    ss << "Left-click: Zoom in\n";
    ss << "Right-click: Zoom out\n";
    ss << "Mouse wheel: Zoom in/out"; // extra credit
    text.setString(ss.str());
}
