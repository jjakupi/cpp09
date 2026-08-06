#include "PmergeMe.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <climits>
#include <cstdlib>
#include <cerrno>
#include <algorithm>
#include <utility>

PmergeMe::PmergeMe()
{
}

PmergeMe::PmergeMe(int argc, char **argv)
{
	parseArguments(argc, argv);
}

PmergeMe::PmergeMe(const PmergeMe &other)
	: _input(other._input),
	  _vector(other._vector),
	  _deque(other._deque)
{
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	if (this != &other)
	{
		_input = other._input;
		_vector = other._vector;
		_deque = other._deque;
	}
	return *this;
}

PmergeMe::~PmergeMe()
{
}

void PmergeMe::sortVector()
{
	fordJohnsonVector(_vector);
}

void PmergeMe::sortDeque()
{
	fordJohnsonDeque(_deque);
}


void PmergeMe::parseArguments(int argc, char **argv)
{
	(void)argv;
	if (argc < 2)
		throw std::runtime_error("Error");

	for (int i = 1; i < argc; ++i)
	{
		char *end;
		errno = 0;
		long value = std::strtol(argv[i], &end, 10);

		if(end == argv[i] || *end != '\0' || errno == ERANGE || value <= 0 ||value > INT_MAX)
		{
			throw std::runtime_error("Error");
		}
		int number = static_cast<int>(value);
		for(std::size_t j = 0; j < _vector.size(); j++)
		{
			if(_vector[j] == number)
				throw std::runtime_error("Error");
		}
		_vector.push_back(number);
		_deque.push_back(number);
	}
}

void PmergeMe::printContainerVector() const
{
	std::cout << "Vector before: ";
	for(size_t i = 0; i < _vector.size(); i++)
	{
		std::cout << _vector[i] << " ";
	}
	std::cout << std::endl;
}

void PmergeMe::printContainerDeque() const
{
	std::cout << std::endl;
	std::cout << "Deque before: ";
	for(size_t j = 0; j < _deque.size(); j++)
	{
		std::cout << _deque[j] << " ";
	}
	std::cout << std::endl;
}


void PmergeMe::fordJohnsonVector(std::vector<int> &numbers)
{
	if (numbers.size() <= 1)
		return;

	std::vector<std::pair<int, int> >pairs;
	for (size_t i = 0; i + 1 < numbers.size(); i+=2)
	{
		int first = numbers[i];
		int second = numbers[i+1];
		if (first > second)
			std::swap(first, second);
		int small = first;
		int big = second;
		pairs.push_back(std::make_pair(small, big));
	}
	std::cout << std::endl;
	std::cout << "Pairs: ";

	for (std::size_t j = 0; j < pairs.size(); ++j)
	{
		std::cout << "(" << pairs[j].first << ", " << pairs[j].second << ") ";
	}
	bool hasStraggler = false;
	int straggler = 0;

	if (numbers.size() % 2 != 0)
	{
		hasStraggler = true;
		straggler = numbers.back();

		std::cout << std::endl;
		//std::cout << "Straggler: " << straggler;
	}
	std::cout << std::endl;
	//std::cout << "------------------------------------------------" << std::endl;
	std::vector<int> bigNumbers;
	// std::cout << "BigNumbers: ";
	for(size_t j = 0; j < pairs.size(); j++)
	{
		bigNumbers.push_back(pairs[j].second);
	// 	std::cout << bigNumbers[j]<< " ";
	 }
//	std::cout << std::endl;
	fordJohnsonVector(bigNumbers);

	std::cout << std::endl;
	// std::cout << "BigNumbers after recursion:=";

	// for (std::size_t j = 0; j < bigNumbers.size(); ++j)
	// std::cout << bigNumbers[j] << " ";
	// std::cout << std::endl;

	std::vector<int> mainChain;

	std::cout << std::endl;
	std::vector<std::pair<int, int> >orderedPairs;
	for(size_t i = 0; i < bigNumbers.size(); i++)
	{
		for(size_t j = 0; j < pairs.size(); j++)
		{

			if(pairs[j].second == bigNumbers[i])
			{
				orderedPairs.push_back(pairs[j]);
				break;
			}
		}
	}
	pairs = orderedPairs;
	//std::cout << std::endl;
	//std::cout << "ADDING: " << pairs[0].first;
	mainChain.push_back(pairs[0].first);
	std::cout << std::endl;
	for(std::size_t i = 0; i < bigNumbers.size(); i++)
	{
		mainChain.push_back(bigNumbers[i]);
	}
	insertPendingVector(mainChain, orderedPairs);
	if (hasStraggler)
	{
		//std::cout << "Inserting straggler: " << straggler << std::endl;
		std::vector<int>::iterator position = std::lower_bound(mainChain.begin(), mainChain.end(),straggler);
		mainChain.insert(position, straggler);
	}
	std::cout << std::endl;
	numbers = mainChain;
	std::cout << std::endl;
	for(std::size_t i = 0; i < numbers.size(); i++)
	{
		std::cout << numbers[i] << "-";
	}
	 std::cout << std::endl;
}

