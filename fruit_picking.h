#ifndef FRUIT_PICKING_H
#define FRUIT_PICKING_H

#include <algorithm>
#include <compare>
#include <list>
#include <ostream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

enum class Taste { SWEET, SOUR };
enum class Size { LARGE, MEDIUM, SMALL };
enum class Quality { HEALTHY, ROTTEN, WORMY };

inline std::ostream& operator<<(std::ostream& os, const Taste& taste) {
    switch (taste) {
        case Taste::SWEET: os << "słodki"; break;
        case Taste::SOUR:  os << "kwaśny"; break;
    }
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const Size& size) {
    switch (size) {
        case Size::LARGE: os << "duży"; break;
        case Size::MEDIUM: os << "średni"; break;
        case Size::SMALL: os << "mały"; break;
    }
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const Quality& quality) {
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

    constexpr Fruit(const Fruit&) = default;
    constexpr Fruit(Fruit&&) = default;

    explicit constexpr Fruit(const std::tuple<Taste, Size, Quality>& tuple)
        : _taste(std::get<0>(tuple))
        , _size(std::get<1>(tuple))
        , _quality(std::get<2>(tuple))
    {}

    explicit constexpr operator std::tuple<Taste, Size, Quality>() const {
        return std::make_tuple(_taste, _size, _quality);
    }

    constexpr ~Fruit() = default;

    constexpr Fruit& operator=(const Fruit&) = default;
    constexpr Fruit& operator=(Fruit&&) = default;

    constexpr bool operator==(const Fruit& other) const {
        return _taste == other._taste 
               && _size == other._size
               && _quality == other._quality;
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

inline std::ostream& operator<<(std::ostream& os, const Fruit& fruit) {
    os << "[ " << fruit.taste() << " " << fruit.size()
       << " " << fruit.quality() << " ]";
    return os;
}

class Picker {
private:
    std::string _name;
    std::list<Fruit> _fruits;

public:
    Picker(const std::string& name = "Anonim") 
        : _name(name.empty() ? "Anonim" : name)
        , _fruits()
    {}
    
    Picker(const Picker&) = default;
    Picker(Picker&&) = default;
    
    Picker& operator=(const Picker&) = default;
    Picker& operator=(Picker&&) = default;
    
    const std::string& get_name() const { return _name; }
    
    Picker& operator+=(const Fruit& fruit) {
        Fruit new_fruit = fruit;
        
        if (!_fruits.empty()) {
            Fruit& last = _fruits.back();
            
            // Jeśli nowy owoc jest zdrowy, a ostatni nadgniły → nowy gnije
            if (new_fruit.quality() == Quality::HEALTHY && 
                last.quality() == Quality::ROTTEN) {
                new_fruit.go_rotten();
            }
            // Jeśli nowy owoc jest nadgniły, a ostatni zdrowy → ostatni gnije
            else if (new_fruit.quality() == Quality::ROTTEN && 
                     last.quality() == Quality::HEALTHY) {
                last.go_rotten();
            }
        }
        
        // Jeśli nowy owoc jest robaczywy → wszystkie słodkie zdrowe stają się robaczywe
        if (new_fruit.quality() == Quality::WORMY) {
            for (auto& f : _fruits) {
                if (f.quality() == Quality::HEALTHY && 
                    f.taste() == Taste::SWEET) {
                    f.become_worm_infested();
                }
            }
        }
        
        _fruits.push_back(new_fruit);
        return *this;
    }
    
    Picker& operator+=(Fruit&& fruit) {
        return *this += fruit;
    }
    
    Picker& operator+=(Picker& other) {
        // Samemu sobie nie zabieramy
        if (this == &other) return *this;
        
        if (!other._fruits.empty()) {
            Fruit stolen = other._fruits.front();
            other._fruits.pop_front();
            *this += stolen;
        }
        return *this;
    }
    
    Picker& operator-=(Picker& other) {
        // Samemu sobie nie oddajemy
        if (this == &other) return *this;
        
        if (!_fruits.empty()) {
            Fruit given = _fruits.front();
            _fruits.pop_front();
            other += given;
        }
        return *this;
    }
    
    size_t count() const {
        return _fruits.size();
    }
    
    size_t count(Taste taste) const {
        return std::count_if(_fruits.begin(), _fruits.end(),
            [taste](const Fruit& f) { return f.taste() == taste; });
    }
    
    size_t count(Size size) const {
        return std::count_if(_fruits.begin(), _fruits.end(),
            [size](const Fruit& f) { return f.size() == size; });
    }
    
    size_t count(Quality quality) const {
        return std::count_if(_fruits.begin(), _fruits.end(),
            [quality](const Fruit& f) { return f.quality() == quality; });
    }
    
    auto operator<=>(const Picker& other) const {
        // 1. Liczba zdrowych owoców
        auto healthy_cmp = count(Quality::HEALTHY) <=> other.count(Quality::HEALTHY);
        if (healthy_cmp != 0) return healthy_cmp;
        
        // 2. Liczba słodkich owoców
        auto sweet_cmp = count(Taste::SWEET) <=> other.count(Taste::SWEET);
        if (sweet_cmp != 0) return sweet_cmp;
        
        // 3. Liczba dużych owoców
        auto large_cmp = count(Size::LARGE) <=> other.count(Size::LARGE);
        if (large_cmp != 0) return large_cmp;
        
        // 4. Liczba średnich owoców
        auto medium_cmp = count(Size::MEDIUM) <=> other.count(Size::MEDIUM);
        if (medium_cmp != 0) return medium_cmp;
        
        // 5. Liczba małych owoców
        auto small_cmp = count(Size::SMALL) <=> other.count(Size::SMALL);
        if (small_cmp != 0) return small_cmp;
        
        // 6. Liczba wszystkich owoców
        return count() <=> other.count();
    }
    
    bool operator==(const Picker& other) const {
        if (_name != other._name) return false;
        if (_fruits.size() != other._fruits.size()) return false;
        
        auto it1 = _fruits.begin();
        auto it2 = other._fruits.begin();
        
        while (it1 != _fruits.end()) {
            if (!(*it1 == *it2)) return false;
            ++it1;
            ++it2;
        }
        
        return true;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Picker& picker);
};

inline std::ostream& operator<<(std::ostream& os, const Picker& picker) {
    os << picker._name << ":";
    for (const auto& fruit : picker._fruits) {
        os << "\n" << "\t" << fruit;
    }
    return os;
}

class Ranking { 
private:
    std::vector<Picker> _ranking;

public:
    Ranking() : _ranking() {}

    Ranking(std::initializer_list<Picker> list) 
        : _ranking(list)
    {
        std::sort(_ranking.begin(), _ranking.end(), 
            [](const Picker& a, const Picker& b) { return a > b; });
    }

    Ranking(const Ranking&) = default;
    Ranking(Ranking&&) = default;

    ~Ranking() = default;

    Ranking& operator=(const Ranking&) = default;
    Ranking& operator=(Ranking&&) = default;

    void operator+=(const Picker& picker) {
        auto it = _ranking.begin();
        // Szukamy pierwszego który jest gorszy lub równy (<= używając >=)
        // Przy remisie, nowy picker idzie PO istniejących (zachowanie kolejności dodawania)
        while (it != _ranking.end() && *it > picker) ++it;
        _ranking.insert(it, picker);
    }

    void operator+=(Picker&& picker) {
        auto it = _ranking.begin();
        while (it != _ranking.end() && *it > picker) ++it;
        _ranking.insert(it, std::move(picker)); 
    }

    void operator+=(const Ranking& other) {
        // Ranking += samego siebie → bez zmian
        if (this == &other) return;
        
        for (const auto& picker : other._ranking) {
            *this += picker;
        }
    }

    void operator+=(Ranking&& other) {
        // Ranking += samego siebie → bez zmian
        if (this == &other) return;
        
        for (auto& picker : other._ranking) {
            *this += std::move(picker);
        }
    }

    Ranking operator+(const Ranking& other) const {
        Ranking result = *this;
        result += other;
        return result;
    }

    Ranking operator+(Ranking&& other) const {
        Ranking result = *this;
        result += std::move(other);
        return result;
    }

    void operator-=(const Picker& picker) {
        for (auto it = _ranking.begin(); it != _ranking.end(); ++it) {
            if (*it == picker) {
                _ranking.erase(it);
                return;
            }
        }
    }

    const Picker& operator[](size_t idx) const {
        // Zachowanie niezdefiniowane dla pustego rankingu (zgodnie z forum)
        return _ranking[std::min(idx, _ranking.size() - 1)];
    }

    size_t count_pickers() const { return _ranking.size(); }
};

inline std::ostream& operator<<(std::ostream& os, const Ranking& ranking) {
    for (size_t i = 0; i < ranking.count_pickers(); ++i) {
        if (i > 0) os << "\n";
        os << ranking[i];
    }
    // Niepusty ranking kończy się \n
    if (ranking.count_pickers() > 0) {
        os << "\n";
    }
    return os;
}

#endif