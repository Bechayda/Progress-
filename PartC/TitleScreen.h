#ifndef TITLESCREEN_HPP
#define TITLESCREEN_HPP

#include <SFML/Graphics.hpp>
#include "SoundManager.h"

class TitleScreen {
public:
    // Constructor: Initializes the title screen with references to the window, fonts, and SoundManager
    TitleScreen(sf::RenderWindow& win, sf::Font& titleFont, sf::Font& bodyFont, SoundManager& soundManager);
    

    enum class GameState { TITLE, ABOUT, GAMEPLAY, QUIT, LOAD };
    GameState run();

    // Draws the title screen elements (background, text, buttons) to the window
    void draw();
    sf::RenderWindow& window;
    sf::Font& titleFont;
    sf::Font& bodyFont;
    SoundManager& soundManager;

    sf::Texture backgroundTexture;
    sf::Sprite background;

    sf::Texture playTexture, loadTexture, aboutTexture, quitTexture;
    sf::Sprite playSprite, loadSprite, aboutSprite, quitSprite;

    // Text elements for the title and buttons
    sf::Text titleLine1;
    sf::Text titleLine2;
    sf::Text playButton;
    sf::Text aboutButton;
    sf::Text quitButton;
    sf::Text loadButton;



    // Animates the title text based on elapsed time (e.g., fading, moving, etc.)
    void animateTitles(float time);
    GameState handleEvents();
    void handleMouseHover(const sf::Vector2f& mousePos);
};

#endif // TITLESCREEN_HPP