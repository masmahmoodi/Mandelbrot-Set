#include "ComplexPlane.h"
#include <cmath>
#include <sstream>
#include <iomanip>

using namespace sf;

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
    : m_vArray(Points, pixelWidth * pixelHeight),
      m_state(State::CALCULATING),
      m_mouseLocation(0.f, 0.f),
      m_pixel_size(pixelWidth, pixelHeight),
      m_plane_center(0.f, 0.f),
      m_plane_size(BASE_WIDTH, BASE_HEIGHT),
      m_zoomCount(0),
      m_aspectRatio(1.f),
      m_lastClick('-') // extra credit
{
    m_aspectRatio = static_cast<float>(pixelHeight) / static_cast<float>(pixelWidth);
    m_plane_size.y = BASE_HEIGHT * m_aspectRatio;
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const {
    target.draw(m_vArray);
}

void ComplexPlane::setLastClick(char c) { // extra credit
    m_lastClick = c; // extra credit
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i pixel) {
    float xRatio = static_cast<float>(pixel.x) / static_cast<float>(m_pixel_size.x);
    float yRatio = static_cast<float>(pixel.y) / static_cast<float>(m_pixel_size.y);

    float x = (m_plane_center.x - m_plane_size.x / 2.f) + xRatio * m_plane_size.x;

    float top = (m_plane_center.y + m_plane_size.y / 2.f);
    float y = top - yRatio * m_plane_size.y;

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
    if (count >= MAX_ITER) { r = g = b = 0; return; }

    float t = static_cast<float>(count) / static_cast<float>(MAX_ITE_
