#ifndef CITY_GAME_MATRIX_H
#define CITY_GAME_MATRIX_H

#include <vector>

#include "geometry/Size.h"

namespace city::algorithms
{
template<typename T>
class Matrix
{
public:
	using element_type = T;
	using value_type   = std::remove_cv_t<element_type>;
	using size_type    = std::size_t;

	constexpr Matrix() = default;
	constexpr Matrix(geometry::Size size)
		: m_size{std::move(size)}, m_data(m_size.area()) {}

	constexpr Matrix(geometry::Size size, T* values)
		: m_size{std::move(size)}, m_data(m_size.area(), values) {}
	constexpr Matrix(geometry::Size size, T value)
		: m_size{std::move(size)}, m_data(m_size.area(), std::move(value)) {}

	void fill(size_type width, size_type height, T value)
	{
		m_size = Size{width, height};
		m_data = std::vector<value_type>(m_size.area(), std::move(value));
	}

	void fill(T value)
	{
		m_data = std::vector<value_type>(area(), std::move(value));
	}

	const T& at(size_type x, size_type y) const
	{
		auto p = position(x, y);
		if (p > area())
		{
			throw std::out_of_range("Position outiside of Matrix");
		}
		return m_data[p];
	}

	T& at(size_type x, size_type y)
	{
		auto p = position(x, y);
		if (p > area())
		{
			throw std::out_of_range("Position outiside of Matrix");
		}
		return m_data[p];
	}

	constexpr size_type width() const noexcept
	{
		return m_size.width();
	}

	constexpr size_type height() const noexcept
	{
		return m_size.height();
	}

	constexpr const geometry::Size& size() const noexcept
	{
		return m_size;
	}

	constexpr size_type area() const noexcept
	{
		return m_size.area();
	}

	constexpr auto begin() noexcept
	{
		return m_data.begin();
	}

	constexpr auto cbegin() const noexcept
	{
		return m_data.cbegin();
	}

	constexpr auto end() noexcept
	{
		return m_data.end();
	}

	constexpr auto end() const noexcept
	{
		return m_data.cend();
	}

	template<typename F>
	void for_each(F&& f)
	{
		for (size_type i = 0; i < area(); ++i)
		{
			const auto [x, y] = position(i);
			f(x, y, m_data[i]);
		}
	}

	constexpr bool check_coordinates(size_type x, size_type y) const
	{
		return x < m_size.width() && y < m_size.height();
	}

private:
	size_type position(size_type x, size_type y) const noexcept
	{
		return m_size.height() * y + x;
	}
	std::pair<size_type, size_type> position(size_type i) const noexcept
	{
		return {i % m_size.width(), i / m_size.width()};
	}

	geometry::Size m_size;
	std::vector<value_type> m_data;
};

}
#endif // CITY_GAME_MATRIX_H
