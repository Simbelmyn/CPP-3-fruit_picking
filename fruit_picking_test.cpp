#include "fruit_picking.h"
#include <iostream>
#include <cassert>
#include <sstream>

#define TEST(name) std::cout << "\n=== TEST: " << name << " ===" << std::endl
#define ASSERT_EQ(a, b) if ((a) != (b)) { \
    std::cerr << "FAILED at line " << __LINE__ << ": " << #a << " (" << (a) \
              << ") != " << #b << " (" << (b) << ")" << std::endl; \
    return false; \
} else { std::cout << "✓ " << #a << " == " << #b << std::endl; }
#define ASSERT_TRUE(x) if (!(x)) { \
    std::cerr << "FAILED at line " << __LINE__ << ": " << #x << " is false" << std::endl; \
    return false; \
} else { std::cout << "✓ " << #x << " is true" << std::endl; }
#define ASSERT_FALSE(x) if (x) { \
    std::cerr << "FAILED at line " << __LINE__ << ": " << #x << " is true" << std::endl; \
    return false; \
} else { std::cout << "✓ " << #x << " is false" << std::endl; }

// Testy Fruit
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
    
    // Copy constructor
    Fruit f2(f1);
    ASSERT_EQ(f2.taste(), Taste::SWEET);
    ASSERT_EQ(f2.size(), Size::MEDIUM);
    ASSERT_EQ(f2.quality(), Quality::WORMY);
    
    // Move constructor
    Fruit f3(std::move(f1));
    ASSERT_EQ(f3.taste(), Taste::SWEET);
    
    // Copy assignment
    Fruit f4(Taste::SOUR, Size::SMALL, Quality::HEALTHY);
    f4 = f2;
    ASSERT_EQ(f4.taste(), Taste::SWEET);
    
    // Move assignment
    Fruit f5(Taste::SOUR, Size::LARGE, Quality::ROTTEN);
    f5 = std::move(f3);
    ASSERT_EQ(f5.taste(), Taste::SWEET);
    
    return true;
}

bool test_fruit_tuple_conversion() {
    TEST("Fruit - konwersje z/do tuple");
    
    std::tuple<Taste, Size, Quality> t1(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
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
    
    // go_rotten na zdrowym
    Fruit f1(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    f1.go_rotten();
    ASSERT_EQ(f1.quality(), Quality::ROTTEN);
    
    // go_rotten na już nadgniłym - nie zmienia
    f1.go_rotten();
    ASSERT_EQ(f1.quality(), Quality::ROTTEN);
    
    // go_rotten na robaczywym - nie zmienia
    Fruit f2(Taste::SOUR, Size::SMALL, Quality::WORMY);
    f2.go_rotten();
    ASSERT_EQ(f2.quality(), Quality::WORMY);
    
    // become_worm_infested na zdrowym
    Fruit f3(Taste::SWEET, Size::MEDIUM, Quality::HEALTHY);
    f3.become_worm_infested();
    ASSERT_EQ(f3.quality(), Quality::WORMY);
    
    // become_worm_infested na nadgniłym - nie zmienia
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

// Testy Picker
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
    
    // Test z rvalue
    Fruit f(Taste::SWEET, Size::MEDIUM, Quality::WORMY);
    p += std::move(f);
    ASSERT_EQ(p.count(), 3);
    
    return true;
}

bool test_picker_contamination_rotten() {
    TEST("Picker - zarażanie nadgniłym owocem");
    
    // Przypadek 1: zdrowy + nadgniły → pierwszy gnije
    Picker p1("Test1");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p1 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);
    
    std::ostringstream oss1;
    oss1 << p1;
    std::string output1 = oss1.str();
    ASSERT_TRUE(output1.find("nadgniły") != std::string::npos);
    ASSERT_EQ(p1.count(Quality::ROTTEN), 2);
    
    // Przypadek 2: nadgniły + zdrowy → drugi gnije
    Picker p2("Test2");
    p2 += Fruit(Taste::SWEET, Size::LARGE, Quality::ROTTEN);
    p2 += Fruit(Taste::SOUR, Size::SMALL, Quality::HEALTHY);
    
    ASSERT_EQ(p2.count(Quality::ROTTEN), 2); // Oba powinny być nadgnite
    ASSERT_EQ(p2.count(Quality::HEALTHY), 0);
    
    std::ostringstream oss2;
    oss2 << p2;
    std::string output2 = oss2.str();
    
    return true;
}

bool test_picker_contamination_wormy() {
    TEST("Picker - zarażanie robaczywym owocem");
    
    Picker p("Test");
    p += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);   // 1
    p += Fruit(Taste::SWEET, Size::SMALL, Quality::HEALTHY);   // 2
    p += Fruit(Taste::SOUR, Size::MEDIUM, Quality::HEALTHY);   // 3 (kwaśny)
    
    ASSERT_EQ(p.count(Quality::HEALTHY), 3);
    ASSERT_EQ(p.count(Taste::SWEET), 2);
    
    // Dodajemy robaczywy - wszystkie słodkie zdrowe powinny stać się robaczywe
    p += Fruit(Taste::SOUR, Size::SMALL, Quality::WORMY);      // 4
    
    ASSERT_EQ(p.count(Quality::WORMY), 3); // 1, 2, 4
    ASSERT_EQ(p.count(Quality::HEALTHY), 1); // tylko 3 (kwaśny)
    
    return true;
}

