#ifndef TARGET_HPP_
#define TARGET_HPP_

#include <array>
#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <list>
#include <random>

#include "abcg.hpp"
#include "abcg_elapsedtimer.hpp"

class OpenGLWindow;

class Targets {
 private:
  friend OpenGLWindow;

  struct Target {
    int m_position;
  } __attribute__((aligned(4)));

  std::list<int> m_targets{};

  static const int TARGETS_QUANTITY = 15;

  const std::array<glm::vec3, TARGETS_QUANTITY> allowedTranslations{
      glm::vec3{-0.4f, 0.2f, 0.5f},  glm::vec3{-0.2f, 0.2f, 0.5f},
      glm::vec3{0.0f, 0.2f, 0.5f},   glm::vec3{0.2f, 0.2f, 0.5f},
      glm::vec3{0.4f, 0.2f, 0.5f},   glm::vec3{-0.4f, 0.0f, 0.5f},
      glm::vec3{-0.2f, 0.0f, 0.5f},  glm::vec3{0.0f, 0.0f, 0.5f},
      glm::vec3{0.2f, 0.0f, 0.5f},   glm::vec3{0.4f, 0.0f, 0.5f},
      glm::vec3{-0.4f, -0.2f, 0.5f}, glm::vec3{-0.2f, -0.2f, 0.5f},
      glm::vec3{0.0f, -0.2f, 0.5f},  glm::vec3{0.2f, -0.2f, 0.5f},
      glm::vec3{0.4f, -0.2f, 0.5f},
  };

  std::default_random_engine m_randomEngine;
  std::uniform_int_distribution<int> m_randomDist{0, TARGETS_QUANTITY - 1};

  static const int SIMULTANEOUS_TARGETS = 4;

  void restart();
  void addTarget(int exclude = -1);
  void removeTarget(int position);
};

#endif