void PmergeMe::insertPendingVector(std::vector<int> &mainChain, const std::vector<std::pair<int, int> > &orderedPairs)
{
	if (orderedPairs.size() <= 1)
		return;

	size_t previous = 1;
	size_t current = 3;

	while (previous < orderedPairs.size())
	{
		size_t groupStart = previous + 1;
		size_t groupEnd = current;

		if (groupEnd > orderedPairs.size())
			groupEnd = orderedPairs.size();

		for (size_t i = groupEnd; ; --i)
		{
			int pending = orderedPairs[i - 1].first;
			int partner = orderedPairs[i - 1].second;

			std::vector<int>::iterator partnerIt =
				std::find(mainChain.begin(), mainChain.end(), partner);

			if (partnerIt == mainChain.end())
			{
				std::cerr << "Error: partner not found" << std::endl;
				return;
			}
			std::vector<int>::iterator position = std::lower_bound(mainChain.begin(), partnerIt, pending);

			//std::cerr << "Inserting the pending " << pending << std::endl;
			mainChain.insert(position, pending);

			if (i == groupStart)
				break;
		}

		size_t next = current + 2 * previous;
		previous = current;
		current = next;
	}
}

void PmergeMe::fordJohnsonDeque(std::deque<int> &numbers)
{
	if (numbers.size() <= 1)
		return;

	std::deque<std::pair<int, int> >pairs;
	for (size_t i = 0; i + 1 < numbers.size(); i+=2)
	{
		int first = numbers[i];
		int second = numbers[i+1];
		if (first > second)
			std::swap(first, second);
		int small = first;
		int big = second;
		pairs.push_back(std::make_pair(small, big));
	}
	std::cout << std::endl;
	std::cout << "Deque-Pairs: ";

	for (std::size_t j = 0; j < pairs.size(); ++j)
	{
		std::cout << "(" << pairs[j].first << ", " << pairs[j].second << ") ";
	}
	bool hasStraggler = false;
	int straggler = 0;

	if (numbers.size() % 2 != 0)
	{
		hasStraggler = true;
		straggler = numbers.back();

		// std::cout << std::endl;
		// std::cout << "Deque-Straggler: " << straggler;
	}
	std::cout << std::endl;
	//std::cout << "------------------------------------------------" << std::endl;
	std::vector<int> bigNumbers;
	//std::cout << "Deque-BigNumbers: ";
	for(size_t j = 0; j < pairs.size(); j++)
	{
		bigNumbers.push_back(pairs[j].second);
		//std::cout << bigNumbers[j]<< " ";
	}
	std::cout << std::endl;
	fordJohnsonVector(bigNumbers);

	std::cout << std::endl;
	//std::cout << "Deque-BigNumbers after recursion:=";

	//for (std::size_t j = 0; j < bigNumbers.size(); ++j)
//	std::cout << bigNumbers[j] << " ";
	std::cout << std::endl;

	std::deque<int> mainChain;

	std::cout << std::endl;
	std::deque<std::pair<int, int> >orderedPairs;
	for(size_t i = 0; i < bigNumbers.size(); i++)
	{
		for(size_t j = 0; j < pairs.size(); j++)
		{

			if(pairs[j].second == bigNumbers[i])
			{
				orderedPairs.push_back(pairs[j]);
				break;
			}
		}
	}
	pairs = orderedPairs;
	std::cout << std::endl;
	//std::cout << "deque-ADDING: " << pairs[0].first;
	mainChain.push_back(pairs[0].first);
	std::cout << std::endl;
	for(std::size_t i = 0; i < bigNumbers.size(); i++)
	{
		mainChain.push_back(bigNumbers[i]);
	}
	insertPendingDeque(mainChain, orderedPairs);
	if (hasStraggler)
	{
		//std::cout << "Deque-Inserting straggler: " << straggler << std::endl;
		std::deque<int>::iterator position = std::lower_bound(mainChain.begin(), mainChain.end(),straggler);
		mainChain.insert(position, straggler);
	}
	std::cout << std::endl;
	numbers = mainChain;
	std::cout << std::endl;
	for(std::size_t i = 0; i < numbers.size(); i++)
	{
		std::cout << numbers[i] << "-";
	}
	 std::cout << std::endl;
}

void PmergeMe::insertPendingDeque(std::deque<int> &mainChain, const std::deque<std::pair<int, int> > &orderedPairs)
{
	if (orderedPairs.size() <= 1)
		return;

	size_t previous = 1;
	size_t current = 3;

	while (previous < orderedPairs.size())
	{
		size_t groupStart = previous + 1;
		size_t groupEnd = current;

		if (groupEnd > orderedPairs.size())
			groupEnd = orderedPairs.size();

		for (size_t i = groupEnd; ; --i)
		{
			int pending = orderedPairs[i - 1].first;
			int partner = orderedPairs[i - 1].second;

			std::deque<int>::iterator partnerIt =
				std::find(mainChain.begin(), mainChain.end(), partner);

			if (partnerIt == mainChain.end())
			{
				std::cerr << "Error: partner not found" << std::endl;
				return;
			}
			std::deque<int>::iterator position = std::lower_bound(mainChain.begin(), partnerIt, pending);

		//	std::cerr << "Deque-Inserting the pending " << pending << std::endl;
			mainChain.insert(position, pending);

			if (i == groupStart)
				break;
		}
		size_t next = current + 2 * previous;
		previous = current;
		current = next;
	}
}
