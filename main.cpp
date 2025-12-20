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
                plane.setMouseLocation({event.mouseMove.x, event.mouseMove.y});
            }

            if (event.type == Event::MouseButtonPressed) {
                plane.setCenter({event.mouseButton.x, event.mouseButton.y});

                if (event.mouseButton.button == Mouse::Left)
                    plane.zoomIn();
                else if (event.mouseButton.button == Mouse::Right)
                    plane.zoomOut();
            }
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
