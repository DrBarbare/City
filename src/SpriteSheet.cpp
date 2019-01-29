#include <stdexcept>
#include "SpriteSheet.h"

namespace city
{

SpriteSheet::SpriteSheet(const std::filesystem::path& file,
                         sf::IntRect initial_frame) :
	m_current_animation{0},
	m_initial_frame{std::move(initial_frame)}
{
	if (!m_texture.loadFromFile(file.string()))
	{
		throw std::runtime_error("Could not load image");
	}
	m_sprite.setTexture(m_texture);
	m_sprite.setTextureRect(initial_frame);
}

const sf::Sprite&
SpriteSheet::next(float dt_s)
{
	if (!m_animations.empty())
	{
		auto rect = m_initial_frame;
		auto frame_number = m_animations[m_current_animation].getOffset(dt_s);
		rect.left += rect.width * frame_number;
		rect.top += rect.height * m_current_animation;
		m_sprite.setTexture(m_texture);
		m_sprite.setTextureRect(rect);
	}
	return current_sprite();
}

const sf::Sprite&
SpriteSheet::current_sprite() const
{
	return m_sprite;
}

std::size_t
SpriteSheet::get_animation() const
{
	return m_current_animation;
}

void
SpriteSheet::set_animation(std::size_t animation)
{
	if (animation >= m_animations.size()) throw std::out_of_range("Animation queried does not exist.");
	m_current_animation = animation;
}
}
