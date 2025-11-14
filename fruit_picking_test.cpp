#include "fruit_picking.h"
#include <iostream>
#include <cassert>
#include <sstream>
#include <iomanip>

#define TEST(name) std::cout << "\n=== TEST: " << name << " ===" << std::endl
#define ASSERT_EQ(a, b)                                                        \
    if ((a) != (b)) {                                                          \
        std::cerr << "FAILED at line " << __LINE__ << ": " << #a << " ("       \
                  << (a) << ") != " << #b << " (" << (b) << ")" << std::endl;  \
        return false;                                                          \
    } else {                                                                   \
        std::cout << "✓ " << #a << " == " << #b << std::endl;                  \
    }
#define ASSERT_TRUE(x)                                                         \
    if (!(x)) {                                                                \
        std::cerr << "FAILED at line " << __LINE__ << ": " << #x               \
                  << " is false" << std::endl;                                 \
        return false;                                                          \
    } else {                                                                   \
        std::cout << "✓ " << #x << " is true" << std::endl;                    \
    }
#define ASSERT_FALSE(x)                                                        \
    if (x) {                                                                   \
        std::cerr << "FAILED at line " << __LINE__ << ": " << #x << " is true" \
                  << std::endl;                                                \
        return false;                                                          \
    } else {                                                                   \
        std::cout << "✓ " << #x << " is false" << std::endl;                   \
    }

// ============================================================================
// TESTY FRUIT
// ============================================================================

