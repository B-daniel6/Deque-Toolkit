/*
# Names: Brandon Daniel and Cameron O'Dell
# Date: 4/24/2026
# File: main.cpp
*/
#include <cassert>
#include <deque>
#include <iostream>
#include <random>
#include <string>

#include "deque.h"

static void basic_integer_test() {
    deque<int> d;
    assert(d.empty());
    assert(d.size() == 0);

    for (int i = 0; i < 128; ++i) {
        d.push_back(i);
        assert(d.front() == 0);
        assert(d.back() == i);
        assert(d.size() == static_cast<std::size_t>(i + 1));
    }

    for (int i = 0; i < 64; ++i) {
        assert(d.front() == i);
        d.pop_front();
        assert(d.size() == static_cast<std::size_t>(127 - i));
    }

    for (int i = 128; i < 192; ++i) {
        d.push_front(i);
        assert(d.front() == i);
    }

    for (std::size_t i = 0; i < d.size(); ++i) {
        int expected = (i < 64) ? (191 - static_cast<int>(i)) : static_cast<int>(i);
        assert(d[i] == expected);
    }

    while (!d.empty()) {
        d.pop_back();
    }
    assert(d.empty());
}

static void randomized_integer_test() {
    std::mt19937_64 rng(1337);
    std::uniform_int_distribution<int> value_dist(-1000, 1000);
    std::uniform_int_distribution<int> op_dist(0, 5);

    std::deque<int> reference;
    deque<int> subject;

    for (int step = 0; step < 20000; ++step) {
        int op = op_dist(rng);
        if (op == 0) {
            int value = value_dist(rng);
            reference.push_back(value);
            subject.push_back(value);
        } else if (op == 1) {
            int value = value_dist(rng);
            reference.push_front(value);
            subject.push_front(value);
        } else if (op == 2) {
            if (!reference.empty()) {
                reference.pop_back();
                subject.pop_back();
            }
        } else if (op == 3) {
            if (!reference.empty()) {
                reference.pop_front();
                subject.pop_front();
            }
        } else if (op == 4) {
            if (!reference.empty()) {
                assert(subject.front() == reference.front());
            }
        } else if (op == 5) {
            if (!reference.empty()) {
                std::size_t idx = static_cast<std::size_t>(rng() % reference.size());
                assert(subject[idx] == reference[idx]);
            }
        }
        assert(subject.size() == reference.size());
        assert(subject.empty() == reference.empty());
        if (!reference.empty()) {
            assert(subject.front() == reference.front());
            assert(subject.back() == reference.back());
        }
    }
}

static void string_test() {
    deque<std::string> d;
    d.push_back("middle");
    d.push_front("front");
    d.push_back("back");

    assert(d.size() == 3);
    assert(d.front() == "front");
    assert(d.back() == "back");
    assert(d[1] == "middle");

    d.pop_front();
    assert(d.front() == "middle");
    d.pop_back();
    assert(d.back() == "middle");
    d.pop_back();
    assert(d.empty());
}

int main() {
    std::cout << "Running deque tests...\n";
    basic_integer_test();
    std::cout << "  basic integer test passed.\n";
    randomized_integer_test();
    std::cout << "  randomized integer test passed.\n";
    string_test();
    std::cout << "  string template test passed.\n";
    std::cout << "All tests completed successfully.\n";
    return 0;
}
