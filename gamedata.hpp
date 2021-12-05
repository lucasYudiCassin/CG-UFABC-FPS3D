#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class State { Playing, GameOver };

struct GameData {
  State m_state{State::Playing};
  int m_score{0};
  int m_shots{0};
} __attribute__((aligned(4)));

#endif