bool test_fruit_construction() {
    TEST("Fruit - podstawowa konstrukcja");

    Fruit f1(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    ASSERT_EQ(f1.taste(), Taste::SWEET);
    ASSERT_EQ(f1.size(), Size::LARGE);
    ASSERT_EQ(f1.quality(), Quality::HEALTHY);

    Fruit f2(Taste::SOUR, Size::SMALL, Quality::ROTTEN);
    ASSERT_EQ(f2.taste(), Taste::SOUR);
    ASSERT_EQ(f2.size(), Size::SMALL);
    ASSERT_EQ(f2.quality(), Quality::ROTTEN);

    return true;
}

bool test_fruit_copy_move() {
    TEST("Fruit - kopiowanie i przenoszenie");

    Fruit f1(Taste::SWEET, Size::MEDIUM, Quality::WORMY);

    Fruit f2(f1);
    ASSERT_EQ(f2.taste(), Taste::SWEET);
    ASSERT_EQ(f2.size(), Size::MEDIUM);
    ASSERT_EQ(f2.quality(), Quality::WORMY);

    Fruit f3(std::move(f1));
    ASSERT_EQ(f3.taste(), Taste::SWEET);

    Fruit f4(Taste::SOUR, Size::SMALL, Quality::HEALTHY);
    f4 = f2;
    ASSERT_EQ(f4.taste(), Taste::SWEET);

    Fruit f5(Taste::SOUR, Size::LARGE, Quality::ROTTEN);
    f5 = std::move(f3);
    ASSERT_EQ(f5.taste(), Taste::SWEET);

    return true;
}

bool test_fruit_tuple_conversion() {
    TEST("Fruit - konwersje z/do tuple");

    std::tuple<Taste, Size, Quality> t1(Taste::SWEET, Size::LARGE,
                                        Quality::HEALTHY);
    Fruit f1(t1);
    ASSERT_EQ(f1.taste(), Taste::SWEET);
    ASSERT_EQ(f1.size(), Size::LARGE);
    ASSERT_EQ(f1.quality(), Quality::HEALTHY);

    auto t2 = static_cast<std::tuple<Taste, Size, Quality>>(f1);
    ASSERT_EQ(std::get<0>(t2), Taste::SWEET);
    ASSERT_EQ(std::get<1>(t2), Size::LARGE);
    ASSERT_EQ(std::get<2>(t2), Quality::HEALTHY);

    return true;
}

bool test_fruit_mutations() {
    TEST("Fruit - mutacje (go_rotten, become_worm_infested)");

    Fruit f1(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    f1.go_rotten();
    ASSERT_EQ(f1.quality(), Quality::ROTTEN);

    f1.go_rotten();
    ASSERT_EQ(f1.quality(), Quality::ROTTEN);

    Fruit f2(Taste::SOUR, Size::SMALL, Quality::WORMY);
    f2.go_rotten();
    ASSERT_EQ(f2.quality(), Quality::WORMY);

    Fruit f3(Taste::SWEET, Size::MEDIUM, Quality::HEALTHY);
    f3.become_worm_infested();
    ASSERT_EQ(f3.quality(), Quality::WORMY);

    Fruit f4(Taste::SOUR, Size::LARGE, Quality::ROTTEN);
    f4.become_worm_infested();
    ASSERT_EQ(f4.quality(), Quality::ROTTEN);

    return true;
}

bool test_fruit_equality() {
    TEST("Fruit - operator ==");

    Fruit f1(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    Fruit f2(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    Fruit f3(Taste::SOUR, Size::LARGE, Quality::HEALTHY);
    Fruit f4(Taste::SWEET, Size::MEDIUM, Quality::HEALTHY);
    Fruit f5(Taste::SWEET, Size::LARGE, Quality::ROTTEN);

    ASSERT_TRUE(f1 == f2);
    ASSERT_FALSE(f1 == f3);
    ASSERT_FALSE(f1 == f4);
    ASSERT_FALSE(f1 == f5);

    return true;
}

bool test_fruit_globals() {
    TEST("Fruit - globalne stałe YUMMY_ONE i ROTTY_ONE");

    ASSERT_EQ(YUMMY_ONE.taste(), Taste::SWEET);
    ASSERT_EQ(YUMMY_ONE.size(), Size::LARGE);
    ASSERT_EQ(YUMMY_ONE.quality(), Quality::HEALTHY);

    ASSERT_EQ(ROTTY_ONE.taste(), Taste::SOUR);
    ASSERT_EQ(ROTTY_ONE.size(), Size::SMALL);
    ASSERT_EQ(ROTTY_ONE.quality(), Quality::ROTTEN);

    return true;
}

bool test_fruit_output() {
    TEST("Fruit - operator <<");

    std::ostringstream oss;
    Fruit f(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    oss << f;
    ASSERT_EQ(oss.str(), "[ słodki duży zdrowy ]");

    return true;
}

bool test_fruit_constexpr() {
    TEST("Fruit - constexpr");

    constexpr Fruit f1(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    constexpr Taste t = f1.taste();
    constexpr Size s = f1.size();
    constexpr Quality q = f1.quality();

    ASSERT_EQ(t, Taste::SWEET);
    ASSERT_EQ(s, Size::LARGE);
    ASSERT_EQ(q, Quality::HEALTHY);

    constexpr Fruit f2(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    constexpr bool eq = f1 == f2;
    ASSERT_TRUE(eq);

    constexpr bool yummy_eq = YUMMY_ONE.taste() == Taste::SWEET;
    ASSERT_TRUE(yummy_eq);

    return true;
}

// ============================================================================
// TESTY PICKER
// ============================================================================

bool test_picker_construction() {
    TEST("Picker - konstrukcja");

    Picker p1("Jan");
    ASSERT_EQ(p1.get_name(), "Jan");
    ASSERT_EQ(p1.count(), 0);

    Picker p2("");
    ASSERT_EQ(p2.get_name(), "Anonim");

    Picker p3;
    ASSERT_EQ(p3.get_name(), "Anonim");

    return true;
}

bool test_picker_add_fruit() {
    TEST("Picker - dodawanie owoców (operator +=)");

    Picker p("Test");

    p += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    ASSERT_EQ(p.count(), 1);

    p += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);
    ASSERT_EQ(p.count(), 2);

    Fruit f(Taste::SWEET, Size::MEDIUM, Quality::WORMY);
    p += std::move(f);
    ASSERT_EQ(p.count(), 3);

    return true;
}

bool test_picker_contamination_rotten() {
    TEST("Picker - zarażanie nadgniłym owocem");

    Picker p1("Test1");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p1 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);

    ASSERT_EQ(p1.count(Quality::ROTTEN), 2);

    Picker p2("Test2");
    p2 += Fruit(Taste::SWEET, Size::LARGE, Quality::ROTTEN);
    p2 += Fruit(Taste::SOUR, Size::SMALL, Quality::HEALTHY);

    ASSERT_EQ(p2.count(Quality::ROTTEN), 2);
    ASSERT_EQ(p2.count(Quality::HEALTHY), 0);

    return true;
}

bool test_picker_contamination_wormy() {
    TEST("Picker - zarażanie robaczywym owocem");

    Picker p("Test");
    p += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p += Fruit(Taste::SWEET, Size::SMALL, Quality::HEALTHY);
    p += Fruit(Taste::SOUR, Size::MEDIUM, Quality::HEALTHY);

    ASSERT_EQ(p.count(Quality::HEALTHY), 3);
    ASSERT_EQ(p.count(Taste::SWEET), 2);

    p += Fruit(Taste::SOUR, Size::SMALL, Quality::WORMY);

    ASSERT_EQ(p.count(Quality::WORMY), 3);
    ASSERT_EQ(p.count(Quality::HEALTHY), 1);

    return true;
}

bool test_picker_count_by_attributes() {
    TEST("Picker - zliczanie po atrybutach");

    Picker p("Counter");
    p += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p += Fruit(Taste::SWEET, Size::MEDIUM, Quality::HEALTHY);
    p += Fruit(Taste::SOUR, Size::SMALL, Quality::HEALTHY);
    p += Fruit(Taste::SOUR, Size::LARGE, Quality::WORMY);

    ASSERT_EQ(p.count(), 4);
    ASSERT_EQ(p.count(Taste::SWEET), 2);
    ASSERT_EQ(p.count(Taste::SOUR), 2);
    ASSERT_EQ(p.count(Size::LARGE), 2);
    ASSERT_EQ(p.count(Size::MEDIUM), 1);
    ASSERT_EQ(p.count(Size::SMALL), 1);
    ASSERT_EQ(p.count(Quality::HEALTHY), 1);
    ASSERT_EQ(p.count(Quality::ROTTEN), 0);
    ASSERT_EQ(p.count(Quality::WORMY), 3);

    return true;
}

bool test_picker_steal_fruit() {
    TEST("Picker - zabieranie owocu od innego (operator += Picker)");

    Picker p1("Złodziej");
    Picker p2("Ofiara");

    p2 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p2 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);

    ASSERT_EQ(p2.count(), 2);
    ASSERT_EQ(p1.count(), 0);

    p1 += p2;

    ASSERT_EQ(p1.count(), 1);
    ASSERT_EQ(p2.count(), 1);
    ASSERT_EQ(p1.count(Taste::SWEET), 1);
    ASSERT_EQ(p2.count(Taste::SOUR), 1);

    Picker p3("Pusty");
    p1 += p3;
    ASSERT_EQ(p1.count(), 1);

    return true;
}

bool test_picker_give_fruit() {
    TEST("Picker - oddawanie owocu (operator -=)");

    Picker p1("Dawca");
    Picker p2("Biorca");

    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p1 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);

    ASSERT_EQ(p1.count(), 2);
    ASSERT_EQ(p2.count(), 0);

    p1 -= p2;

    ASSERT_EQ(p1.count(), 1);
    ASSERT_EQ(p2.count(), 1);
    ASSERT_EQ(p1.count(Taste::SOUR), 1);
    ASSERT_EQ(p2.count(Taste::SWEET), 1);

    Picker p3("Pusty");
    p3 -= p2;
    ASSERT_EQ(p3.count(), 0);
    ASSERT_EQ(p2.count(), 1);

    return true;
}

bool test_picker_comparison_basic() {
    TEST("Picker - porównanie <=> (podstawowe)");

    Picker p1("A");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);

    Picker p2("B");
    p2 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);

    ASSERT_TRUE(p1 > p2);
    ASSERT_TRUE(p2 < p1);

    return true;
}

bool test_picker_comparison_criteria() {
    TEST("Picker - porównanie <=> (wszystkie kryteria)");

    Picker p1("Test1");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    Picker p2("Test2");
    p2 += Fruit(Taste::SWEET, Size::LARGE, Quality::ROTTEN);
    ASSERT_TRUE(p1 > p2);

    Picker p3("Test3");
    p3 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p3 += Fruit(Taste::SWEET, Size::SMALL, Quality::ROTTEN);
    Picker p4("Test4");
    p4 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p4 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);
    ASSERT_TRUE(p3 > p4);

    Picker p5("Test5");
    p5 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p5 += Fruit(Taste::SWEET, Size::LARGE, Quality::ROTTEN);
    Picker p6("Test6");
    p6 += Fruit(Taste::SWEET, Size::SMALL, Quality::HEALTHY);
    p6 += Fruit(Taste::SWEET, Size::MEDIUM, Quality::ROTTEN);
    ASSERT_TRUE(p5 > p6);

    return true;
}