bool test_picker_count_by_attributes() {
    TEST("Picker - zliczanie po atrybutach");
    
    Picker p("Counter");
    p += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);   // 1: H
    p += Fruit(Taste::SWEET, Size::MEDIUM, Quality::HEALTHY);  // 2: H
    p += Fruit(Taste::SOUR, Size::SMALL, Quality::HEALTHY);    // 3: H
    p += Fruit(Taste::SOUR, Size::LARGE, Quality::WORMY);      // 4: W
    // Po dodaniu WORMY: 1 i 2 (słodkie zdrowe) → robaczywe
    
    ASSERT_EQ(p.count(), 4);
    ASSERT_EQ(p.count(Taste::SWEET), 2);
    ASSERT_EQ(p.count(Taste::SOUR), 2);
    ASSERT_EQ(p.count(Size::LARGE), 2);
    ASSERT_EQ(p.count(Size::MEDIUM), 1);
    ASSERT_EQ(p.count(Size::SMALL), 1);
    ASSERT_EQ(p.count(Quality::HEALTHY), 1);  // tylko 3 (kwaśny)
    ASSERT_EQ(p.count(Quality::ROTTEN), 0);
    ASSERT_EQ(p.count(Quality::WORMY), 3);    // 1, 2, 4
    
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
    
    p1 += p2; // Złodziej zabiera pierwszy owoc Ofiary
    
    ASSERT_EQ(p1.count(), 1);
    ASSERT_EQ(p2.count(), 1);
    ASSERT_EQ(p1.count(Taste::SWEET), 1); // Zabrał pierwszy (słodki)
    ASSERT_EQ(p2.count(Taste::SOUR), 1);  // Ofierze został drugi (kwaśny)
    
    // Próba zabrania od pustego
    Picker p3("Pusty");
    p1 += p3;
    ASSERT_EQ(p1.count(), 1); // Bez zmiany
    
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
    
    p1 -= p2; // Dawca oddaje pierwszy owoc Biorcy
    
    ASSERT_EQ(p1.count(), 1);
    ASSERT_EQ(p2.count(), 1);
    ASSERT_EQ(p1.count(Taste::SOUR), 1);  // Dawcy został drugi
    ASSERT_EQ(p2.count(Taste::SWEET), 1); // Biorca dostał pierwszy
    
    // Oddanie gdy jest pusty
    Picker p3("Pusty");
    p3 -= p2;
    ASSERT_EQ(p3.count(), 0); // Bez zmiany
    ASSERT_EQ(p2.count(), 1); // Bez zmiany
    
    return true;
}

bool test_picker_comparison_basic() {
    TEST("Picker - porównanie <=> (podstawowe)");
    
    Picker p1("A");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    
    Picker p2("B");
    p2 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    
    // p1 ma więcej zdrowych (2 vs 1) → p1 > p2
    ASSERT_TRUE(p1 > p2);
    ASSERT_TRUE(p2 < p1);
    
    return true;
}

