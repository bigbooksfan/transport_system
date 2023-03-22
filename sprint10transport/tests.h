#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <numeric>

#include "input_reader.h"

namespace tests {

    using std::string;
    using std::cerr;

    using namespace std;            // need it here to avoid massive amount of ""s errors

    template <typename T, typename U>
    void AssertEqualImpl(const T& t, const U& u, const string& t_str, const string& u_str, const string& file, const string& func, unsigned line, const string& hint) {
        if (t != u) {
            cerr << boolalpha;
            cerr << file << "(" << line << "): " << func << ": ";
            cerr << "ASSERT_EQUAL(" << t_str << ", " << u_str << ") failed: ";
            cerr << *t << " != " << *u << ".";
            if (!hint.empty()) {
                cerr << " Hint: " << hint;
            }
            cerr << std::endl;
            abort();
        }
    }

    void AssertImpl(bool value, const string& expr_str, const string& file, const string& func, unsigned line,
        const string& hint) {
        if (!value) {
            cerr << file << "(" << line << "): " << func << ":\n";
            cerr << "ASSERT(" << expr_str << ") failed.\n";
            if (!hint.empty()) {
                cerr << "Hint: " << hint;
            }
            cerr << std::endl;
            abort();
        }
    }

    template <typename T>
    void RunTestImpl(const T& func, const string& func_name) {
        func();
        cerr << func_name << " OK\n";
    }

#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, "")

#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, "")

#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

#define RUN_TEST(func) RunTestImpl((func), #func)

    void ReadStop() {
        {       // exceptions
            
        }
        {       // read Stop, create it and check coords
            std::vector<std::string> rows(9);
            rows[0] = "8\n";
            rows[1] = "Stop Tolstopaltsevo: 55.611087, 37.208290\n";
            rows[2] = "Stop Marushkino: 55.595884, 37.209755\n";
            rows[3] = "Stop Rasskazovka: 55.632761, 37.333324\n";
            rows[4] = "Stop Biryulyovo Zapadnoye: 55.574371, 37.651700\n";
            rows[5] = "Stop Biryusinka: 55.581065, 37.648390\n";
            rows[6] = "Stop Universam: 55.587655, 37.645687\n";
            rows[7] = "Stop Biryulyovo Tovarnaya: 55.592028, 37.653656\n";
            rows[8] = "Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164\n";
            std::string acc = std::accumulate(
                rows.begin(), rows.end(),
                std::string("")
            );
            std::istringstream testdata(acc);
            std::cin.rdbuf(testdata.rdbuf());
            std::string placeholder;
            std::getline(std::cin, placeholder);
            transport_catalogue a(placeholder);

            Coordinates getted = a.FindStop("Biryulyovo Passazhirskaya");
            Coordinates correct{ 55.580999 , 37.659164 };
            ASSERT_HINT(getted == correct, "Error in reading Stop or coordinates\n");
            getted = a.FindStop("Universam");
            correct = { 55.587655 , 37.645687 };
            ASSERT_HINT(getted == correct, "Error in reading Stop or coordinates\n");
        }
    }

    void ReadBuses() {
        {       // read Bus
            std::vector<std::string> rows(3);
            rows[0] = "2\n";
            rows[1] = "Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n";
            rows[2] = "Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n";
            std::string acc = std::accumulate(
                rows.begin(), rows.end(),
                std::string("")
            );
            std::istringstream testdata(acc);
            std::cin.rdbuf(testdata.rdbuf());
            std::string placeholder;
            std::getline(std::cin, placeholder);
            transport_catalogue a(placeholder);

            std::string getted = a.FindBus("256");
            std::string correct = "Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye";
            ASSERT_HINT(getted.compare(correct) == 0, "Error in reading Bus");
        }
    }

    void AllTests() {
        RUN_TEST(ReadStop);
        RUN_TEST(ReadBuses);
    }

}		// namespace tests