bool test_picker_equality() {
    TEST("Picker - operator ==");

    Picker p1("Jan");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p1 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);

    Picker p2("Jan");
    p2 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p2 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);

    ASSERT_TRUE(p1 == p2);

    Picker p3("Maria");
    p3 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p3 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);
    ASSERT_FALSE(p1 == p3);

    Picker p4("Jan");
    p4 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    ASSERT_FALSE(p1 == p4);

    Picker p5("Jan");
    p5 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p5 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    ASSERT_FALSE(p1 == p5);

    Picker p6("Jan");
    p6 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);
    p6 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    ASSERT_FALSE(p1 == p6);

    return true;
}

bool test_picker_output() {
    TEST("Picker - operator <<");

    Picker p("Jan");
    p += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p += Fruit(Taste::SOUR, Size::SMALL, Quality::HEALTHY);

    std::ostringstream oss;
    oss << p;
    std::string output = oss.str();

    ASSERT_TRUE(output.find("Jan:") != std::string::npos);
    ASSERT_TRUE(output.find("[ słodki duży zdrowy ]") != std::string::npos);
    ASSERT_TRUE(output.find("[ kwaśny mały zdrowy ]") != std::string::npos);

    return true;
}

// ============================================================================
// TESTY RANKING
// ============================================================================

