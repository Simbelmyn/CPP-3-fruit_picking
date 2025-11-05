#include <ostream>
#include <utility>

constexpr enum class Taste { SWEET, SOUR };
constexpr enum class Size { LARGE, MEDIUM, SMALL };
constexpr enum class Quality { HEALTHY, ROTTEN, WORMY };

std::ostream& operator <<(std::ostream& os, const Taste& taste) {
    switch (taste) {
        case Taste::SWEET: os << "słodki"; break;
        case Taste::SOUR:  os << "kwaśny"; break;
    }
    return os;
}

std::ostream& operator <<(std::ostream& os, const Size& size) {
    switch (size) {
        case Size::LARGE: os << "duży"; break;
        case Size::MEDIUM: os << "średni"; break;
        case Size::SMALL: os << "mały"; break;
    }
    return os;
}

std::ostream& operator <<(std::ostream& os, const Quality& quality) {
    switch (quality) {
        case Quality::HEALTHY: os << "zdrowy"; break;
        case Quality::ROTTEN: os << "nadgniły"; break;
        case Quality::WORMY: os << "robaczywy"; break;
    }
    return os;
}

class Fruit {
private:
    Taste _taste;
    Size _size;
    Quality _quality;
public:
    constexpr Fruit(Taste taste, Size size, Quality quality)
        : _taste(taste)
        , _size(size)
        , _quality(quality)
    {}

    constexpr Fruit(const Fruit& fruit) = default;
    constexpr Fruit(Fruit&& fruit) = default;

    explicit constexpr Fruit(const std::tuple<Taste, Size, Quality>& tuple)
        : _taste(std::get<0>(tuple))
        , _size(std::get<1>(tuple))
        , _quality(std::get<2>(tuple))
    {}

    explicit constexpr operator std::tuple<Taste, Size, Quality>() const {
        return std::make_tuple(_taste, _size, _quality);
    }

    constexpr ~Fruit() = default;

    constexpr Fruit& operator =(const Fruit& ) = default;
    constexpr Fruit& operator =(Fruit&& ) = default;
    
    

    constexpr bool operator ==(const Fruit& other) const {
        return _taste == other.taste() 
               and _size == other.size()
               and _quality == other.quality();
    }

    void make_rotten() { 
        if (_quality == Quality::HEALTHY) {
            _quality = Quality::ROTTEN;
        }
    }

    void make_wormy() { 
        if (_quality == Quality::HEALTHY) {
            _quality = Quality::WORMY;
        }
    }

    constexpr Taste taste() const { return _taste; }
    constexpr Size size() const { return _size; }
    constexpr Quality quality() const { return _quality; }
};

constexpr Fruit YUMMY_ONE(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
constexpr Fruit ROTTY_ONE(Taste::SOUR, Size::SMALL, Quality::ROTTEN);

std::ostream& operator <<(std::ostream& os, const Fruit& fruit) {
    os << "[ " << fruit.taste() << " " << fruit.size()
       << " " << fruit.quality() << " ]";
    return os;
}