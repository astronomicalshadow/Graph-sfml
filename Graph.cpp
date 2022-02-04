// Graph.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <sstream>
#include <math.h>
#include <SFML/Graphics.hpp>

int main()
{
    // Screen
    int ScreenWidth = 1280, ScreenHeight = 720;
    sf::RenderWindow window(sf::VideoMode(ScreenWidth, ScreenHeight), "Graph");

    // Anti aliasing
    sf::ContextSettings AntiAliasing;
    AntiAliasing.antialiasingLevel = 8;

    // FPS Limit
    window.setFramerateLimit(60);

    // Camera
    sf::View Camera(sf::Vector2f(0, 0), sf::Vector2f(300, 200));
    window.setView(Camera);


    sf::Texture BackgroundTexture;
    BackgroundTexture.loadFromFile("Graphics/shooting-star-sunset-anime-horizon-30-4k.jpg");
    sf::Sprite BackgroundSprite;
    BackgroundSprite.setTexture(BackgroundTexture);
    BackgroundSprite.setOrigin(BackgroundSprite.getScale().x / 2.0f, BackgroundSprite.getScale().y / 2.0f);
    BackgroundSprite.setPosition(0, 0);

    // Square
    sf::RectangleShape square(sf::Vector2f(120, 50));
    square.setPosition(0, 0);

    sf::Vector2f CursorPosition;
    bool CursorMoving = false;

    // XY Line
    sf::Vertex X[] =
    {
        sf::Vertex(sf::Vector2f(-2000,   0), sf::Color::White),
        sf::Vertex(sf::Vector2f(2000, 0), sf::Color::White),
    };
    sf::Vertex Y[] =
    {
        sf::Vertex(sf::Vector2f(0, -2000), sf::Color::White),
        sf::Vertex(sf::Vector2f(0, 2000), sf::Color::White),
    };

    // Graph
    sf::VertexArray Graph(sf::LinesStrip);
    for (double x = -500.f; x < 500.f; x += 0.01)
    {
        double y = -pow((x + 1), 2) * (x - 1) * (x + 2) * (x - 2);
        double y2 = sin(x);
        Graph.append(sf::Vertex(sf::Vector2f(x, y), sf::Color::Red));
        Graph.append(sf::Vertex(sf::Vector2f(x, y2), sf::Color::Blue));
    }

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyPressed:
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                {
                    Camera.move(sf::Vector2f(0.f, 1.f));
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {
                    Camera.move(sf::Vector2f(0.f, -1.f));
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                {
                    Camera.move(sf::Vector2f(-10.f, 0.f));
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                    Camera.move(sf::Vector2f(10.f, 0.f));
                }
                break;

            case sf::Event::MouseButtonPressed:
                // Mouse button is pressed, get the position and set moving as active
                if (event.mouseButton.button == sf::Mouse::Right)
                {
                    CursorMoving = true;
                    CursorPosition = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
                else if (event.mouseButton.button == sf::Mouse::Left)
                {
                    CursorPosition = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
                break;
            case  sf::Event::MouseButtonReleased:
                // Mouse button is released, no longer move
                if (event.mouseButton.button == sf::Mouse::Right) {
                    CursorMoving = false;
                }
                if (event.mouseButton.button == sf::Mouse::Left)
                    break;

            case sf::Event::MouseMoved:
            {
                // Ignore mouse movement unless a button is pressed (see above)
                if (!CursorMoving)
                    break;
                // Determine the new position in world coordinates
                const sf::Vector2f NewCursorPosition = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                // Determine how the cursor has moved
                // Swap these to invert the movement direction
                const sf::Vector2f CursorMovedDistance = CursorPosition - NewCursorPosition;
                // Move our view accordingly and update the window
                Camera.move(CursorMovedDistance);
                window.setView(Camera);

                // Save the new position as the old one
                // We're recalculating this, since we've changed the view
                CursorPosition = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                break;
            }

            case sf::Event::MouseWheelMoved:
            {
                float Zoom = 0;
                if (event.mouseWheel.delta <= -1 && Camera.getSize().x < 2500)
                {
                    Zoom = 1.1f;
                    Camera.zoom(Zoom);
                    window.setView(Camera);
                }
                else if (event.mouseWheel.delta >= 1)
                {
                    Zoom = 0.9f;
                    Camera.zoom(Zoom);
                    window.setView(Camera);
                }
                std::cout << Camera.getSize().x << "," << Camera.getSize().y << "\n\n";
                break;


            }
            }

            window.clear();
            //window.draw(BackgroundSprite);
            window.draw(Graph);
            window.draw(X, 2, sf::Lines);
            window.draw(Y, 2, sf::Lines);
            window.draw(square);
            window.display();

        }
    }
}

