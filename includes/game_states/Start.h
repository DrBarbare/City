#ifndef CITY_GAME_STATE_START_H
#define CITY_GAME_STATE_START_H

#include "GameState.h"

namespace city::state
{
class Start : public GameState
{
	bool m_startEditor;
	sfg::Button::Ptr m_button;
	sfg::Label::Ptr m_label;
	sfg::Box::Ptr m_box;
	sfg::Window::Ptr m_ui_window;

public:
	Start();
	void update(Game& game, const float dt) override;
	void draw(Window& window, const float dt) override;
};
}

#endif // CITY_GAME_STATE_START_H
