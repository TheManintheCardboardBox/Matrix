#include <cassert>
#include <type_traits>
#include <vector>
#include "Matrix.hpp"
#include "test_suite.hpp"

int main()
{
	testSuite tests;

	tests.addTest([]()
	{
		Matrix<double> A;

		Matrix<double> B{ 4, 4 };
		std::cout << B;

		std::vector<double> vec{ 1.0, 2.0, 3.0, 4.0, 5.0 };
		Matrix<double> C{ vec };
		std::cout << C;

		double arr[] = { 1.0, 2.0, 3.0, 4.0, 5.0 };
		Matrix<double> D{ arr, 5 };
		std::cout << D;

		Matrix<double> E{ D };
		std::cout << E;
		ASSERT_TRUE(E == D);
	},
		"Constructors.");

	tests.addTest([]()
	{
		static_assert(std::is_destructible<Matrix<double>>::value, "Matrix is not destructible.\n");
	},
		"Destructor.");

	tests.addTest([]()
	{
		Matrix<double> A{ 3, 5 };
		ASSERT_TRUE(A.size().first == 3);
		ASSERT_TRUE(A.size().second == 5);

		std::vector<double> vec1{ 1.0, 2.0, 3.0, 4.0, 5.0 };
		Matrix<double> B{ vec1 };
		ASSERT_TRUE(B.get(3, 0) == 4.0);

		double arr[] = { 1.0, 2.0, 3.0, 4.0, 5.0 };
		Matrix<double> C{ arr, 5 };
		ASSERT_TRUE(*C.data(4) == 5.0);

		std::vector<double> vec2{ 3.0, 4.0 };
		Matrix<double> D{ vec2 };
		ASSERT_TRUE(D.norm() == 5.0);

		Matrix<double> E{ 4, 4 };
		E.uniform_();
		std::cout << E;
	},
		"Methods.");

	tests.addTest([]()
	{
		std::vector<double> vec{ 1.0, 2.0, 3.0, 4.0, 5.0 };
		Matrix<double> A{ vec };
		Matrix<double> B;
		B = A;
	},
		"Assignment.");

	tests.addTest([]()
	{
		std::vector<double> vec1{ 1.0, 2.0, 3.0, 4.0, 5.0 };
		Matrix<double> A{ vec1 };
		Matrix<double> B{ A };

		std::vector<double> vec2{ 3.0, 4.0 };
		Matrix<double> C{ vec2 };

		ASSERT_TRUE(A == B);
		ASSERT_TRUE(A != C);
	},
		"Comparators.");

	tests.addTest([]()
	{
		std::vector<double> vec1{ 3.0, 3.0, 3.0, 3.0, 3.0 };
		Matrix<double> A{ vec1 };

		std::vector<double> vec2{ 2.0, 2.0, 2.0, 2.0, 2.0 };
		Matrix<double> B{ vec2 };

		std::vector<double> vec3{ 5.0, 5.0, 5.0, 5.0, 5.0 };
		Matrix<double> C{ vec3 };

		std::vector<double> vec4{ 1.0, 1.0, 1.0, 1.0, 1.0 };
		Matrix<double> D{ vec4 };

		ASSERT_TRUE((A + B) == C);
		ASSERT_TRUE((A - B) == D);

		Matrix<double> E{ 3, 3 };
		E.set(1.0, 0, 0);
		E.set(1.0, 1, 1);
		E.set(1.0, 2, 2);
		Matrix<double> F{ 3, 3 };
		F.uniform_();

		ASSERT_TRUE((E * F) == F);
		ASSERT_TRUE((F * E) == F);

		ASSERT_TRUE((C * 0.2) == D);
	},
		"Arithmetics.");

	tests.addTest([]()
	{
		Matrix<double> A{ 4, 4 }, b{ 4, 1 };
		A.uniform_();
		b.uniform_();

		solve(A, b);

		Matrix<double> x = solve(A, b);
		double r = (A * x - b).norm();
		ASSERT_TRUE(r < 0.001);
	},
		"Solve.");

	tests.run();

	return 0;
}
