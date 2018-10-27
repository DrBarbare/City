#include "Animation.h"

namespace city
{
Animation::Animation(std::size_t start_offset,
	                 std::size_t number_frames,
	                 float frame_duration
	) :
	m_current_frame{start_offset},
	m_start_offset{start_offset},
	m_number_frames{number_frames},
	m_frame_duration{frame_duration},
	m_accumulated_time{0.0f}
{ }

std::size_t
Animation::getOffset(float dt_s)
{
	m_accumulated_time += dt_s;
	std::size_t next_frame = m_accumulated_time / m_frame_duration;

	if (next_frame != m_current_frame)
	{
		next_frame %= m_number_frames;

		if (next_frame == 0)
		{
			next_frame = m_start_offset;
			m_accumulated_time = 0;
		}
	}

	return next_frame;
}

}
