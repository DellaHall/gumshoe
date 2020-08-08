#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

int main()
{
  json j2 = {
    {"pi", 3.141},
    {"happy", true},
    {"name", "Niels"},
    {"nothing", nullptr},
    {"answer", {
        {"everything", 42}
    }},
    {"list", {1, 0, 2}},
    {"object", {
        {"currency", "USD"},
        {"value", 42.99}
    }}
  };
  // Create the main window
  sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
  // Load a sprite to display
  sf::Texture texture;
  if (!texture.loadFromFile("graphics/stfu.png"))
      return EXIT_FAILURE;
  sf::Sprite sprite(texture);
  // Create a graphical text to display
  sf::Font font;
  if (!font.loadFromFile("fonts/Hack-Bold.ttf"))
      return EXIT_FAILURE;
  sf::Text text("Hello SFML", font, 50);
  // Load a music to play
  sf::Music music;
  if (!music.openFromFile("sound/File_Mozart_-_Coronation_Mass_(Markevitch)_-_01._Kyrie.ogg"))
      return EXIT_FAILURE;
  // Play the music
  //music.play();
  // Start the game loop
  while (window.isOpen())
  {
    // Process events
    sf::Event event;
    while (window.pollEvent(event))
    {
        // Close window: exit
        if (event.type == sf::Event::Closed)
            window.close();
    }
    // Clear screen
    window.clear();
    // Draw the sprite
    window.draw(sprite);
    // Draw the string
    window.draw(text);
    // Update the window
    window.display();
  }
  return EXIT_SUCCESS;
}