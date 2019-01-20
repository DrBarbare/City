#ifndef CITY_GAME_FLOODFILL_H
#define CITY_GAME_FLOODFILL_H

#include <vector>
#include <utility>
#include <cstdint>
#include <functional>
#include "algorithms/Matrix.h"

namespace city::algorithms
{

template <typename T>
class FloodFill
{
public:
	using element_type     = std::remove_cv_t<T>;
	using RegionPoints     = std::vector<std::pair<int, int> >;
	using BelongsCondition = std::function<bool(const element_type&, const element_type&)>;
	using InputType        = Matrix<element_type>;

	FloodFill(const InputType& input,
	          BelongsCondition cond)
		: m_cond{std::move(cond)},
		  m_input{std::cref(input)},
		  m_target{std::cref(input.at(0,0))},
		  m_visited(input.size(), std::uint8_t{0u})
	{}

	RegionPoints
	operator()(std::size_t x, std::size_t y)
	{
		return this->operator()(m_input.get().at(x, y), x, y);
	}

	RegionPoints
	operator()(const T& target, std::size_t x, std::size_t y)
	{
		m_target = std::cref(target);
		m_visited.fill(false);

		implementation(x, y);

		return std::move(m_region);
	}

private:
	BelongsCondition m_cond;

	std::reference_wrapper<const InputType> m_input;
	std::reference_wrapper<const element_type> m_target;
	Matrix<std::uint8_t> m_visited;
	RegionPoints m_region;

	void
	implementation(std::size_t x, std::size_t y)
	{
		if (m_input.get().check_coordinates(x, y)
		    && !m_visited.at(x, y)
		    && m_cond(m_input.get().at(x, y), m_target))
		{
			m_visited.at(x, y) = 1u;

			m_region.emplace_back(x, y);
			implementation(x - 1, y    );
			implementation(x    , y - 1);
			implementation(x - 1, y - 1);
			implementation(x + 1, y    );
			implementation(x    , y + 1);
			implementation(x + 1, y + 1);
			implementation(x + 1, y - 1);
			implementation(x - 1, y + 1);
		}
	}

};

}

#endif // CITY_GAME_FLOODFILL_H
