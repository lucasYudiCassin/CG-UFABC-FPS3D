#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class State { Playing, GameOver };

struct GameData {
  State m_state{State::Playing};
} __attribute__((aligned(4)));

#endif