#ifndef LZSSTL_TEST_HH
#define LZSSTL_TEST_HH

#include <iostream>
#include <vector>

namespace test{

class TestCase{
public:
	TestCase(const char* case_name):testcase_name(case_name){}

	virtual void Run() = 0;
public:
	const char* testcase_name;
	int nTestResult;
	int nFailed;
	int nPassed;
};

class UnitTest {
public:
	static UnitTest* GetInstance();

	TestCase* RegisterTestCsase(TestCase* testcase);

	void Run();
public:
	TestCase* CurrentTestCase;
	double nPassed;
	double nFailed;
protected:
	std::vector<TestCase*> _testcase;
};

UnitTest* UnitTest::GetInstance() {
	static UnitTest instance;
	return &instance;
}

TestCase* UnitTest::RegisterTestCsase(TestCase* testcase)
{
	_testcase.push_back(testcase);
	return testcase;
}

void UnitTest::Run()
{
	for (auto it : _testcase)
	{
		TestCase* testcase = it;
		CurrentTestCase = testcase;
		testcase->nTestResult = 1;
		testcase->nFailed = 0;
		testcase->nPassed = 0;
		std::cout << "==========================================" << std::endl;
		std::cout << " Run TestCase: " << testcase->testcase_name << std::endl;
		testcase->Run();
		if (testcase->nFailed == 0)
			std::cout << " ";
		else
		{
			std::cout << "";
			std::cout << " " << testcase->nPassed << " / " << testcase->nFailed + testcase->nPassed
				<< " Cases passed. ( " << testcase->nPassed / (testcase->nFailed + testcase->nPassed) * 100 << "% ) " << std::endl;
			std::cout << "green" << " End TestCase:" << testcase->testcase_name << std::endl;
		}

		if (testcase->nTestResult)
		{
			++nPassed;
		}
		else
		{
			++nFailed;
		}
	}

	std::cout<<" green "<<
}
} //end of namespace test

#endif