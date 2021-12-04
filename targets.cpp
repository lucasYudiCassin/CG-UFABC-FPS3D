#include "targets.hpp"

#include < chrono >
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core.h"

void Targets::initializeTarget() {
  // Start pseudo-random number generator
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  m_targets.clear();
  m_targetsArray = {{.m_position = {0.0f, 0.0f, 0.5f}, .m_isDraw = false}};
};

void Targets::targetAddedEvent(Targets::Target target) {
  m_targets.push_back(target);
};

void Targets::targetHittedEvent(Targets::Target &target, double elapsedTime) {
  target.m_hit = true;
  target.m_elapsedTime = elapsedTime;
};

Targets::Target Targets::createTargetEvent(double elapsedTime) {
  return Target{
      .m_elapsedTime = elapsedTime,
      .m_translation = glm::vec3{0.0f},
      .m_hit = false,
  };
};
