#ifndef CITY_GAME_STATE_EDITOR_H
#define CITY_GAME_STATE_EDITOR_H

#include <vector>
#include "GameState.h"
#include "Tile.h"

namespace city::state
{
class Editor : public GameState
{
	bool m_leaveState;

	sfg::Button::Ptr m_button;
	sfg::Label::Ptr m_label;
	sfg::Box::Ptr m_box;
	sfg::Window::Ptr m_ui_window;

	std::vector<Tile> m_tiles;
public:
	Editor();
	void update(Game& game, const float dt) override;
	void draw(Window& window, const float dt) override;
};
}

#endif // CITY_GAME_STATE_EDITOR_H
