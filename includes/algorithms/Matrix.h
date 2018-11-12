#ifndef CITY_GAME_MATRIX_H
#define CITY_GAME_MATRIX_H

#include <vector>

namespace city::algorithms
{
template<typename T>
class Matrix
{
public:
	using element_type = T;
	using value_type   = std::remove_cv_t<element_type>;
	using size_type    = std::size_t;

	Matrix() = default;
	Matrix(std::size_t width, std::size_t height)
		: m_width{width}, m_height{height}, m_data(width*height) {}
	Matrix(std::size_t width, std::size_t height, T* values)
		: m_width{width}, m_height{height}, m_data(width*height, values) {}
	Matrix(std::size_t width, std::size_t height, T value)
		: m_width{width}, m_height{height}, m_data(width*height, std::move(value)) {}

	void fill(size_type width, size_type height, T value)
	{
		m_width = width;
		m_height = height;
		m_data = std::vector<value_type>(width * height, std::move(value));
	}

	void fill(T value)
	{
		m_data = std::vector<value_type>(size(), std::move(value));
	}

	const T& at(size_type x, size_type y) const
	{
		auto p = position(x, y);
		if (p > size())
		{
			throw std::out_of_range("Position outiside of Matrix");
		}
		return m_data[p];
	}

	T& at(size_type x, size_type y)
	{
		auto p = position(x, y);
		if (p > size())
		{
			throw std::out_of_range("Position outiside of Matrix");
		}
		return m_data[p];
	}

	size_type width() const noexcept
	{
		return m_width;
	}

	size_type height() const noexcept
	{
		return m_height;
	}

	size_type size() const noexcept
	{
		return m_width * m_height;
	}

	auto begin()
	{
		return m_data.begin();
	}

	auto end()
	{
		return m_data.end();
	}

	template<typename F>
	void for_each(F&& f)
	{
		for (size_type i = 0; i < size(); ++i)
		{
			const auto [x, y] = position(i);
			f(x, y, m_data[i]);
		}
	}

	bool check_coordinates(size_type x, size_type y) const
	{
		return x < m_width && y < m_height;
	}

private:
	size_type position(size_type x, size_type y) const noexcept
	{
		return m_height * y + x;
	}
	std::pair<size_type, size_type> position(size_type i) const noexcept
	{
		return {i % m_width, i / m_width};
	}

	size_type m_width;
	size_type m_height;
	std::vector<value_type> m_data;
};

}
#endif // CITY_GAME_MATRIX_H
