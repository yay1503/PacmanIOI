#pragma once

#include "globals.h"

inline void drawPacman(sf::RenderWindow &window, sf::Vector2f pos,
                       sf::Color color, Direction dir, float animTime) {
  float radius = TILE_SIZE / 2.0f * 0.8f;
  float mouthAngle = std::abs(std::sin(animTime * 15.0f)) * 70.0f;
  float startAngle = 0;
  if (dir == Direction::Down)
    startAngle = 90;
  else if (dir == Direction::Left)
    startAngle = 180;
  else if (dir == Direction::Up)
    startAngle = 270;
  float half = mouthAngle / 2.0f;
  const int pts = 30;
  sf::VertexArray shape(sf::PrimitiveType::Triangles, pts * 3);
  for (int i = 0; i < pts; ++i) {
    float a1 = (startAngle + half + (360.0f - mouthAngle) * (i / (float)pts)) *
               3.14159f / 180.f;
    float a2 =
        (startAngle + half + (360.0f - mouthAngle) * ((i + 1) / (float)pts)) *
        3.14159f / 180.f;
    shape[i * 3 + 0] = sf::Vertex{pos, color};
    shape[i * 3 + 1] = sf::Vertex{
        pos + sf::Vector2f(std::cos(a1) * radius, std::sin(a1) * radius),
        color};
    shape[i * 3 + 2] = sf::Vertex{
        pos + sf::Vector2f(std::cos(a2) * radius, std::sin(a2) * radius),
        color};
  }
  window.draw(shape);
}

inline void drawPacmanDeath(sf::RenderWindow &window, sf::Vector2f pos,
                            sf::Color color, float progress) {
  float radius = TILE_SIZE / 2.0f * 0.8f;
  if (progress < 0.3f) {
    float t = progress / 0.3f;
    int flash = (int)(t * 10.0f) % 2;
    sf::Color c = flash ? sf::Color::White : color;
    sf::CircleShape circle(radius);
    circle.setOrigin({radius, radius});
    circle.setPosition(pos);
    circle.setFillColor(c);
    window.draw(circle);
  } else if (progress < 0.95f) {
    float t = (progress - 0.3f) / 0.65f;
    float mouthAngle = t * 360.0f;
    float shrink = 1.0f - t * 0.4f;
    float r = radius * shrink;
    if (mouthAngle < 355.0f) {
      float startAngle = 90.0f, half = mouthAngle / 2.0f;
      float arcSpan = 360.0f - mouthAngle;
      const int pts = 30;
      sf::VertexArray shape(sf::PrimitiveType::Triangles, pts * 3);
      for (int i = 0; i < pts; ++i) {
        float a1 =
            (startAngle + half + arcSpan * (i / (float)pts)) * 3.14159f / 180.f;
        float a2 = (startAngle + half + arcSpan * ((i + 1) / (float)pts)) *
                   3.14159f / 180.f;
        shape[i * 3 + 0] = sf::Vertex{pos, color};
        shape[i * 3 + 1] = sf::Vertex{
            pos + sf::Vector2f(std::cos(a1) * r, std::sin(a1) * r), color};
        shape[i * 3 + 2] = sf::Vertex{
            pos + sf::Vector2f(std::cos(a2) * r, std::sin(a2) * r), color};
      }
      window.draw(shape);
    }
  } else {
    float t = (progress - 0.95f) / 0.05f;
    float sparkR = 3.0f * (1.0f - t);
    if (sparkR > 0.2f) {
      sf::RectangleShape spark({2.0f, sparkR * 4.0f});
      spark.setOrigin({1.0f, sparkR * 2.0f});
      spark.setPosition(pos);
      spark.setFillColor(sf::Color(255, 255, 200, (uint8_t)(255 * (1.0f - t))));
      window.draw(spark);
    }
  }
}
