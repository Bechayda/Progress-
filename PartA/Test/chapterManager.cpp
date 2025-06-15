#include "chapterManager.h"

void ChapterManager::transitionToChapter(sf::RenderWindow& window, sf::Font& bodyfont, const std::string& chapterTitle) {
    sf::Text text;
    text.setFont(bodyfont);
    text.setString("Chapter 2: " + chapterTitle);
    text.setCharacterSize(70);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    text.setPosition(
        (window.getSize().x - text.getGlobalBounds().width) / 2,
        (window.getSize().y - text.getGlobalBounds().height) / 2
    );

    sf::RectangleShape fade(sf::Vector2f(window.getSize()));
    fade.setFillColor(sf::Color::Black);
    fade.setFillColor(sf::Color(0, 0, 0, 255)); // Full black

    sf::Clock clock;
    while (clock.getElapsedTime().asSeconds() < 4.f) {
        window.clear(sf::Color::Black);
        window.draw(fade);
        window.draw(text);
        window.display();
    }
}