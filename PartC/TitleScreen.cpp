#include "TitleScreen.h"
#include <cmath>
#include <iostream>

// Constructor to initialize assets and layout
/*TitleScreen::TitleScreen(sf::RenderWindow& win, sf::Font& titleFont, sf::Font& bodyFont, SoundManager& soundManager)
    : window(win), titleFont(titleFont), bodyFont(bodyFont), soundManager(soundManager)
{
    backgroundTexture.loadFromFile("newfront.png");
    background.setTexture(backgroundTexture);

    float scaleX = (float)window.getSize().x / backgroundTexture.getSize().x;
    float scaleY = (float)window.getSize().y / backgroundTexture.getSize().y;
    background.setScale(scaleX, scaleY);

    /*titleLine1.setFont(titleFont);
    titleLine1.setString("Escape from");
    titleLine1.setCharacterSize(80);
    titleLine1.setFillColor(sf::Color::Red);
    titleLine1.setPosition(50, 30);

    titleLine2.setFont(titleFont);
    titleLine2.setString("Biringan City");
    titleLine2.setCharacterSize(100);
    titleLine2.setFillColor(sf::Color::Red);
    titleLine2.setPosition(30, 100);

    playButton.setFont(bodyFont);
    aboutButton.setFont(bodyFont);
    quitButton.setFont(bodyFont);
    loadButton.setFont(bodyFont);

    playButton.setString("Play");
    aboutButton.setString("About");
    loadButton.setString("Load");
    quitButton.setString("Quit");

    playButton.setCharacterSize(48);
    aboutButton.setCharacterSize(48);
    loadButton.setCharacterSize(48);
    quitButton.setCharacterSize(48);

    playButton.setPosition(80, 300);
    aboutButton.setPosition(80, 400);
    loadButton.setPosition(80, 500);  // Adjust position to include Load button
    quitButton.setPosition(80, 600);
}

TitleScreen::GameState TitleScreen::run() {
    sf::Clock clock;
    soundManager.playMusic("BeginMusic.ogg", true);  // Background music

    while (window.isOpen()) {
        GameState state = handleEvents();
        if (state != GameState::TITLE) return state;

        float time = clock.getElapsedTime().asSeconds();
        animateTitles(time);

        draw();
    }
    return GameState::QUIT;
}

void TitleScreen::handleMouseHover(const sf::Vector2f& mousePos) {
    playButton.setFillColor(playButton.getGlobalBounds().contains(mousePos) ? sf::Color::Red : sf::Color::Black);
    aboutButton.setFillColor(aboutButton.getGlobalBounds().contains(mousePos) ? sf::Color::Red : sf::Color::Black);
    loadButton.setFillColor(loadButton.getGlobalBounds().contains(mousePos) ? sf::Color::Red : sf::Color::Black);
    quitButton.setFillColor(quitButton.getGlobalBounds().contains(mousePos) ? sf::Color::Red : sf::Color::Black);
}

TitleScreen::GameState TitleScreen::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (playButton.getGlobalBounds().contains(mousePos)) {
                soundManager.stopMusic();
                return GameState::GAMEPLAY;
            }
            if (aboutButton.getGlobalBounds().contains(mousePos))
                return GameState::ABOUT;
            if (loadButton.getGlobalBounds().contains(mousePos))
                return GameState::LOAD;  // Fully functional Load button
            if (quitButton.getGlobalBounds().contains(mousePos)) {
                window.close();
                return GameState::QUIT;
            }
        }
    }

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    handleMouseHover(mousePos);
    return GameState::TITLE;
}

void TitleScreen::animateTitles(float time) {
    float offsetY1 = std::sin(time * 3.0f) * 2.0f;
    float scale1 = 1.0f + std::sin(time * 2.0f) * 0.01f;
    float offsetY2 = std::sin(time * 2.5f + 1.0f) * 5.0f;
    float scale2 = 1.0f + std::sin(time * 3.5f + 0.5f) * 0.02f;

    titleLine1.setPosition(50, 30 + offsetY1);
    titleLine2.setPosition(30, 100 + offsetY2);
    titleLine1.setScale(scale1, scale1);
    titleLine2.setScale(scale2, scale2);
}

void TitleScreen::draw() {
    window.clear();
    window.draw(background);
    window.draw(titleLine1);
    window.draw(titleLine2);
    window.draw(playButton);
    window.draw(aboutButton);
    window.draw(loadButton);  // Draw Load button
    window.draw(quitButton);
    window.display();
}*/
TitleScreen::TitleScreen(sf::RenderWindow& win, sf::Font& titleFont, sf::Font& bodyFont, SoundManager& soundManager)
    : window(win), titleFont(titleFont), bodyFont(bodyFont), soundManager(soundManager)
{
    backgroundTexture.loadFromFile("newfront.png");
    background.setTexture(backgroundTexture);

    float scaleX = static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x;
    float scaleY = static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y;
    background.setScale(scaleX, scaleY);

    // Load Baybayin button images
    playTexture.loadFromFile("start1.jpg");
    loadTexture.loadFromFile("buhayin.png");
    aboutTexture.loadFromFile("gabay.png");
    quitTexture.loadFromFile("exit.png");

    playSprite.setTexture(playTexture);
    loadSprite.setTexture(loadTexture);
    aboutSprite.setTexture(aboutTexture);
    quitSprite.setTexture(quitTexture);

    // Scale down
    float targetWidth = 130.f;
    float scalePlay = targetWidth / playTexture.getSize().x;
    float scaleLoad = targetWidth / loadTexture.getSize().x;
    float scaleAbout = targetWidth / aboutTexture.getSize().x;
    float scaleQuit = targetWidth / quitTexture.getSize().x;

    playSprite.setScale(scalePlay, scalePlay);
    loadSprite.setScale(scaleLoad, scaleLoad);
    aboutSprite.setScale(scaleAbout, scaleAbout);
    quitSprite.setScale(scaleQuit, scaleQuit);

    // 🔄 Center origin of each sprite (so we can align center to circles)
    playSprite.setOrigin(playTexture.getSize().x / 2, playTexture.getSize().y / 2);
    loadSprite.setOrigin(loadTexture.getSize().x / 2, loadTexture.getSize().y / 2);
    aboutSprite.setOrigin(aboutTexture.getSize().x / 2, aboutTexture.getSize().y / 2);
    quitSprite.setOrigin(quitTexture.getSize().x / 2, quitTexture.getSize().y / 2);

    // 🔧 Position aligned with circles
    float xCenter = 1260; // Align center X with glowing dots
    float yStart = 160;   // First circle center Y
    float spacing = 120;  // Distance between dots

    playSprite.setPosition(xCenter, yStart);
    loadSprite.setPosition(xCenter, yStart + 1 * spacing);
    aboutSprite.setPosition(xCenter, yStart + 2 * spacing);
    quitSprite.setPosition(xCenter, yStart + 3 * spacing);
}

