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

void Targets::addTarget(int exclude) {
  while (m_targets.size() < SIMULTANEOUS_TARGETS) {
    // generate random position and add to m_targets.
    const int position = m_randomDist(m_randomEngine);
    auto positionIsFilled = std::find(m_targets.begin(), m_targets.end(),
                                      position) != m_targets.end();

    if (position == exclude || positionIsFilled) {
      // fmt::print("not inserted {}\n", position);
      continue;
    }

    // fmt::print("inserted {}\n", position);
    // fmt::print("positions = {}\n", m_targets.size());
    // fmt::print("positionIsFilled = {}\n", positionIsFilled);

    m_targets.push_back(position);
  }
}

void Targets::removeTarget(int position) {
  m_targets.remove(position);
  addTarget(position);
}
