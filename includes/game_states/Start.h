#ifndef CITY_GAME_STATE_START_H
#define CITY_GAME_STATE_START_H

#include "GameState.h"

namespace city::state
{
class Start : public GameState
{
	bool m_startEditor;
	std::function<void(Window&, const float dt)> m_draw_ui;

public:
	Start();
	void update(Game& game, const float dt) override;
	void draw(Window& window, const float dt) override;
};
}

#endif // CITY_GAME_STATE_START_H
