#pragma once
#include <SFML/Graphics.hpp>
#include <complex>
#include <thread>  
#include <vector>  // extra credit

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0f;
const float BASE_HEIGHT = 4.0f;
const float BASE_ZOOM = 0.5f;

class ComplexPlane : public sf::Drawable {
public:
    enum class State { CALCULATING, DISPLAYING };

    ComplexPlane(int pixelWidth, int pixelHeight);
    void updateRender();
    void zoomIn();
    void zoomOut();
    void setCenter(sf::Vector2i mousePixel);
    void setMouseLocation(sf::Vector2i mousePixel);
    void loadText(sf::Text& text);

    void setLastClick(char c); // extra credit

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::VertexArray m_vArray;
    State m_state;
    sf::Vector2f m_mouseLocation;
    sf::Vector2i m_pixel_size;
    sf::Vector2f m_plane_center;
    sf::Vector2f m_plane_size;
    int m_zoomCount;
    float m_aspectRatio;

    char m_lastClick; 
    sf::Vector2f mapPixelToCoords(sf::Vector2i pixel);
    size_t countIterations(sf::Vector2f coord);
    void iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b);

    void computeRows(int startRow, int endRow); 
};
