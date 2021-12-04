#include "targets.hpp"

#include <algorithm>
#include <cstdio>
#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core.h"

void Targets::restart() {
  m_targets.clear();

  // Start pseudo-random number generator
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  addTarget();
}

void Targets::addTarget() {
  int i = 0;
  while (i < SIMULTANEOUS_TARGETS) {
    // generate random position and add to m_targets.
    const int position = m_randomDist(m_randomEngine);
    // std::list<int> positions;

    // std::transform(m_targets.begin(), m_targets.end(),
    //                std::back_inserter(positions),
    //                [](Target target) { return target.m_position; });

    // auto positionIsFilled = std::find(positions.cbegin(), positions.cend(),
    //                                   position) != positions.cend();

    // auto positionIsFilled = std::find(m_targets.begin(), m_targets.end(),
    //                                   position) != m_targets.end();

    // if (positionIsFilled) {
    m_targets.push_back(position);
    // }
    i++;
  }
}

void Targets::removeTarget(int position) {
  m_targets.remove(position);
  addTarget();
}
