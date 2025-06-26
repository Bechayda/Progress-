#include "BackgroundManager.h"
#include <iostream>


BackgroundManager::BackgroundManager(sf::RenderWindow& win)
    : window(win) {
    fadeRect.setSize(sf::Vector2f(window.getSize()));
    fadeRect.setFillColor(sf::Color(0, 0, 0, 0));
}


void BackgroundManager::setBackground(const std::string& imagePath) {
    
    if (texture.loadFromFile(imagePath)) {
        sprite.setTexture(texture);

        float windowWidth = static_cast<float>(window.getSize().x);
        float windowHeight = static_cast<float>(window.getSize().y);
        float textureWidth = static_cast<float>(texture.getSize().x);
        float textureHeight = static_cast<float>(texture.getSize().y);

        // Maintain aspect ratio and fill the screen
        float scaleX = windowWidth / textureWidth;
        float scaleY = windowHeight / textureHeight;
        float scale = std::max(scaleX, scaleY); // Cover entire window (may crop edges)

        sprite.setScale(scale, scale);

        // Center the image
        float scaledWidth = textureWidth * scale;
        float scaledHeight = textureHeight * scale;

        float offsetX = (scaledWidth - windowWidth) / 2.0f;
        float offsetY = (scaledHeight - windowHeight) / 2.0f;

        sprite.setPosition(-offsetX, -offsetY);

        hasBackground = true;
        isHiddenFlag = false;
        currentBackgroundPath = imagePath;
    }
    else {
        std::cerr << "Failed to load background: " << imagePath << std::endl;
        hasBackground = false;
        currentBackgroundPath = "";
    }
}
//}
void BackgroundManager::update(float deltaTime) {
    if (isFadingFlag) {
        fadeElapsed += deltaTime;
        fadeOpacity = std::min(255.0f, (fadeElapsed / fadeDuration) * 255.0f);
        fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(fadeOpacity)));

        if (fadeElapsed >= fadeDuration) {
            isFadingFlag = false;
            isHiddenFlag = true;
        }
    }

    if (isFadingFlag) {
        fadeElapsed += deltaTime;
        fadeOpacity = 255.0f - std::min(255.0f, (fadeElapsed / fadeDuration) * 255.0f);
        fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(fadeOpacity)));

        if (fadeElapsed >= fadeDuration) {
            isFadingFlag = false;
            fadeRect.setFillColor(sf::Color(0, 0, 0, 0));
        }
    }
}

void BackgroundManager::draw() {
    if (hasBackground && !isHiddenFlag) {
        window.draw(sprite);
    }
    if (isFadingFlag || isHiddenFlag) {
        window.draw(fadeRect);
    }
}

void BackgroundManager::startFadeOut(float duration) {
    fadeDuration = duration;
    fadeElapsed = 0.0f;
    fadeOpacity = 255.0f;
    fadeRect.setFillColor(sf::Color(0, 0, 0, 255));
    isFadingFlag = true;
    isHiddenFlag = false;
}

void BackgroundManager::resetFade() {
    isHiddenFlag = false;
    fadeOpacity = 255.0f;
    fadeRect.setFillColor(sf::Color(0, 0, 0, static_cast<sf::Uint8>(fadeOpacity)));
}

bool BackgroundManager::isFading() const {
    return isFadingFlag;
}

bool BackgroundManager::isHidden() const {
    return isHiddenFlag;
}

std::string BackgroundManager::getCurrentBackground() const {
    return currentBackgroundPath;
}