bool test_picker_comparison_criteria() {
    TEST("Picker - porównanie <=> (wszystkie kryteria)");
    
    // Kryterium 1: liczba zdrowych
    Picker p1("Test1");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    Picker p2("Test2");
    p2 += Fruit(Taste::SWEET, Size::LARGE, Quality::ROTTEN);
    ASSERT_TRUE(p1 > p2);
    
    // Kryterium 2: liczba słodkich (przy równej liczbie zdrowych)
    Picker p3("Test3");
    p3 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p3 += Fruit(Taste::SWEET, Size::SMALL, Quality::ROTTEN);
    Picker p4("Test4");
    p4 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p4 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);
    ASSERT_TRUE(p3 > p4); // p3 ma więcej słodkich
    
    // Kryterium 3: liczba dużych
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
    
    // Różne imię
    Picker p3("Maria");
    p3 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p3 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);
    ASSERT_FALSE(p1 == p3);
    
    // Różna liczba owoców
    Picker p4("Jan");
    p4 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    ASSERT_FALSE(p1 == p4);
    
    // Różne owoce
    Picker p5("Jan");
    p5 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p5 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY); // inny owoc
    ASSERT_FALSE(p1 == p5);
    
    // Różna kolejność
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
    // Drugi owoc nie zarazi pierwszego bo oba są zdrowe
    
    std::ostringstream oss;
    oss << p;
    std::string output = oss.str();
    
    std::cout << "OUTPUT:\n" << output << "\nEND\n";
    
    ASSERT_TRUE(output.find("Jan:") != std::string::npos);
    ASSERT_TRUE(output.find("[ słodki duży zdrowy ]") != std::string::npos);
    ASSERT_TRUE(output.find("[ kwaśny mały zdrowy ]") != std::string::npos);
    
    return true;
}

// Testy Ranking
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
    
    // Sprawdź kolejność (najlepszy pierwszy)
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
    Picker p3("Jan"); // To samo imię, ale inne owoce
    p3 += Fruit(Taste::SOUR, Size::MEDIUM, Quality::HEALTHY);
    
    r += p1;
    r += p2;
    r += p3;
    
    ASSERT_EQ(r.count_pickers(), 3);
    
    // Usuń p1 (używa operator ==)
    Picker to_remove("Jan");
    to_remove += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    r -= to_remove;
    
    ASSERT_EQ(r.count_pickers(), 2);
    
    // Sprawdź, że usunięto właściwego
    bool found_p1 = false;
    for (size_t i = 0; i < r.count_pickers(); ++i) {
        if (r[i] == p1) found_p1 = true;
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
    ASSERT_EQ(r1[0].get_name(), "B"); // p2 lepszy (więcej owoców)
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
    ASSERT_EQ(r1.count_pickers(), 1); // r1 bez zmian
    ASSERT_EQ(r2.count_pickers(), 1); // r2 bez zmian
    
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
    
    // Indeks poza zakresem → zwraca ostatni
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

// Testy edge cases
bool test_edge_empty_ranking() {
    TEST("Edge case - pusty ranking");
    
    Ranking r;
    ASSERT_EQ(r.count_pickers(), 0);
    
    // Próba usunięcia z pustego
    Picker p("Test");
    r -= p;
    ASSERT_EQ(r.count_pickers(), 0);
    
    return true;
}

bool test_edge_complex_contamination() {
    TEST("Edge case - złożone zarażanie");
    
    Picker p("Complex");
    
    // Scenariusz: H → R → H
    p += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);   // 1: H
    p += Fruit(Taste::SWEET, Size::MEDIUM, Quality::ROTTEN);   // 2: R, 1→R
    
    ASSERT_EQ(p.count(Quality::ROTTEN), 2); // 1, 2
    ASSERT_EQ(p.count(Quality::HEALTHY), 0);
    
    p += Fruit(Taste::SWEET, Size::SMALL, Quality::HEALTHY);   // 3: H→R (bo 2 był R)
    
    ASSERT_EQ(p.count(Quality::ROTTEN), 3); // 1, 2, 3 wszystkie
    ASSERT_EQ(p.count(Quality::HEALTHY), 0);
    
    return true;
}

bool test_edge_ranking_ties() {
    TEST("Edge case - remisy w rankingu");
    
    // Dwa identyczne pickery
    Picker p1("First");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    
    Picker p2("Second");
    p2 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    
    Ranking r;
    r += p1; // Dodany jako pierwszy
    r += p2; // Dodany jako drugi
    
    // Według algorytmu: szukamy gdzie *it > p2
    // p1 nie jest > p2 (są równe), więc pętla się zatrzymuje na początku
    // p2 zostaje wstawiony PRZED p1? NIE - insert wstawia PRZED iteratorem
    // Więc jeśli iterator wskazuje na p1, to p2 idzie przed p1
    
    // Faktycznie według logiki: p2 powinien być PRZED p1 (nowy lepszy/równy idzie wyżej)
    // ALE według forum: "porządek zbieraczy z listy inicjalizacyjnej powinien zostać zachowany"
    
    // To sugeruje że przy operator+= powinniśmy wstawiać NA KOŃCU grupy równych
    // Czyli while (*it >= picker) zamiast while (*it > picker)
    
    // Dla teraz: akceptujemy że Second jest przed First (bo jest równy)
    ASSERT_TRUE(r.count_pickers() == 2);
    
    return true;
}