bool test_ranking_construction() {
    TEST("Ranking - konstrukcja");

    Ranking r1;
    ASSERT_EQ(r1.count_pickers(), 0);

    Picker p1("A");
    Picker p2("B");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);

    Ranking r2{p1, p2};
    ASSERT_EQ(r2.count_pickers(), 2);

    return true;
}

bool test_ranking_add_picker() {
    TEST("Ranking - dodawanie pickera (operator +=)");

    Ranking r;

    Picker p1("Najgorszy");
    Picker p2("Średni");
    p2 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    Picker p3("Najlepszy");
    p3 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p3 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);

    r += p2;
    r += p1;
    r += p3;

    ASSERT_EQ(r.count_pickers(), 3);
    ASSERT_EQ(r[0].get_name(), "Najlepszy");
    ASSERT_EQ(r[1].get_name(), "Średni");
    ASSERT_EQ(r[2].get_name(), "Najgorszy");

    return true;
}

bool test_ranking_remove_picker() {
    TEST("Ranking - usuwanie pickera (operator -=)");

    Ranking r;

    Picker p1("Jan");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    Picker p2("Maria");
    p2 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);
    Picker p3("Jan");
    p3 += Fruit(Taste::SOUR, Size::MEDIUM, Quality::HEALTHY);

    r += p1;
    r += p2;
    r += p3;

    ASSERT_EQ(r.count_pickers(), 3);

    Picker to_remove("Jan");
    to_remove += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    r -= to_remove;

    ASSERT_EQ(r.count_pickers(), 2);

    bool found_p1 = false;
    for (size_t i = 0; i < r.count_pickers(); ++i) {
        if (r[i] == p1)
            found_p1 = true;
    }
    ASSERT_FALSE(found_p1);

    return true;
}

bool test_ranking_merge() {
    TEST("Ranking - łączenie rankingów (operator += Ranking)");

    Picker p1("A");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    Picker p2("B");
    p2 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p2 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);

    Ranking r1;
    r1 += p1;

    Ranking r2;
    r2 += p2;

    r1 += r2;

    ASSERT_EQ(r1.count_pickers(), 2);
    ASSERT_EQ(r1[0].get_name(), "B");
    ASSERT_EQ(r1[1].get_name(), "A");

    return true;
}

bool test_ranking_add() {
    TEST("Ranking - dodawanie rankingów (operator +)");

    Picker p1("A");
    Picker p2("B");
    p2 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);

    Ranking r1;
    r1 += p1;

    Ranking r2;
    r2 += p2;

    Ranking r3 = r1 + r2;

    ASSERT_EQ(r3.count_pickers(), 2);
    ASSERT_EQ(r1.count_pickers(), 1);
    ASSERT_EQ(r2.count_pickers(), 1);

    return true;
}

bool test_ranking_indexing() {
    TEST("Ranking - indeksowanie (operator [])");

    Ranking r;
    Picker p1("First");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    Picker p2("Second");
    p2 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    Picker p3("Third");

    r += p1;
    r += p2;
    r += p3;

    ASSERT_EQ(r[0].get_name(), "First");
    ASSERT_EQ(r[1].get_name(), "Second");
    ASSERT_EQ(r[2].get_name(), "Third");
    ASSERT_EQ(r[100].get_name(), "Third");

    return true;
}

bool test_ranking_output() {
    TEST("Ranking - operator <<");

    Picker p1("Jan");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    Picker p2("Maria");
    p2 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);

    Ranking r;
    r += p1;
    r += p2;

    std::ostringstream oss;
    oss << r;
    std::string output = oss.str();

    ASSERT_TRUE(output.find("Jan:") != std::string::npos);
    ASSERT_TRUE(output.find("Maria:") != std::string::npos);
    ASSERT_TRUE(output.find("[ słodki duży zdrowy ]") != std::string::npos);

    return true;
}

// ============================================================================
// TESTY EDGE CASES
// ============================================================================

