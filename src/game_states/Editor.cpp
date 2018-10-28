#include "AssetsManager.h"

#include "game_states/Editor.h"
#include "game_states/Start.h"

namespace city::state
{
Editor::Editor() : m_leaveState{false}
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
	// Create a desktop and add the window to it.
	window.desktop().Add( m_ui_window );
	m_world.draw(window, dt);
}



}