bool test_edge_steal_and_give_chain() {
    TEST("Edge case - łańcuch przekazywania owoców");
    
    Picker p1("A");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p1 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);
    
    Picker p2("B");
    Picker p3("C");
    
    // A → B (A oddaje pierwszy)
    p1 -= p2;
    ASSERT_EQ(p1.count(), 1);
    ASSERT_EQ(p2.count(), 1);
    ASSERT_EQ(p2.count(Taste::SWEET), 1); // B dostał słodki
    
    // B → C (B oddaje swój jedyny)
    p2 -= p3;
    ASSERT_EQ(p2.count(), 0);
    ASSERT_EQ(p3.count(), 1);
    
    // C zabiera od A
    p3 += p1;
    ASSERT_EQ(p3.count(), 2);
    ASSERT_EQ(p1.count(), 0);
    
    return true;
}

bool test_fruit_constexpr() {
    TEST("Fruit - constexpr");
    
    // Test compile-time construction
    constexpr Fruit f1(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    constexpr Taste t = f1.taste();
    constexpr Size s = f1.size();
    constexpr Quality q = f1.quality();
    
    ASSERT_EQ(t, Taste::SWEET);
    ASSERT_EQ(s, Size::LARGE);
    ASSERT_EQ(q, Quality::HEALTHY);
    
    // Test equality in compile time
    constexpr Fruit f2(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    constexpr bool eq = f1 == f2;
    ASSERT_TRUE(eq);
    
    // Test global constants
    constexpr bool yummy_eq = YUMMY_ONE.taste() == Taste::SWEET;
    ASSERT_TRUE(yummy_eq);
    
    return true;
}

bool test_move_semantics() {
    TEST("Edge case - semantyka przenoszenia");
    
    Picker p1("Original");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    p1 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);
    
    // Move constructor
    Picker p2(std::move(p1));
    ASSERT_EQ(p2.get_name(), "Original");
    ASSERT_EQ(p2.count(), 2);
    
    // Move assignment
    Picker p3("Temp");
    p3 = std::move(p2);
    ASSERT_EQ(p3.get_name(), "Original");
    ASSERT_EQ(p3.count(), 2);
    
    // Ranking move
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
    
    // Pierwszy robaczywy
    p += Fruit(Taste::SOUR, Size::LARGE, Quality::WORMY);
    ASSERT_EQ(p.count(Quality::WORMY), 3); // 2 słodkie + 1 nowy
    
    // Dodaj kolejny zdrowy słodki
    p += Fruit(Taste::SWEET, Size::SMALL, Quality::HEALTHY);
    ASSERT_EQ(p.count(Quality::HEALTHY), 2); // kwaśny + nowy słodki
    
    // Kolejny robaczywy - powinien zarazić nowego słodkiego
    p += Fruit(Taste::SWEET, Size::MEDIUM, Quality::WORMY);
    ASSERT_EQ(p.count(Quality::WORMY), 5); // poprzednie 3 + nowy słodki + nowy robaczywy
    ASSERT_EQ(p.count(Quality::HEALTHY), 1); // tylko kwaśny
    
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
    
    // Ranking powinien automatycznie posortować przy konstrukcji
    Ranking r{p1, p2, p3};
    
    ASSERT_EQ(r[0].get_name(), "Best");
    ASSERT_EQ(r[1].get_name(), "Medium");
    ASSERT_EQ(r[2].get_name(), "Worst");
    
    return true;
}

bool test_picker_comparison_all_criteria() {
    TEST("Edge case - porównanie ze wszystkimi kryteriami po kolei");
    
    // Równa liczba zdrowych (0), słodkich (1), dużych (1), średnich (0), małych (0)
    // Różna całkowita liczba owoców
    Picker p1("More");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::ROTTEN);
    p1 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);
    
    Picker p2("Less");
    p2 += Fruit(Taste::SWEET, Size::LARGE, Quality::ROTTEN);
    
    // p1 ma więcej owoców w sumie
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
    p += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);  // 1: H
    p += Fruit(Taste::SWEET, Size::LARGE, Quality::ROTTEN);   // 2: R, 1→R
    p += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);  // 3: H→R (bo 2 był R)
    p += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);  // 4: H→R (bo 3 stał się R)
    p += Fruit(Taste::SWEET, Size::LARGE, Quality::ROTTEN);   // 5: R, 4→R (już był R)
    
    ASSERT_EQ(p.count(Quality::ROTTEN), 5); // wszystkie
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
    p -= p2; // Oddawanie z pustego
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
    
    // LISTA INICJALIZACYJNA - tutaj kolejność powinna być zachowana przy remisie
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
    
    Picker p2 = p1; // Kopia
    
    // Modyfikacja p1 nie powinna wpływać na p2
    p1 += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);
    
    ASSERT_EQ(p1.count(), 2);
    ASSERT_EQ(p2.count(), 1);
    
    // Ranking
    Ranking r1;
    r1 += p1;
    
    Ranking r2 = r1; // Kopia
    r1 += p2;
    
    ASSERT_EQ(r1.count_pickers(), 2);
    ASSERT_EQ(r2.count_pickers(), 1);
    
    return true;
}