bool test_edge_empty_ranking() {
    TEST("Edge case - pusty ranking");

    Ranking r;
    ASSERT_EQ(r.count_pickers(), 0);

    Picker p("Test");
    r -= p;
    ASSERT_EQ(r.count_pickers(), 0);

    return true;
}

bool test_edge_complex_contamination() {
    TEST("Edge case - złożone zarażanie");

    Picker p("Complex");

    p += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p += Fruit(Taste::SWEET, Size::MEDIUM, Quality::ROTTEN);

    ASSERT_EQ(p.count(Quality::ROTTEN), 2);
    ASSERT_EQ(p.count(Quality::HEALTHY), 0);

    p += Fruit(Taste::SWEET, Size::SMALL, Quality::HEALTHY);

    ASSERT_EQ(p.count(Quality::ROTTEN), 3);
    ASSERT_EQ(p.count(Quality::HEALTHY), 0);

    return true;
}

bool test_edge_steal_and_give_chain() {
    TEST("Edge case - łańcuch przekazywania owoców");

    Picker p1("A");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p1 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);

    Picker p2("B");
    Picker p3("C");

    p1 -= p2;
    ASSERT_EQ(p1.count(), 1);
    ASSERT_EQ(p2.count(), 1);
    ASSERT_EQ(p2.count(Taste::SWEET), 1);

    p2 -= p3;
    ASSERT_EQ(p2.count(), 0);
    ASSERT_EQ(p3.count(), 1);

    p3 += p1;
    ASSERT_EQ(p3.count(), 2);
    ASSERT_EQ(p1.count(), 0);

    return true;
}

bool test_move_semantics() {
    TEST("Edge case - semantyka przenoszenia");

    Picker p1("Original");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p1 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);

    Picker p2(std::move(p1));
    ASSERT_EQ(p2.get_name(), "Original");
    ASSERT_EQ(p2.count(), 2);

    Picker p3("Temp");
    p3 = std::move(p2);
    ASSERT_EQ(p3.get_name(), "Original");
    ASSERT_EQ(p3.count(), 2);

    Ranking r1;
    r1 += p3;
    Ranking r2(std::move(r1));
    ASSERT_EQ(r2.count_pickers(), 1);

    return true;
}

bool test_multiple_wormy_contamination() {
    TEST("Edge case - wielokrotne zarażanie robaczywymi");

    Picker p("Multi");
    p += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p += Fruit(Taste::SWEET, Size::MEDIUM, Quality::HEALTHY);
    p += Fruit(Taste::SOUR, Size::SMALL, Quality::HEALTHY);

    p += Fruit(Taste::SOUR, Size::LARGE, Quality::WORMY);
    ASSERT_EQ(p.count(Quality::WORMY), 3);

    p += Fruit(Taste::SWEET, Size::SMALL, Quality::HEALTHY);
    ASSERT_EQ(p.count(Quality::HEALTHY), 2);

    p += Fruit(Taste::SWEET, Size::MEDIUM, Quality::WORMY);
    ASSERT_EQ(p.count(Quality::WORMY), 5);
    ASSERT_EQ(p.count(Quality::HEALTHY), 1);

    return true;
}

bool test_ranking_initializer_list_sorting() {
    TEST("Edge case - sortowanie przy konstrukcji z listy");

    Picker p1("Worst");
    Picker p2("Best");
    p2 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p2 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    Picker p3("Medium");
    p3 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);

    Ranking r{p1, p2, p3};

    ASSERT_EQ(r[0].get_name(), "Best");
    ASSERT_EQ(r[1].get_name(), "Medium");
    ASSERT_EQ(r[2].get_name(), "Worst");

    return true;
}

bool test_picker_comparison_all_criteria() {
    TEST("Edge case - porównanie ze wszystkimi kryteriami");

    Picker p1("More");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::ROTTEN);
    p1 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);

    Picker p2("Less");
    p2 += Fruit(Taste::SWEET, Size::LARGE, Quality::ROTTEN);

    ASSERT_TRUE(p1 > p2);

    return true;
}

bool test_ranking_remove_nonexistent() {
    TEST("Edge case - usuwanie nieistniejącego pickera");

    Ranking r;
    Picker p1("Exists");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    r += p1;

    Picker p2("NotExists");
    p2 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);

    size_t count_before = r.count_pickers();
    r -= p2;
    ASSERT_EQ(r.count_pickers(), count_before);

    return true;
}

bool test_fruit_rotten_chain() {
    TEST("Edge case - łańcuch zarażania nadgniłymi");

    Picker p("Chain");
    p += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p += Fruit(Taste::SWEET, Size::LARGE, Quality::ROTTEN);
    p += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p += Fruit(Taste::SWEET, Size::LARGE, Quality::ROTTEN);

    ASSERT_EQ(p.count(Quality::ROTTEN), 5);
    ASSERT_EQ(p.count(Quality::HEALTHY), 0);

    return true;
}

