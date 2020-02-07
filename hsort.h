#ifndef HSORT_H
#define HSORT_H

#include <vector>
#include <iostream>

using namespace std;

template <typename T, typename Comparator >
void hsort(std::vector<T>& data, Comparator c = Comparator());

template <typename T, typename Comparator >
void heapify(std::vector<T>& data, size_t loc, size_t effsize, Comparator& c );

template <typename T, typename Comparator >
void hsort(std::vector<T>& data, Comparator c)
{
	size_t loc = data.size();
	while(loc != 0)
	{
		// from a vector to a heap tree
		heapify(data, loc, 0, c);
		loc--;
	}
	size_t effsize = 0;
	while(effsize != data.size())
	{
		T temp = data[0];
		data[0] = data[data.size()- 1 - effsize];
		data[data.size()-1 - effsize] = temp;
		effsize++;
		heapify(data, 1, effsize, c);
	}
	size_t i = 0;
	while(i < data.size()/2)
	{
		T temp = data[i];
		data[i] = data[data.size() - 1 - i];
		data[data.size() - 1 - i] = temp;
		i++;
	}
}

template <typename T, typename Comparator >
// loc is the location to start heapify
// effsize is the number of items in the vector that are part of heap
void heapify(std::vector<T>& data, size_t loc, size_t effsize, Comparator& c )
{
	size_t left_child = 2*loc;
	size_t right_child = 2*loc+1;
	// stop at the child
	if(left_child > (data.size()-effsize) && right_child > (data.size()-effsize))
	{
		return;
	}
	// if the loc is not a leaf node, it mush has the left child
	// Assum the smaller child is the left one
	size_t smaller_child = left_child;
	if(right_child <= (data.size() - effsize))
	{
		if(c(data[right_child - 1], data[left_child - 1]))
		//if(c(data[right_child - 1], data[left_child - 1]))
		{
			smaller_child = right_child;
		}
	}
	//if the smaller_child meets the compare requirment
	if(c(data[smaller_child - 1],data[loc-1]))
	//if(c(data[smaller_child - 1], data[loc - 1]))
	{
		// swap the place of the child and its parent
		T temp = data[loc - 1];
		data[loc - 1] = data[smaller_child - 1];
		data[smaller_child - 1] = temp;
		// do the same process for its size
		heapify(data, smaller_child, effsize, c);
	}
}
#endif