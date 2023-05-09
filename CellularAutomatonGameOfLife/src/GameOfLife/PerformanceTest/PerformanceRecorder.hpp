#pragma once
#include <map>
#include <string>
#include <chrono>
#include <fstream>

using namespace std;
using namespace std::chrono;

namespace PerformanceTest
{

	class Stat
	{
	public:
		Stat(const string statName);
		~Stat();

		const string Name;
		steady_clock::time_point LastStart;

		void Write(long nanoseconds);

	private:
		ofstream statFile;
	};

	class PerformanceRecorder
	{
	public:

		~PerformanceRecorder();

		void begin(const string statName);
		void end(const string statName);
	private:
		std::map<string, Stat*> _map;
	};
}

