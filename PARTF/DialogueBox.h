#ifndef DIALOGUEBOX_HPP
#define DIALOGUEBOX_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <queue>
#include <vector>
#include "SoundManager.h"

struct DialogueEntry {
    std::string text;
    std::string speaker;

    DialogueEntry(const std::string& t, const std::string& s) : text(t), speaker(s) {}
};

class DialogueBox {
public:
    DialogueBox(sf::RenderWindow& window, sf::Font& bodyFont, SoundManager& soundManager);

    void startDialogue(const std::vector<DialogueEntry>& entries);
    void update();
    void draw();
    void handleInput(const sf::Event& event);
    bool isVisible() const;

    void setBackground(const std::string& imagePath);
    void drawBackground();

    std::string getCurrentSpeaker() const;
    std::string getCurrentDialogue() const;
    bool wasBackPressed() const;
    void resetBackPressed();

    void blockAdvanceUntil(const std::string& soundName);

private:
    void nextDialogue();

    sf::RenderWindow& window;
    sf::Font& font;

    sf::RectangleShape background;
    sf::Text textDisplay;

    sf::RectangleShape speakerBackground;
    sf::Text speakerText;

    std::string fullText;
    std::string speakerName;
    std::size_t currentCharIndex;
    sf::Clock typeClock;
    sf::RectangleShape sidePanel;
    sf::RectangleShape rightPanel;
    bool visible;
    bool finishedTyping;
    float typeSpeed;

    std::queue<DialogueEntry> dialogues;

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    bool hasBackgroundImage = false;

    sf::RectangleShape forwardButton;
    sf::Text forwardText;
    bool autoForwardEnabled;
    sf::Clock advanceClock;

    sf::RectangleShape backButton;
    sf::Text backText;
    bool backToTitleRequested = false;

    bool backPressed;

    std::string blockedSoundName;
    SoundManager& soundManager; 
    
};

#endif // DIALOGUEBOX_HPP