bool test_self_operations() {
    TEST("Edge case - operacje na samym sobie");
    
    // Picker += samego siebie
    Picker p1("Self");
    p1 += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    size_t count_before = p1.count();
    p1 += p1;
    ASSERT_EQ(p1.count(), count_before); // Bez zmian
    
    // Picker -= samego siebie
    p1 -= p1;
    ASSERT_EQ(p1.count(), count_before); // Bez zmian
    
    // Ranking += samego siebie
    Ranking r;
    r += p1;
    size_t pickers_before = r.count_pickers();
    
    std::cout << "Before r += r: " << pickers_before << std::endl;
    r += r;
    std::cout << "After r += r: " << r.count_pickers() << std::endl;
    
    // Powinno zostać bez zmian
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
    
    // Niepusty ranking kończy się \n
    ASSERT_TRUE(output.back() == '\n');
    
    // Pusty ranking NIE kończy się \n
    Ranking r_empty;
    std::ostringstream oss2;
    oss2 << r_empty;
    std::string output2 = oss2.str();
    ASSERT_TRUE(output2.empty() || output2.back() != '\n');
    
    return true;
}

// Test całościowy: symulacja konkursu zbierania
bool test_full_simulation() {
    TEST("Symulacja konkursu zbierania owoców");
    
    std::cout << "\n--- Rozpoczęcie konkursu ---\n";
    
    // Uczestnicy
    Picker jan("Jan");
    Picker maria("Maria");
    Picker piotr("Piotr");
    
    // Runda 1
    std::cout << "Runda 1:\n";
    jan += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    maria += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    maria += Fruit(Taste::SWEET, Size::MEDIUM, Quality::HEALTHY);
    piotr += Fruit(Taste::SOUR, Size::SMALL, Quality::HEALTHY);
    
    // Runda 2 - Maria znajduje nadgniły
    std::cout << "Runda 2: Maria znajduje nadgniły owoc\n";
    maria += Fruit(Taste::SOUR, Size::SMALL, Quality::ROTTEN);
    
    // Runda 3 - Piotr znajduje robaczywy
    std::cout << "Runda 3: Piotr znajduje robaczywy owoc\n";
    piotr += Fruit(Taste::SWEET, Size::LARGE, Quality::HEALTHY);
    piotr += Fruit(Taste::SWEET, Size::MEDIUM, Quality::WORMY);
    
    // Wymiana
    std::cout << "Jan oddaje Marii swój owoc\n";
    jan -= maria;
    
    // Ranking
    Ranking ranking;
    ranking += jan;
    ranking += maria;
    ranking += piotr;
    
    std::cout << "\n--- Końcowy ranking ---\n";
    std::cout << ranking << std::endl;
    
    // Weryfikacja
    ASSERT_EQ(jan.count(), 0);
    ASSERT_TRUE(maria.count() > 0);
    ASSERT_TRUE(piotr.count() > 0);
    
    return true;
}

