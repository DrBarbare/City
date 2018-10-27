#ifndef CITY_GAME_SPRITESHEET_H
#define CITY_GAME_SPRITESHEET_H

#include <SFML/Graphics.hpp>

#include <filesystem>
#include <vector>

#include "Animation.h"

namespace city
{

class SpriteSheet
{
	std::size_t m_current_animation;
	std::vector<Animation> m_animations;

	sf::IntRect m_initial_frame;
	sf::Texture m_texture;
	sf::Sprite m_sprite;

public:

	SpriteSheet(const std::filesystem::path& file,
	            sf::IntRect initial_frame);

	void next_animation();
	const sf::Sprite& next(float dt_s);
	const sf::Sprite& current_sprite() const;

	template<typename... Args>
	void add_animation(Args&&... args)
	{
		m_animations.emplace_back(std::forward<Args>(args)...);
	}
};

}

#endif // CITY_GAME_SPRITESHEET_H
