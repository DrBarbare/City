#include "Atlas.h"

namespace city
{

Atlas::Atlas(const std::filesystem::path& file,
             sf::IntRect initial_frame) :
	m_initial_frame{std::move(initial_frame)}
{
	m_texture.loadFromFile(file);
	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(initial_frame);
}

const sf::Sprite&
Atlas::next(float dt_s)
{
	if (!m_animations.empty())
	{
		auto rect = m_initial_frame;
		auto frame_number = m_animations[m_current_animation].getOffset(dt_s);
		rect.left = rect.width * frame_number;
		rect.top = rect.height * m_current_animation;

		m_sprite.setTextureRect(rect);
	}
	return current_sprite();
}

const sf::Sprite&
Atlas::current_sprite() const
{
	return m_sprite;
}

void
Atlas::next_animation()
{
	m_current_animation = ++m_current_animation % m_animations.size();
}
}
