#include "PmergeMe.hpp"
#include <iostream>
#include <exception>

int main(int argc, char **argv)
{
	try
	{
		PmergeMe test(argc, argv);
		PmergeMe objekt(argc, argv);
			objekt.printContainerVector();
			test.sortVector();
		PmergeMe test1(argc, argv);
		PmergeMe objekt1(argc, argv);
			objekt1.printContainerDeque();
			test1.sortDeque();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}
