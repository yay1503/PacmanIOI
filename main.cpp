
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>

// arena wali file ko include kiya
#include "arena.cpp"
#include "globals.h"

// pacman_movement wali file ko include kiya
#include "pacman_movement.h"

// pacman_draw wali file ko include kiya (animated chomping + death animation)
#include "pacman_draw.h"

// xp_orbs wali file ko include kiya (orbs + score)
#include "xp_orbs.h"

int main() {
  unsigned mapW = (unsigned)baseMap[0].size();
  unsigned mapH = (unsigned)baseMap.size();
  const float uiOffset = 40.0f; // Leaves room at the top for score/lives UI

  // Initialize main rendering window scaled by map tile sizes
  sf::RenderWindow window(sf::VideoMode(sf::Vector2u(
                              mapW * (unsigned)TILE_SIZE,
                              mapH * (unsigned)TILE_SIZE + (unsigned)uiOffset)),
                          "Pac-Man Core");

  // Load font for score display
  sf::Font font("C:/Windows/Fonts/arial.ttf");

  // game starts
  Entity pacman;
  int score = 0;
  // Starting pixel location for Pacman on spawn
  sf::Vector2f pacSpawn(14 * TILE_SIZE + TILE_SIZE / 2,
                        5 * TILE_SIZE + TILE_SIZE / 2 + uiOffset);

  // Spawn orbs on every walkable tile
  std::vector<sf::Vector2f> orbs = spawnOrbs(uiOffset);

  auto resetGame = [&]() {
    pacman.pos = pacSpawn;
    pacman.currentDir = Direction::Left;
    pacman.queuedDir = Direction::Left;
    pacman.color = sf::Color::Yellow;
    score = 0;
    orbs = spawnOrbs(uiOffset);
  };

  resetGame();
  sf::Clock clock;

  while (window.isOpen()) {
    float dt = clock.restart().asSeconds();
    if (dt > 0.1f)
      dt = 0.1f;
    static float animTime = 0.f;
    animTime += dt;

    while (const std::optional<sf::Event> ev = window.pollEvent())
      if (ev->is<sf::Event::Closed>())
        window.close();

    // Take inputs for pacman
    handlePacmanInput(pacman);
    moveEntity(pacman, PACMAN_SPEED, false, dt, uiOffset, mapW);

    // Collect orbs that Pac-Man touches
    collectOrbs(orbs, pacman.pos, score);

    window.clear(sf::Color::Black);

    drawArena(window, uiOffset, animTime);

    // Draw orbs
    drawOrbs(window, orbs);

    // Animated chomping Pac-Man draw
    drawPacman(window, pacman.pos, pacman.color, pacman.currentDir, animTime);

    // Draw score HUD
    drawScore(window, font, score);

    window.display();
  }
  return 0;
}
