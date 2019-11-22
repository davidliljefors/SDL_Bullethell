#pragma once

#include <cmath>
#include <string>
#include <chrono>
#include <iostream>

class Timer {
public:

	Timer(std::string name)
		: m_name(std::move(name)),
		m_beg(std::chrono::high_resolution_clock::now()) { }
	~Timer() {
		auto end = std::chrono::high_resolution_clock::now();
		auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - m_beg);
		std::cout << m_name << " : " << dur.count() << " microseconds\n";
	}
private:
	std::string m_name;
	std::chrono::time_point<std::chrono::high_resolution_clock> m_beg;
};