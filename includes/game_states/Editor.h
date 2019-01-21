#ifndef CITY_GAME_STATE_EDITOR_H
#define CITY_GAME_STATE_EDITOR_H

#include "GameState.h"
#include "World.h"

namespace city::state
{
class Editor : public GameState
{
	enum class ViewState
	{
		none,
		pan,
		select
	};

	bool m_leaveState;

	World m_world;

	sf::View m_view;
	float m_zoom;
	sf::Vector2f m_last_mouse_position;
	ViewState m_view_state;

	std::function<void(Window&, const float dt)> m_draw_ui;
	std::function<sf::String()> m_getSelectedTileName;
	Tile getSelectedTile() const;

	sf::Vector2f m_selection_start;
	sf::Vector2f m_selection_position;
	World::point_condition selectionCondition();

	Tile m_current_tile_type;

	std::tuple<std::size_t, std::size_t, std::size_t, std::size_t> clampSelectionWorld();

public:
	Editor();
	void update(Game& game, const float dt) override;
	void draw(Window& window, const float dt) override;


	void on_mouse_wheel(float delta) override;
	void on_mouse_button_pressed(int button, float x, float y) override;
	void on_mouse_button_released(int button, float x, float y) override;
	void on_mouse_moved(float x, float y) override;
	void on_mouse_button_pressed(int button, const EventListener::MousePixelToCoords& p2c) override;
	void on_mouse_button_released(int button, const EventListener::MousePixelToCoords& p2c) override;
	void on_mouse_moved(const EventListener::MousePixelToCoords& p2c) override;
};
}

#endif // CITY_GAME_STATE_EDITOR_H