bool test_empty_picker_operations() {
    TEST("Edge case - operacje na pustym pickerze");

    Picker p("Empty");

    ASSERT_EQ(p.count(), 0);
    ASSERT_EQ(p.count(Taste::SWEET), 0);
    ASSERT_EQ(p.count(Size::LARGE), 0);
    ASSERT_EQ(p.count(Quality::HEALTHY), 0);

    Picker p2("Receiver");
    p -= p2;
    ASSERT_EQ(p.count(), 0);
    ASSERT_EQ(p2.count(), 0);

    return true;
}

bool test_ranking_with_equal_pickers() {
    TEST("Edge case - ranking z wieloma równymi pickerami");

    Picker p1("Equal1");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);

    Picker p2("Equal2");
    p2 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);

    Picker p3("Equal3");
    p3 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);

    Ranking r{p1, p2, p3};

    ASSERT_EQ(r[0].get_name(), "Equal1");
    ASSERT_EQ(r[1].get_name(), "Equal2");
    ASSERT_EQ(r[2].get_name(), "Equal3");

    return true;
}

bool test_copy_semantics_independence() {
    TEST("Edge case - niezależność kopii");

    Picker p1("Original");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);

    Picker p2 = p1;

    p1 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);

    ASSERT_EQ(p1.count(), 2);
    ASSERT_EQ(p2.count(), 1);

    Ranking r1;
    r1 += p1;

    Ranking r2 = r1;
    r1 += p2;

    ASSERT_EQ(r1.count_pickers(), 2);
    ASSERT_EQ(r2.count_pickers(), 1);

    return true;
}

bool test_self_operations() {
    TEST("Edge case - operacje na samym sobie");

    Picker p1("Self");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    size_t count_before = p1.count();
    p1 += p1;
    ASSERT_EQ(p1.count(), count_before);

    p1 -= p1;
    ASSERT_EQ(p1.count(), count_before);

    Ranking r;
    r += p1;
    size_t pickers_before = r.count_pickers();

    r += r;
    ASSERT_EQ(r.count_pickers(), pickers_before);

    return true;
}

bool test_ranking_output_newline() {
    TEST("Edge case - ranking kończy się newline");

    Picker p1("Test");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);

    Ranking r;
    r += p1;

    std::ostringstream oss;
    oss << r;
    std::string output = oss.str();

    ASSERT_TRUE(!output.empty());
    ASSERT_TRUE(output.back() == '\n');

    Ranking r_empty;
    std::ostringstream oss2;
    oss2 << r_empty;
    std::string output2 = oss2.str();

    ASSERT_TRUE(output2.empty() || output2.back() != '\n');

    return true;
}

bool test_wormy_does_not_affect_sour() {
    TEST("Edge case - robaczywy nie zaraza kwaśnych");

    Picker p("Test");
    p += Fruit(Taste::SOUR, Size::LARGE, Quality::HEALTHY);
    p += Fruit(Taste::SOUR, Size::MEDIUM, Quality::HEALTHY);
    p += Fruit(Taste::SWEET, Size::SMALL, Quality::HEALTHY);

    p += Fruit(Taste::SOUR, Size::LARGE, Quality::WORMY);

    ASSERT_EQ(p.count(Quality::HEALTHY), 2);
    ASSERT_EQ(p.count(Quality::WORMY), 2);

    return true;
}

bool test_rotten_doesnt_affect_wormy() {
    TEST("Edge case - nadgniły nie zaraza robaczywego");

    Picker p("Test");
    p += Fruit(Taste::SWEET, Size::LARGE, Quality::WORMY);
    p += Fruit(Taste::SWEET, Size::MEDIUM, Quality::ROTTEN);

    // Robaczywy pozostaje robaczywy (nie gnije)
    ASSERT_EQ(p.count(Quality::WORMY), 1);
    // Tylko nowo dodany owoc jest nadgniły
    ASSERT_EQ(p.count(Quality::ROTTEN), 1);
    ASSERT_EQ(p.count(Quality::HEALTHY), 0);

    return true;
}

bool test_ranking_stable_insert() {
    TEST("Edge case - stabilność przy dodawaniu równych");

    Picker p1("First");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);

    Picker p2("Second");
    p2 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);

    Picker p3("Third");
    p3 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);

    Ranking r;
    r += p1;
    r += p2;
    r += p3;

    ASSERT_EQ(r[0].get_name(), "First");
    ASSERT_EQ(r[1].get_name(), "Second");
    ASSERT_EQ(r[2].get_name(), "Third");

    return true;
}

