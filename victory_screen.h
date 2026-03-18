#pragma once


#include "globals.h"


class EndScreen {
public:
  EndScreen(const sf::Font &font, bool hasWon)
      : m_font(font), m_hasWon(hasWon), m_selectedIndex(0) {}

  EndChoice run(sf::RenderWindow &window) {
    sf::Text mainMessage(m_font);
    mainMessage.setString(m_hasWon ? "Hurray, you won!"
                                   : "Skill issue, you lost");
    mainMessage.setCharacterSize(54);
    mainMessage.setFillColor(sf::Color::Yellow);
    mainMessage.setStyle(sf::Text::Bold);
    sf::FloatRect msgBounds = mainMessage.getLocalBounds();
    mainMessage.setOrigin({msgBounds.position.x + msgBounds.size.x / 2.f,
                           msgBounds.position.y + msgBounds.size.y / 2.f});
    mainMessage.setPosition(
        {window.getSize().x / 2.f, window.getSize().y * 0.3f});

    const int OPTION_COUNT = 2;
    std::string optionLabels[OPTION_COUNT] = {"Play Again", "Quit"};
    sf::Text optionTexts[OPTION_COUNT] = {sf::Text(m_font), sf::Text(m_font)};
    sf::RectangleShape optionBoxes[OPTION_COUNT];

    for (int i = 0; i < OPTION_COUNT; ++i) {
      optionTexts[i].setString(optionLabels[i]);
      optionTexts[i].setCharacterSize(34);
      optionTexts[i].setFillColor(sf::Color::White);
      sf::FloatRect tb = optionTexts[i].getLocalBounds();
      optionTexts[i].setOrigin(
          {tb.position.x + tb.size.x / 2.f, tb.position.y + tb.size.y / 2.f});
      float yPos = window.getSize().y * 0.55f + (i * 70.f);
      optionTexts[i].setPosition({window.getSize().x / 2.f, yPos});
      sf::FloatRect gb = optionTexts[i].getGlobalBounds();
      optionBoxes[i].setSize({gb.size.x + 60.f, gb.size.y + 24.f});
      optionBoxes[i].setOrigin(
          {optionBoxes[i].getSize().x / 2.f, optionBoxes[i].getSize().y / 2.f});
      optionBoxes[i].setPosition({window.getSize().x / 2.f, yPos});
    }

    while (window.isOpen()) {
      while (const std::optional<sf::Event> ev = window.pollEvent()) {
        if (ev->is<sf::Event::Closed>()) {
          window.close();
          return EndChoice::Quit;
        }
        if (const auto *kp = ev->getIf<sf::Event::KeyPressed>()) {
          if (kp->code == sf::Keyboard::Key::Up)
            m_selectedIndex =
                (m_selectedIndex - 1 + OPTION_COUNT) % OPTION_COUNT;
          else if (kp->code == sf::Keyboard::Key::Down)
            m_selectedIndex = (m_selectedIndex + 1) % OPTION_COUNT;
          else if (kp->code == sf::Keyboard::Key::Enter) {
            if (m_selectedIndex == 0)
              return EndChoice::PlayAgain;
            window.close();
            return EndChoice::Quit;
          }
        }
      }

      for (int i = 0; i < OPTION_COUNT; ++i) {
        if (i == m_selectedIndex) {
          optionBoxes[i].setFillColor(sf::Color(0, 0, 210, 230));
          optionBoxes[i].setOutlineThickness(2.f);
          optionBoxes[i].setOutlineColor(sf::Color(120, 170, 255));
        } else {
          optionBoxes[i].setFillColor(sf::Color::Transparent);
          optionBoxes[i].setOutlineThickness(0.f);
        }
      }

      window.clear(sf::Color::Black);
      window.draw(mainMessage);
      for (int i = 0; i < OPTION_COUNT; ++i) {
        window.draw(optionBoxes[i]);
        window.draw(optionTexts[i]);
      }
      window.display();
    }
    return EndChoice::Quit;
  }

private:
  const sf::Font &m_font;
  bool m_hasWon;
  int m_selectedIndex;
};
