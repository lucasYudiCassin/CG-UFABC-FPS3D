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
 public:
  void initializeCamera();

  void computeViewMatrix();
  void computeProjectionMatrix(int width, int height);

  void dolly(float speed);
  void truck(float speed);
  void pan(float speed);
  void tilt(float speed);

 private:
  friend OpenGLWindow;

  double timeElapsed;

  struct Target {
    int m_position;
  } __attribute__((aligned(4)));

  std::list<int> m_targets{};

  const std::array<glm::vec3, 9> allowedTranslations{
      glm::vec3{0.3f, 0.3f, 0.0f}, glm::vec3{0.3f, 0.6f, 0.0f},
      glm::vec3{0.3f, 0.9f, 0.0f}, glm::vec3{0.6f, 0.3f, 0.0f},
      glm::vec3{0.6f, 0.6f, 0.0f}, glm::vec3{0.6f, 0.9f, 0.0f},
      glm::vec3{0.9f, 0.3f, 0.0f}, glm::vec3{0.9f, 0.6f, 0.0f},
      glm::vec3{0.9f, 0.9f, 0.0f},
  };

  std::default_random_engine m_randomEngine;
  std::uniform_int_distribution<int> m_randomDist{0, 8};

  const int SIMULTANEOUS_TARGETS = 4;

  void restart();
  static Targets::Target createTargetEvent();
  void addTarget();
  void removeTarget(int position);
};

#endif