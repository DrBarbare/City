#include "Window.h"
#include <SFML/System.hpp>

#include "AssetsManager.h"

namespace city
{

void
Window::resize_background(std::size_t width, std::size_t height)
{
	// Radjust background to scene
	auto view = m_window.getDefaultView();
	view.setSize(width, height);
	m_background.setPosition(m_window.mapPixelToCoords(sf::Vector2i(0, 0), view));
	m_window.setView(view);

	// Resize background
	const auto& bg_size = m_background.getTexture()->getSize();
	float scaleX = float(width)  / float(bg_size.x);
	float scaleY = float(height) / float(bg_size.y);
	m_background.setScale(scaleX, scaleY);
}

bool
Window::poll_events(EventListener* listener)
{
	sf::Event event;
	while(m_window.pollEvent(event))
	{
		m_desktop.HandleEvent(event);
		switch(event.type)
		{
		/* Close the window */
		case sf::Event::Closed:
		{
			listener->on_closed();
			m_window.close();
			return false;
		}
		/* Resize the window */
		case sf::Event::Resized:
		{
			resize_background(event.size.width, event.size.height);
			listener->on_resize();
			break;
		}
		case sf::Event::KeyPressed:
		{
			//on_keypressed();
			//if(event.key.code == sf::Keyboard::Escape) this->game->window.close();
			break;
		}
		case sf::Event::MouseMoved:
		{
			auto pos = sf::Mouse::getPosition(m_window);
			listener->on_mouse_moved(pos.x, pos.y);
			listener->on_mouse_moved([this, pos = std::move(pos)](const sf::View& view)
					{
						return m_window.mapPixelToCoords(pos, view);
					});
			break;
		}
		case sf::Event::MouseButtonPressed:
		{
			auto pos = sf::Mouse::getPosition(m_window);
			listener->on_mouse_button_pressed(event.mouseButton.button, pos.x, pos.y);
			listener->on_mouse_button_pressed(event.mouseButton.button,
			        [this, pos = std::move(pos)](const sf::View& view)
					{
						return m_window.mapPixelToCoords(pos, view);
					});
			break;
		}
		case sf::Event::MouseButtonReleased:
		{
			auto pos = sf::Mouse::getPosition(m_window);
			listener->on_mouse_button_released(event.mouseButton.button, pos.x, pos.y);
			listener->on_mouse_button_released(event.mouseButton.button,
			        [this, pos = std::move(pos)](const sf::View& view)
					{
						return m_window.mapPixelToCoords(pos, view);
					});
			break;
		}
		case sf::Event::MouseWheelMoved:
		{
			listener->on_mouse_wheel(event.mouseWheel.delta);
		}
		default: break;
		}
	}
	return true;
}

Window::Window() :
	m_dt_s{0}
{
	m_window.create(sf::VideoMode(1200, 800), "City");
	m_window.setFramerateLimit(30);
	m_background.setTexture(AssetsManager::load(assets::Textures::background));
	m_font.loadFromFile(AssetsManager::fontTTFFile("Lato", "Regular").string());
	m_fps.setFont(m_font);
	m_fps.setCharacterSize(20); // in pixels, not points!
	m_fps.setFillColor(sf::Color::Red);
	m_fps.setStyle(sf::Text::Bold);
	m_desktop.LoadThemeFromFile(AssetsManager::themeFile().string());
}

float
Window::dt_s() const
{
	return m_dt_s;
}

void
Window::display(const std::function<void()>& draw)
{
	m_clock.restart();

	// TODO: push clear as a limiter?
	m_window.clear(sf::Color::Black);
	m_window.resetGLStates();
	m_desktop.RemoveAll();

	//m_window.pushGLStates();
	m_window.draw(m_background);
	m_fps.setString(std::to_string(1.0 / dt_s()));
	m_window.draw(m_fps);
	auto view = m_window.getDefaultView();
	// Draw UI element
	draw();
	//m_window.popGLStates();
	m_window.setView(view);

	m_ui.Display( m_window );

	m_window.display();

	// Store time since last drawing call
	auto elapsed = m_clock.restart();
	m_dt_s = elapsed.asSeconds();
	m_desktop.Update( dt_s() );

}


};