TitleScreen::GameState TitleScreen::run() {
    sf::Clock clock;
    soundManager.playMusic("BeginM.ogg", true);

    while (window.isOpen()) {
        GameState state = handleEvents();
        if (state != GameState::TITLE) return state;

        float time = clock.getElapsedTime().asSeconds();
        animateTitles(time);

        draw();
    }

    return GameState::QUIT;
}

void TitleScreen::handleMouseHover(const sf::Vector2f& mousePos) {
    auto handleHover = [&](sf::Sprite& sprite) {
        if (sprite.getGlobalBounds().contains(mousePos))
            sprite.setColor(sf::Color(255, 100, 100));  // reddish hover
        else
            sprite.setColor(sf::Color::White);
        };

    handleHover(playSprite);
    handleHover(loadSprite);
    handleHover(aboutSprite);
    handleHover(quitSprite);
}

TitleScreen::GameState TitleScreen::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (playSprite.getGlobalBounds().contains(mousePos)) {
                soundManager.stopMusic();
                return GameState::GAMEPLAY;
            }
            if (loadSprite.getGlobalBounds().contains(mousePos))
                return GameState::LOAD;
            if (aboutSprite.getGlobalBounds().contains(mousePos))
                return GameState::ABOUT;
            if (quitSprite.getGlobalBounds().contains(mousePos)) {
                window.close();
                return GameState::QUIT;
            }
        }
    }

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    handleMouseHover(mousePos);

    return GameState::TITLE;
}

void TitleScreen::animateTitles(float time) {
    // Optional animations if you ever bring text back
}

void TitleScreen::draw() {
    window.clear();
    window.draw(background);
    window.draw(playSprite);
    window.draw(loadSprite);
    window.draw(aboutSprite);
    window.draw(quitSprite);
    window.display();
}