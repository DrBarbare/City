#ifndef CITY_GAME_ANIMATION_H
#define CITY_GAME_ANIMATION_H

#include <cstddef>

namespace city
{

class Animation
{
	std::size_t m_current_frame;
	std::size_t m_start_offset;
	std::size_t m_number_frames;
	float m_frame_duration;
	float m_accumulated_time;

public:
	Animation(std::size_t start_offset,
	          std::size_t number_frames,
	          float frame_duration);

	std::size_t getOffset(float dt_s);
};

}
#endif // CITY_GAME_ANIMATION_H
