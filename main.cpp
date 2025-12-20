#include <SFML/Graphics.hpp>
#include "ComplexPlane.h"

using namespace sf;

int main() {
    VideoMode desktop = VideoMode::getDesktopMode();
    unsigned int width = desktop.width / 2;
    unsigned int height = desktop.height / 2;

    RenderWindow window(VideoMode(width, height), "Mandelbrot Viewer");
    window.setFramerateLimit(30);

    ComplexPlane plane(width, height);

    Font font;
    font.loadFromFile("arial.ttf");

    Text hud("", font, 20);
    hud.setFillColor(Color::White);
    hud.setPosition(10, 10);

    while (window.isOpen()) {

        Event event;
        while (window.pollEvent(event)) {

            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseMoved) {
                plane.setMouseLocation(Vector2i(event.mouseMove.x, event.mouseMove.y));
            }

            // =========================================
            // CLICK ZOOM (WORKS ON VDI/MAC + REAL MOUSE)
            // Left click  -> zoom in
            // Right click -> zoom out (if SFML receives Mouse::Right)
            // Ctrl+Left   -> zoom out (VDI/Mac "right click" fallback)
            // =========================================
            if (event.type == Event::MouseButtonPressed) {

                bool isCtrl =
                    Keyboard::isKeyPressed(Keyboard::LControl) ||
                    Keyboard::isKeyPressed(Keyboard::RControl);

                bool zoomOut =
                    (event.mouseButton.button == Mouse::Right) ||
                    (event.mouseButton.button == Mouse::Left && isCtrl);

                plane.setCenter(Vector2i(event.mouseButton.x, event.mouseButton.y));

                if (zoomOut) plane.zoomOut();
                else         plane.zoomIn();
            }

            // MOUSE WHEEL = ZOOM IN / OUT
            if (event.type == Event::MouseWheelScrolled) {
                plane.setCenter(Vector2i(event.mouseWheelScroll.x,
                                         event.mouseWheelScroll.y));

                if (event.mouseWheelScroll.delta > 0)
                    plane.zoomIn();
                else
                    plane.zoomOut();
            }
        }

        // KEYBOARD ZOOM (fallback)
        if (Keyboard::isKeyPressed(Keyboard::Equal) ||
            Keyboard::isKeyPressed(Keyboard::Add)) {
            plane.zoomIn();
        }

        if (Keyboard::isKeyPressed(Keyboard::Hyphen) ||
            Keyboard::isKeyPressed(Keyboard::Subtract)) {
            plane.zoomOut();
        }

        plane.updateRender();
        plane.loadText(hud);

        window.clear();
        window.draw(plane);
        window.draw(hud);
        window.display();
    }

    return 0;
}
