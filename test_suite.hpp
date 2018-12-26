#include <chrono>
#include <stdexcept>
#include <functional>
#include <vector>
#include <iostream>
#include <string>

#define ASSERT_TRUE(x, ...) if(!(x)) throw std::runtime_error("False != True " + std::string(__VA_ARGS__));

using testFType = std::function<void()>;
using testCaseTuple = std::pair<testFType, std::string>;

class testSuite
{
private:
	std::vector<testCaseTuple> mTestCases;
public:
	testSuite() {}
	void addTest(testFType testBody, std::string name)
	{
		mTestCases.push_back(testCaseTuple(testBody, name));
	}
	void run()
	{
		for (const auto& test : mTestCases)
		{
			try
			{
				auto start = std::chrono::system_clock::now();
				test.first();
				auto end = std::chrono::system_clock::now();
				std::chrono::duration<double> elapsed_seconds = end - start;
				std::cout << "TEST." << test.second << " OK. \nElapsed time(s): " << elapsed_seconds.count() << std::endl;
			}
			catch (const std::exception& e)
			{
				std::cout << "TEST." << test.second << " FAILED" << std::endl;
				std::cout << "Fail message: " << std::string(e.what()) << std::endl; \
			}
			std::cout << std::string(80, '-') << std::endl;
		}
	}
};