bool test_ranking_merge_stable() {
    TEST("Edge case - stabilność przy mergowaniu rankingów");

    Picker p1("A1");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);

    Picker p2("A2");
    p2 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);

    Picker p3("B1");
    p3 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);

    Picker p4("B2");
    p4 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);

    Ranking r1;
    r1 += p1;
    r1 += p2;

    Ranking r2;
    r2 += p3;
    r2 += p4;

    r1 += r2;

    ASSERT_EQ(r1[0].get_name(), "A1");
    ASSERT_EQ(r1[1].get_name(), "A2");
    ASSERT_EQ(r1[2].get_name(), "B1");
    ASSERT_EQ(r1[3].get_name(), "B2");

    return true;
}

bool test_enum_output_polish() {
    TEST("Edge case - polskie nazwy w outputcie");

    std::ostringstream oss1, oss2, oss3;

    oss1 << Taste::SWEET;
    ASSERT_EQ(oss1.str(), "słodki");

    oss2 << Size::MEDIUM;
    ASSERT_EQ(oss2.str(), "średni");

    oss3 << Quality::WORMY;
    ASSERT_EQ(oss3.str(), "robaczywy");

    return true;
}

bool test_picker_output_format() {
    TEST("Edge case - format outputu pickera z tabulacją");

    Picker p("Test");
    p += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);

    std::ostringstream oss;
    oss << p;
    std::string output = oss.str();

    ASSERT_TRUE(output.find("Test:") != std::string::npos);
    ASSERT_TRUE(output.find("\n\t[") != std::string::npos);

    return true;
}

bool test_comparison_tie_breaker_order() {
    TEST("Edge case - dokładna kolejność kryteriów porównania");

    Picker p1("Same");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p1 += Fruit(Taste::SOUR, Size::MEDIUM, Quality::ROTTEN);
    p1 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);

    Picker p2("Same");
    p2 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p2 += Fruit(Taste::SWEET, Size::MEDIUM, Quality::ROTTEN);
    p2 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);

    ASSERT_TRUE(p2 > p1);

    return true;
}

// ============================================================================
// TEST SYMULACYJNY
// ============================================================================

bool test_full_simulation() {
    TEST("Symulacja konkursu zbierania owoców");

    Picker jan("Jan");
    Picker maria("Maria");
    Picker piotr("Piotr");

    jan += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    maria += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    maria += Fruit(Taste::SWEET, Size::MEDIUM, Quality::HEALTHY);
    piotr += Fruit(Taste::SOUR, Size::SMALL, Quality::HEALTHY);

    maria += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);

    piotr += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    piotr += Fruit(Taste::SWEET, Size::MEDIUM, Quality::WORMY);

    jan -= maria;

    Ranking ranking;
    ranking += jan;
    ranking += maria;
    ranking += piotr;

    ASSERT_EQ(jan.count(), 0);
    ASSERT_TRUE(maria.count() > 0);
    ASSERT_TRUE(piotr.count() > 0);

    return true;
}

// ============================================================================
// GŁÓWNA FUNKCJA
// ============================================================================

void print_separator() {
    std::cout << "╔════════════════════════════════════════════════════════╗\n";
}

void print_line(const std::string &text) {
    std::cout << "║ " << std::left << std::setw(54) << text << " ║\n";
}

