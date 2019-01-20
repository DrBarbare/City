#ifndef CITY_GAME_SIZE_H
#define CITY_GAME_SIZE_H

#include <cstddef>

namespace city::geometry
{
class Size
{
public:
    constexpr Size() : m_width{}, m_height{} {}
    constexpr Size(std::size_t width, std::size_t height) : m_width{width}, m_height{height} {}

    constexpr const auto& width() const noexcept { return m_width; }
    constexpr auto& width() noexcept { return m_width; }
    constexpr const auto& height() const noexcept { return m_height; }
    constexpr auto& height() noexcept { return m_height; }

    constexpr bool operator==(const Size& rhs) const noexcept
    {
        return height() == rhs.height() && width() == rhs.width();
    }

    constexpr bool operator!=(const Size& rhs) const noexcept
    {
        return !(*this == rhs);
    }

    constexpr std::size_t area() const noexcept
    {
        return width() * height();
    }

private:
    std::size_t m_width;
    std::size_t m_height;
};
}

#endif // CITY_GAME_SIZE_H