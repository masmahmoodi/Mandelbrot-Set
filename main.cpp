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

            // MOUSE CLICK ZOOM (FIXED)
            if (event.type == Event::MouseButtonPressed) {

                // LEFT CLICK = ZOOM IN
                if (event.mouseButton.button == Mouse::Left &&
                    !Keyboard::isKeyPressed(Keyboard::LControl)) {

                    plane.setCenter(Vector2i(event.mouseButton.x, event.mouseButton.y));
                    plane.zoomIn();
                }

                // RIGHT CLICK = ZOOM OUT
                // (Ctrl + Click on Mac is treated as right click)
                if (event.mouseButton.button == Mouse::Right ||
                    (event.mouseButton.button == Mouse::Left &&
                     Keyboard::isKeyPressed(Keyboard::LControl))) {

                    plane.setCenter(Vector2i(event.mouseButton.x, event.mouseButton.y));
                    plane.zoomOut();
                }
            }

            // MOUSE WHEEL ZOOM
            if (event.type == Event::MouseWheelScrolled) {
                plane.setCenter(Vector2i(event.mouseWheelScroll.x,
                                         event.mouseWheelScroll.y));

                if (event.mouseWheelScroll.delta > 0)
                    plane.zoomIn();
                else
                    plane.zoomOut();
            }
        }

        // KEYBOARD ZOOM (FALLBACK)
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
