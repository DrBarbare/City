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
	auto label = sfg::Label::Create("You are in\nEditor state\nSelect a tile\nand put it down!");

	// Create a simple button and connect the click signal.
	auto m_button = sfg::Button::Create("Back to Main menu");
	m_button->GetSignal(sfg::Widget::OnLeftClick)
	          .Connect([this, label]
	{
		label->SetText( "Leaving..." );
		m_leaveState = true;
	});

	auto table = sfg::ListBox::Create();
	table->AppendItem(sf::String{"Nothing"});
	table->SetSelection(0);
	for (auto tile : AssetsManager::tiles())
	{
		table->AppendItem(sf::String{tile.first});
	}

	m_getSelectedTileName = [table]() { return table->GetSelectedItemText(); };

	// Create a vertical box layouter with 5 pixels spacing and add the label
	// and button to it.
	auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
	box->Pack(table);
	box->Pack(label);
	box->Pack(m_button, false);

	// Create a window and add the box layouter to it. Also set the window's title.
	auto ui_window = sfg::Window::Create(sfg::Window::Style::BACKGROUND);
	ui_window->SetTitle("Menu");
	ui_window->Add(box);

	m_draw_ui = [ui_window=std::move(ui_window)](Window& window, const float) {
		auto window_shape = window.shape();
		auto allocation = ui_window->GetAllocation();

		if (allocation.left == 0.0f && allocation.top == 0.0f)
		{
			auto width = allocation.width * 1.33f;
			ui_window->SetAllocation(sf::FloatRect{
				window_shape.x - width,
				0.0f,
				width,
				window_shape.y
			});
		}

		window.desktop().Add( ui_window );
	};

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

std::tuple<std::size_t, std::size_t, std::size_t, std::size_t>
Editor::clampSelectionWorld()
{
	std::size_t x0 = m_selection_start.x;
	std::size_t y0 = m_selection_start.y;
	std::size_t x1 = std::clamp<float>(m_selection_position.x, 0.0f, m_world.width()  - 1);
	std::size_t y1 = std::clamp<float>(m_selection_position.y, 0.0f, m_world.height() - 1);
	if (x0 > x1) std::swap(x0, x1);
	if (y0 > y1) std::swap(y0, y1);

	return std::make_tuple(x0, y0, x1, y1);
}

World::point_condition
Editor::selectionCondition()
{
	if (m_view_state == ViewState::select
	    && m_selection_start.x >= 0.0f && m_selection_start.y >= 0.0f
		&& m_selection_start.x < m_world.width() && m_selection_start.y < m_world.height())
	{
		auto [x0, y0, x1, y1] = clampSelectionWorld();
		return [x0, x1, y0, y1](std::size_t x, std::size_t y)
		{
			return x0 <= x && x <= x1 && y0 <= y && y <= y1;
		};
	}
	else
	{
		return [](std::size_t,std::size_t) { return false; };
	}
}

Tile
Editor::getSelectedTile() const
{
	Tile tile;
	auto tile_iterator = AssetsManager::tiles().find(m_getSelectedTileName());
	if (tile_iterator != AssetsManager::tiles().cend())
	{
		tile = tile_iterator->second;
	}
	return tile;
}

void
Editor::draw(Window& window, const float dt)
{
	window.renderer().setView(m_view);
	m_draw_ui(window, dt);
	m_world.draw(window, dt, selectionCondition());
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
Editor::on_mouse_button_pressed(int button, const EventListener::MousePixelToCoords& p2c)
{
	auto pos = m_world.viewCoordsToWorld(p2c(m_view));
	if (button == sf::Mouse::Right)
	{
		m_world.regionInfo(std::move(pos));
	}
	else if (button == sf::Mouse::Left)
	{	
		m_view_state = ViewState::select;
		m_selection_start = pos;
		m_selection_position = pos;
	}
}

void
Editor::on_mouse_button_released(int button, const EventListener::MousePixelToCoords& p2c)
{
	//auto pos = m_world.viewCoordsToWorld(p2c(m_view));
	if (m_view_state == ViewState::select && button == sf::Mouse::Left)
	{
		auto cond = selectionCondition();
		if (cond(m_selection_position.x, m_selection_position.y))
		{
			auto [x0, y0, x1, y1] = clampSelectionWorld();
			m_world.setRegion(geometry::Point{x0, y0}, geometry::Point{x1, y1}, getSelectedTile());
		}
		m_view_state = ViewState::none;
	}
}

void
Editor::on_mouse_moved(const EventListener::MousePixelToCoords& p2c)
{
	auto pos = m_world.viewCoordsToWorld(p2c(m_view));
	if (m_view_state == ViewState::select)
	{
		m_selection_position = std::move(pos);
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
