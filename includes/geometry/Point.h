#ifndef CITY_GAME_POINT_H
#define CITY_GAME_POINT_H

#include <cstddef>

namespace city::geometry
{
class Point
{
public:
    constexpr Point() : m_x{}, m_y{} {}
    constexpr Point(std::size_t x, std::size_t y) : m_x{x}, m_y{y} {}

    constexpr const auto& x() const noexcept { return m_x; }
    constexpr auto& x() noexcept { return m_x; }
    constexpr const auto& y() const noexcept { return m_y; }
    constexpr auto& y() noexcept { return m_y; }

    constexpr bool operator==(const Point& rhs) const noexcept
    {
        return y() == rhs.y() && x() == rhs.x();
    }

    constexpr bool operator!=(const Point& rhs) const noexcept
    {
        return !(*this == rhs);
    }

private:
    std::size_t m_x;
    std::size_t m_y;
};
}

#endif // CITY_GAME_POINT_H