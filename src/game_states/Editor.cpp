#include "AssetsManager.h"

#include "game_states/Editor.h"
#include "game_states/Start.h"

namespace city::state
{
Editor::Editor() :
	m_leaveState{false},
	m_zoom{1.0f},
	m_last_mouse_position{0.0f, 0.0f},
	m_view_state{ViewState::none}
{
	// Create the label.
	m_label = sfg::Label::Create("Editor state");

	// Create a simple button and connect the click signal.
	m_button = sfg::Button::Create("Back to Main menu");
	m_button->GetSignal(sfg::Widget::OnLeftClick)
	        .Connect([&]
	{
		m_label->SetText( "Leaving..." );
		m_leaveState = true;
	});

	// Create a vertical box layouter with 5 pixels spacing and add the label
	// and button to it.
	m_box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
	m_box->Pack( m_label );
	m_box->Pack( m_button, false );

	// Create a window and add the box layouter to it. Also set the window's title.
	m_ui_window = sfg::Window::Create();
	m_ui_window->SetTitle( "Menu" );
	m_ui_window->Add( m_box );

	auto size = m_world.gameDimension();
	m_view.setCenter(size);

}

void
Editor::update(Game& game, const float dt)
{
	if (m_leaveState)
	{
		game.back_to_previous_state();
		game.switch_to_state<Start>();
	}
}

void
Editor::draw(Window& window, const float dt)
{
	window.renderer().setView(m_view);
	// Create a desktop and add the window to it.
	window.desktop().Add( m_ui_window );
	m_world.draw(window, dt);
}

void
Editor::on_mouse_button_pressed(int button, float x, float y)
{
	if (button == sf::Mouse::Middle)
	{
		m_view_state = ViewState::pan;
		m_last_mouse_position = sf::Vector2f(x, y);
	}
}

void
Editor::on_mouse_button_pressed(int button, const EventListener::MousePixelToCoords& p2c)
{
	if (button == sf::Mouse::Left)
	{
		auto pos = p2c(m_view);
		m_world.regionInfo(pos);
	}
}

void
Editor::on_mouse_button_released(int button, float, float)
{
	if (button == sf::Mouse::Middle)
	{
		m_view_state = ViewState::none;
	}
}

void
Editor::on_mouse_moved(float x, float y)
{
	if (m_view_state == ViewState::pan)
	{
		auto anchor = m_last_mouse_position;
		m_last_mouse_position = sf::Vector2f{x, y};
		m_view.move(-1.0f * (m_last_mouse_position - anchor) * m_zoom);
	}
}

void
Editor::on_mouse_wheel(float delta)
{
	constexpr double zoom_factor = 1.6;
	constexpr double inv_zoom_factor = 1.0 / zoom_factor;
	if (delta < 0.0f)
	{
		m_view.zoom(zoom_factor);
		m_zoom *= zoom_factor;
	}
	else
	{
		m_view.zoom(inv_zoom_factor);
		m_zoom *= inv_zoom_factor;
	}
}

}
