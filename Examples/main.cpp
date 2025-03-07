#include <SFML/Graphics.hpp>
#include<iostream>

int main()
{
    float radius = 10.0f;
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    window.setFramerateLimit(120);
    sf::CircleShape shape;
    shape.setPosition(sf::Vector2f(200, 200));
    shape.setRadius(radius);
    shape.setFillColor(sf::Color::Green);
    sf::Vector2f direction = sf::Vector2f(5.0f, 2.5f);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        std::cout << window.getPosition().x << " " << window.getPosition().y;
        sf::Vector2f shapepos = shape.getPosition();

        sf::Vector2u windowSize = window.getSize();
        sf::Vector2f windowMinSize(0.0, 0.0f);

        if (windowSize.x <= shapepos.x + 2 * radius) {
            direction.x = -direction.x;
        }
        if (windowSize.y <= shapepos.y + 2 * radius) {
            direction.y = -direction.y;
        }
        if (windowMinSize.x >= shapepos.x) {
            direction.x = -direction.x;
        }
        if (windowMinSize.y >= shapepos.y) {
            direction.y = -direction.y;
        }
        shape.setPosition(shape.getPosition() + direction);



        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}