#ifndef FRUIT_PICKING_H
#define FRUIT_PICKING_H

#include <algorithm>
#include <list>
#include <ostream>
#include <utility>
#include <vector>

enum class Taste { SWEET, SOUR };
enum class Size { LARGE, MEDIUM, SMALL };
enum class Quality { HEALTHY, ROTTEN, WORMY };

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

    void go_rotten() { 
        if (_quality == Quality::HEALTHY) {
            _quality = Quality::ROTTEN;
        }
    }

    void become_worm_infested() { 
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

class Picker { // temporary
public:
    bool operator ==(const Picker& other) const = default; // temporary
    bool operator <(const Picker& other) const { return true; } // temporary
    bool operator <=(const Picker& other) const { return true; } // temporary
};

// WAŻNE - zakładam, że picker_a <= picker_b implikuje, ze picker_b bedzie
//         wyżej w rankingu niż picker_a!
class Ranking { 
private:
    std::vector<Picker> _ranking;
public:
    constexpr Ranking() : _ranking(std::vector<Picker>()) {}

    constexpr Ranking(const std::initializer_list<Picker>& list) 
        : _ranking(list)
    {}
    constexpr Ranking(std::initializer_list<Picker>&& list) 
        : _ranking(list) 
    {}

    constexpr Ranking(const Ranking& ranking) = default;
    constexpr Ranking(Ranking&& ranking) = default;

    constexpr ~Ranking() = default;

    constexpr Ranking& operator=(const Ranking& other) = default;
    constexpr Ranking& operator=(Ranking&& other) = default;

    constexpr void operator +=(const Picker& picker) {
        auto it = _ranking.begin();
        while (it != _ranking.end() and picker < *it) ++it;
        _ranking.insert(it, picker);
    }

    constexpr void operator +=(Picker&& picker) {
        auto it = _ranking.begin();
        while (it != _ranking.end() and picker < *it) ++it;
        _ranking.insert(it, std::move(picker)); 
    }

    constexpr void operator +=(const Ranking& other) {
        std::vector<Picker> new_ranking;
        auto it1 = _ranking.begin();
        auto it2 = other._ranking.begin();

        while (it1 != _ranking.end() and it2 != other._ranking.end()) {
            if (*it1 < *it2) {
                new_ranking.emplace_back(*it2++);
            }
            else {
                new_ranking.emplace_back(std::move(*it1++));
            }
        }
        std::move(it1, _ranking.end(), std::back_inserter(new_ranking));
        new_ranking.insert(new_ranking.end(), it2, other._ranking.end());

        _ranking = std::move(new_ranking);
    }

    constexpr void operator +=(Ranking&& other) {
        std::vector<Picker> new_ranking;
        auto it1 = _ranking.begin();
        auto it2 = other._ranking.begin();

        while (it1 != _ranking.end() and it2 != other._ranking.end()) {
            if (*it1 < *it2) {
                new_ranking.emplace_back(std::move(*it2++));
            }
            else {
                new_ranking.emplace_back(std::move(*it1++));
            }
        }
        std::move(it1, _ranking.end(), std::back_inserter(new_ranking));
        std::move(it2, other._ranking.end(), std::back_inserter(new_ranking));

        _ranking = std::move(new_ranking);
    }

};

#endif