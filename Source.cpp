#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

class Game {
public:
    Game() : window(sf::VideoMode(800, 600), "Spaceship Game"), spaceship(sf::Vector2f(50, 50)), gameOver(false), score(0), startScreen(true) {
        window.setFramerateLimit(60);

        // Load background texture
        if (!backgroundTexture.loadFromFile("images/space.jpg")) {
            // Handle error loading texture
            std::cerr << "Error loading background texture!" << std::endl;
        }

        // Set texture for background
        background.setTexture(backgroundTexture);

        // Load spaceship texture
        if (!spaceshipTexture.loadFromFile("images/spaceship.png")) {
            // Handle error loading texture
            std::cerr << "Error loading spaceship texture!" << std::endl;
        }

        // Set texture for spaceship
        spaceship.setTexture(&spaceshipTexture);

        // Load asteroid texture
        if (!asteroidTexture.loadFromFile("images/rock2.png")) {
            // Handle error loading texture
            std::cerr << "Error loading asteroid texture!" << std::endl;
        }

        // Set texture for asteroid sprite
        asteroidSprite.setTexture(asteroidTexture);

        // Load font for buttons
        if (!font.loadFromFile("font/BAUHS93.ttf")) {
            // Handle error loading font
            std::cerr << "Error loading font!" << std::endl;
        }

        // Set up start button
        startButton.setSize(sf::Vector2f(145, 50));
        startButton.setFillColor(sf::Color::Yellow);
        startButtonText.setFont(font);
        startButtonText.setString("Start Game");
        startButtonText.setCharacterSize(20);
        startButtonText.setFillColor(sf::Color::Black);

        // Set up restart button
        restartButton.setSize(sf::Vector2f(175, 50));
        restartButton.setFillColor(sf::Color::Yellow);
        restartButtonText.setFont(font);
        restartButtonText.setString("Restart Game");
        restartButtonText.setCharacterSize(20);
        restartButtonText.setFillColor(sf::Color::Black);

        // Create initial asteroids
        createAsteroids(3); // Change the number of asteroids as needed
    }

    void run() {



        while (window.isOpen()) {
            if (startScreen) {
                processStartScreenEvents();
                renderStartScreen();
            }
            else {
                processEvents();
                update();
                render();
            }
        }
    }

private:
    sf::RenderWindow window;
    sf::RectangleShape spaceship;
    sf::Texture spaceshipTexture; // Texture for spaceship
    sf::Sprite asteroidSprite;    // Use sf::Sprite for asteroids
    sf::Texture asteroidTexture;  // Texture for asteroids
    sf::Sprite background;        // Background sprite
    sf::Texture backgroundTexture; // Texture for background
    sf::RectangleShape startButton; // Start button
    sf::Text startButtonText; // Text on the start button
    sf::RectangleShape restartButton; // Restart button
    sf::Text restartButtonText; // Text on the restart button
    bool gameOver;
    int score;
    bool startScreen;
    sf::Clock asteroidSpawnClock; // Clock for tracking asteroid spawn time
    sf::Clock scoreClock; // Clock for tracking score

    std::vector<sf::Sprite> asteroids; // Vector to store asteroid sprites

    sf::Font font;

    void processStartScreenEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    sf::Vector2f mousePositionF(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

                    if (startButton.getGlobalBounds().contains(mousePositionF)) {
                        startScreen = false;
                        resetGame();
                    }
                }
            }
        }
    }


    void processEvents() {
        sf::Event event;

        // Handle continuous input outside the event loop
        if (!gameOver) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && spaceship.getPosition().x > 0) {
                spaceship.move(-5, 0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && spaceship.getPosition().x < 750) {
                spaceship.move(5, 0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && spaceship.getPosition().y > 0) {
                spaceship.move(0, -5);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && spaceship.getPosition().y < 550) {
                spaceship.move(0, 5);
            }
        }

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    sf::Vector2f mousePositionF(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

                    if (startScreen && startButton.getGlobalBounds().contains(mousePositionF)) {
                        startScreen = false;
                        resetGame();
                    }

                    if (gameOver && restartButton.getGlobalBounds().contains(mousePositionF)) {
                        resetGame();
                    }
                }
            }
        }
    }



    void update() {
        if (!gameOver) {
            // Update asteroid positions
            for (auto& asteroid : asteroids) {
                asteroid.move(0, 6);

                // Check for collision with each asteroid
                if (spaceship.getGlobalBounds().intersects(asteroid.getGlobalBounds())) {
                    gameOver = true;
                }

                // Check if asteroid is out of bounds
                if (asteroid.getPosition().y > window.getSize().y) {
                    // Remove the asteroid from the vector
                    asteroids.erase(asteroids.begin());
                }
            }

            // Check if it's time to spawn a new asteroid
            if (asteroidSpawnClock.getElapsedTime().asSeconds() > 0.5) { // Adjust the time interval as needed
                createAsteroid();
                asteroidSpawnClock.restart();
            }

            // Update the score
            if (scoreClock.getElapsedTime().asSeconds() >= 1.0) {
                score++; // Increment score every second
                scoreClock.restart();
            }
        }
    }

    void renderStartScreen() {
        window.clear();

        // Draw the background first
        window.draw(background);

        // Draw the game name
        sf::Text gameNameText("Astro-dodge", font, 50);
        gameNameText.setFillColor(sf::Color::Red);
        gameNameText.setPosition(275, 200);
        window.draw(gameNameText);

        // Draw the start button
        startButton.setPosition(335, 400);
        window.draw(startButton);

        startButtonText.setPosition(360, 410);
        window.draw(startButtonText);

        window.display();
    }

    void render() {
        window.clear();

        // Draw the background first
        window.draw(background);

        if (!gameOver) {
            window.draw(spaceship);
            for (const auto& asteroid : asteroids) {
                window.draw(asteroid); // Draw each asteroid sprite
            }

            // Draw the score
            sf::Text scoreText("Score: " + std::to_string(score), font, 20);
            scoreText.setFillColor(sf::Color::White);
            scoreText.setPosition(10, 10);
            window.draw(scoreText);
        }
        else {
            // Display game over message and final score
            sf::Text gameOverText("Game Over!", font, 50);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(250, 250);
            window.draw(gameOverText);

            sf::Text finalScoreText("Final Score: " + std::to_string(score), font, 30);
            finalScoreText.setFillColor(sf::Color::White);
            finalScoreText.setPosition(300, 320);
            window.draw(finalScoreText);

            // Display restart button
            restartButton.setPosition(300, 400);
            window.draw(restartButton);

            restartButtonText.setPosition(325, 410);
            window.draw(restartButtonText);
        }

        window.display();
    }

    void createAsteroid() {
        sf::Sprite newAsteroid = asteroidSprite;
        newAsteroid.setPosition(std::rand() % 750, 0);
        asteroids.push_back(newAsteroid);
    }

    void createAsteroids(int count) {
        asteroids.clear();
        for (int i = 0; i < count; ++i) {
            sf::Sprite asteroidCopy = asteroidSprite;
            asteroidCopy.setPosition(std::rand() % 750, i * -150); // Initial positions staggered for each asteroid
            asteroids.push_back(asteroidCopy);
        }
    }

    void resetGame() {
        gameOver = false;
        score = 0;

        // Reset the clocks
        asteroidSpawnClock.restart();
        scoreClock.restart();

        // Recreate initial asteroids
        createAsteroids(3);

        // Reset spaceship position
        spaceship.setPosition(375, 500);
    }
};

int main() {
    Game game;
    game.run();

    return 0;
}
