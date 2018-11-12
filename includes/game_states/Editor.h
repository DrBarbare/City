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
		pan
	};

	bool m_leaveState;

	sf::View m_view;
	float m_zoom;
	sf::Vector2f m_last_mouse_position;
	ViewState m_view_state;

	sfg::Button::Ptr m_button;
	sfg::Label::Ptr m_label;
	sfg::Box::Ptr m_box;
	sfg::Window::Ptr m_ui_window;

	World m_world;

public:
	Editor();
	void update(Game& game, const float dt) override;
	void draw(Window& window, const float dt) override;


	void on_mouse_button_pressed(int button, float x, float y) override;
	void on_mouse_button_pressed(int button, const EventListener::MousePixelToCoords& p2c) override;
	void on_mouse_button_released(int button, float x, float y) override;
	void on_mouse_moved(float x, float y) override;
	void on_mouse_wheel(float delta) override;
};
}

#endif // CITY_GAME_STATE_EDITOR_H