int main() {
    print_separator();
    print_line("KOMPREHENSYWNA TESTERKA FRUIT_PICKING");
    print_separator();

    int passed = 0;
    int failed = 0;

    struct Test {
        bool (*func)();
        const char *name;
        const char *category;
    };

    Test tests[] = {
        // FRUIT
        {test_fruit_construction, "Konstrukcja", "Fruit"},
        {test_fruit_copy_move, "Kopiowanie i przenoszenie", "Fruit"},
        {test_fruit_tuple_conversion, "Konwersje tuple", "Fruit"},
        {test_fruit_mutations, "Mutacje", "Fruit"},
        {test_fruit_equality, "Równość", "Fruit"},
        {test_fruit_constexpr, "Constexpr", "Fruit"},
        {test_fruit_globals, "Stałe globalne", "Fruit"},
        {test_fruit_output, "Wypisywanie", "Fruit"},

        // PICKER
        {test_picker_construction, "Konstrukcja", "Picker"},
        {test_picker_add_fruit, "Dodawanie owoców", "Picker"},
        {test_picker_contamination_rotten, "Zarażanie nadgniłym", "Picker"},
        {test_picker_contamination_wormy, "Zarażanie robaczywym", "Picker"},
        {test_picker_count_by_attributes, "Zliczanie atrybutów", "Picker"},
        {test_picker_steal_fruit, "Zabieranie owoców", "Picker"},
        {test_picker_give_fruit, "Oddawanie owoców", "Picker"},
        {test_picker_comparison_basic, "Porównanie podstawowe", "Picker"},
        {test_picker_comparison_criteria, "Porównanie kryteria", "Picker"},
        {test_picker_equality, "Równość", "Picker"},
        {test_picker_output, "Wypisywanie", "Picker"},

        // RANKING
        {test_ranking_construction, "Konstrukcja", "Ranking"},
        {test_ranking_add_picker, "Dodawanie pickera", "Ranking"},
        {test_ranking_remove_picker, "Usuwanie pickera", "Ranking"},
        {test_ranking_merge, "Łączenie rankingów", "Ranking"},
        {test_ranking_add, "Operator +", "Ranking"},
        {test_ranking_indexing, "Indeksowanie", "Ranking"},
        {test_ranking_output, "Wypisywanie", "Ranking"},

        // EDGE CASES
        {test_edge_empty_ranking, "Pusty ranking", "Edge"},
        {test_edge_complex_contamination, "Złożone zarażanie", "Edge"},
        {test_edge_steal_and_give_chain, "Łańcuch przekazywania", "Edge"},
        {test_move_semantics, "Semantyka przenoszenia", "Edge"},
        {test_multiple_wormy_contamination, "Wielokrotne zarażanie", "Edge"},
        {test_ranking_initializer_list_sorting, "Sortowanie init list", "Edge"},
        {test_picker_comparison_all_criteria, "Wszystkie kryteria", "Edge"},
        {test_ranking_remove_nonexistent, "Usuwanie nieistniejącego", "Edge"},
        {test_fruit_rotten_chain, "Łańcuch nadgniłych", "Edge"},
        {test_empty_picker_operations, "Operacje na pustym", "Edge"},
        {test_ranking_with_equal_pickers, "Równi pickerzy", "Edge"},
        {test_copy_semantics_independence, "Niezależność kopii", "Edge"},
        {test_self_operations, "Operacje na sobie", "Edge"},
        {test_ranking_output_newline, "Newline w outputcie", "Edge"},
        {test_wormy_does_not_affect_sour, "Robaczywy vs kwaśny", "Edge"},
        {test_rotten_doesnt_affect_wormy, "Nadgniły vs robaczywy", "Edge"},
        {test_ranking_stable_insert, "Stabilność insert", "Edge"},
        {test_ranking_merge_stable, "Stabilność merge", "Edge"},
        {test_enum_output_polish, "Polskie nazwy enum", "Edge"},
        {test_picker_output_format, "Format outputu pickera", "Edge"},
        {test_comparison_tie_breaker_order, "Kolejność kryteriów", "Edge"},

        // SYMULACJA
        {test_full_simulation, "Pełna symulacja konkursu", "Simulation"}};

    int total = sizeof(tests) / sizeof(tests[0]);
    std::string current_category = "";

    for (int i = 0; i < total; ++i) {
        if (current_category != tests[i].category) {
            current_category = tests[i].category;
            std::cout << "\n╔══════════════════════════════════════════════════"
                         "══════╗\n";
            print_line(std::string("KATEGORIA: ") + current_category);
            std::cout << "╚════════════════════════════════════════════════════"
                         "════╝\n";
        }

        std::cout << "\n[" << (i + 1) << "/" << total << "] ";
        try {
            if (tests[i].func()) {
                passed++;
                std::cout << "✓ PASSED: " << tests[i].name << std::endl;
            } else {
                failed++;
                std::cout << "✗ FAILED: " << tests[i].name << std::endl;
            }
        } catch (const std::exception &e) {
            failed++;
            std::cout << "✗ EXCEPTION in " << tests[i].name << ": " << e.what()
                      << std::endl;
        }
    }

    std::cout << "\n";
    print_separator();
    print_line("PODSUMOWANIE");
    print_separator();
    print_line("Testy wykonane: " + std::to_string(total));
    print_line("Zaliczone:      " + std::to_string(passed));
    print_line("Niezaliczone:   " + std::to_string(failed));
    print_separator();

    if (failed == 0) {
        print_line("🎉 WSZYSTKIE TESTY ZALICZONE! 🎉");
    } else {
        print_line("⚠️  UWAGA: Są nieudane testy");
    }
    print_separator();

    return failed > 0 ? 1 : 0;
}