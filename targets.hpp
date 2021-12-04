#ifndef TARGET_HPP_
#define TARGET_HPP_

#include <array>
#include <glm/fwd.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <list>
#include <random>

class OpenGLWindow;

class Targets {
 public:
  void initializeTarget();

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
    double m_elapsedTime;
    glm::vec3 m_translation;
    bool m_hit{false};
  } __attribute__((aligned(32)));

  std::list<Target> m_targets;
  struct TargetPositions {
    glm::vec3 m_position;
    bool m_isDraw;
  } __attribute__((aligned(32)));

  std::array<glm::vec3, 9> allowedTranslations{
      glm::vec3{0.0f, 0.3f, 0.3f}, glm::vec3{0.0f, 0.6f, 0.3f},
      glm::vec3{0.0f, 0.9f, 0.3f}, glm::vec3{0.0f, 0.3f, 0.6f},
      glm::vec3{0.0f, 0.6f, 0.6f}, glm::vec3{0.0f, 0.9f, 0.6f},
      glm::vec3{0.0f, 0.3f, 0.9f}, glm::vec3{0.0f, 0.6f, 0.9f},
      glm::vec3{0.0f, 0.9f, 0.9f},
  };

  std::list<TargetPositions> m_targetsArray;

  std::default_random_engine m_randomEngine;
  std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};

  void initializeTargets();
  static Targets::Target createTargetEvent(double elapsedTime);
  void targetAddedEvent(Targets::Target target);
  static void targetHittedEvent(Targets::Target &target, double elapsedTime);
};

#endif