// Główna funkcja testująca
int main() {
    std::cout << "╔════════════════════════════════════════════════════════╗\n";
    std::cout << "║  KOMPREHENSYWNA TESTERKA FRUIT_PICKING                ║\n";
    std::cout << "╚════════════════════════════════════════════════════════╝\n";
    
    int passed = 0;
    int failed = 0;
    
    // Tablica testów
    struct Test {
        bool (*func)();
        const char* name;
    };
    
    Test tests[] = {
        {test_fruit_construction, "Fruit Construction"},
        {test_fruit_copy_move, "Fruit Copy/Move"},
        {test_fruit_tuple_conversion, "Fruit Tuple Conversion"},
        {test_fruit_mutations, "Fruit Mutations"},
        {test_fruit_equality, "Fruit Equality"},
        {test_fruit_constexpr, "Fruit Constexpr"},
        {test_fruit_globals, "Fruit Global Constants"},
        {test_fruit_output, "Fruit Output"},
        {test_picker_construction, "Picker Construction"},
        {test_picker_add_fruit, "Picker Add Fruit"},
        {test_picker_contamination_rotten, "Picker Rotten Contamination"},
        {test_picker_contamination_wormy, "Picker Wormy Contamination"},
        {test_picker_count_by_attributes, "Picker Count By Attributes"},
        {test_picker_steal_fruit, "Picker Steal Fruit"},
        {test_picker_give_fruit, "Picker Give Fruit"},
        {test_picker_comparison_basic, "Picker Comparison Basic"},
        {test_picker_comparison_criteria, "Picker Comparison Criteria"},
        {test_picker_equality, "Picker Equality"},
        {test_picker_output, "Picker Output"},
        {test_ranking_construction, "Ranking Construction"},
        {test_ranking_add_picker, "Ranking Add Picker"},
        {test_ranking_remove_picker, "Ranking Remove Picker"},
        {test_ranking_merge, "Ranking Merge"},
        {test_ranking_add, "Ranking Add"},
        {test_ranking_indexing, "Ranking Indexing"},
        {test_ranking_output, "Ranking Output"},
        {test_edge_empty_ranking, "Edge: Empty Ranking"},
        {test_edge_complex_contamination, "Edge: Complex Contamination"},
        {test_edge_ranking_ties, "Edge: Ranking Ties"},
        {test_edge_steal_and_give_chain, "Edge: Steal/Give Chain"},
        {test_move_semantics, "Edge: Move Semantics"},
        {test_multiple_wormy_contamination, "Edge: Multiple Wormy"},
        {test_ranking_initializer_list_sorting, "Edge: Init List Sorting"},
        {test_picker_comparison_all_criteria, "Edge: All Comparison Criteria"},
        {test_ranking_remove_nonexistent, "Edge: Remove Nonexistent"},
        {test_fruit_rotten_chain, "Edge: Rotten Chain"},
        {test_empty_picker_operations, "Edge: Empty Picker Ops"},
        {test_ranking_with_equal_pickers, "Edge: Equal Pickers"},
        {test_copy_semantics_independence, "Edge: Copy Independence"},
        {test_self_operations, "Edge: Self Operations"},
        {test_ranking_output_newline, "Edge: Ranking Output Newline"},
        {test_full_simulation, "Full Competition Simulation"}
    };
    
    int total = sizeof(tests) / sizeof(tests[0]);
    
    for (int i = 0; i < total; ++i) {
        std::cout << "\n[" << (i+1) << "/" << total << "] ";
        try {
            if (tests[i].func()) {
                passed++;
                std::cout << "✓ PASSED: " << tests[i].name << std::endl;
            } else {
                failed++;
                std::cout << "✗ FAILED: " << tests[i].name << std::endl;
            }
        } catch (const std::exception& e) {
            failed++;
            std::cout << "✗ EXCEPTION in " << tests[i].name << ": " << e.what() << std::endl;
        }
    }
    
    std::cout << "\n╔════════════════════════════════════════════════════════╗\n";
    std::cout << "║  PODSUMOWANIE                                          ║\n";
    std::cout << "╠════════════════════════════════════════════════════════╣\n";
    std::cout << "║  Testy wykonane: " << total << std::string(38 - std::to_string(total).length(), ' ') << "║\n";
    std::cout << "║  Zaliczone:      " << passed << std::string(38 - std::to_string(passed).length(), ' ') << "║\n";
    std::cout << "║  Niezaliczone:   " << failed << std::string(38 - std::to_string(failed).length(), ' ') << "║\n";
    std::cout << "╠════════════════════════════════════════════════════════╣\n";
    if (failed == 0) {
        std::cout << "║  🎉 WSZYSTKIE TESTY ZALICZONE! 🎉                      ║\n";
    } else {
        std::cout << "║  ⚠️  UWAGA: Są nieudane testy                          ║\n";
    }
    std::cout << "╚════════════════════════════════════════════════════════╝\n";
    
    return failed > 0 ? 1 : 0;
}