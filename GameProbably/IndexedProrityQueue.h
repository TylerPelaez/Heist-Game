#pragma once
#include <vector>
#include <list>
#include <array>

class IndexedProrityQueue
{

	std::vector<float> keys;
	std::vector<int> data;

	void swap( int i, int j )
	{
		int t = data[i];
		data[i] = data[j];
		data[j] = t;
	}

public:
	void reorderUp()
	{
		int a = data.size() - 1;
		while ( a > 0 )
		{
			if ( keys[data[a]] < keys[data[a - 1]] )
			{
				swap( a, a - 1 );
			}
			else
			{
				return;
			}
			a--;
		}
	}

	void reorderDown()
	{
		for ( int a = 0; a < ( ((int) data.size()) - 1); a++ )
		{
			if ( keys[data[a]] > keys[data[a + 1]] )
			{
				swap( a, a + 1 );
			}
			else
			{
				return;
			}
		}
	}

	IndexedProrityQueue( std::vector<float> _keys )
	{
		keys = _keys;
		data = std::vector<int>();
	}

	int pop()
	{
		int r = data[0];
		data[0] = data[data.size() - 1];
		data.erase( data.begin() );
		reorderDown();
		return r;
	}

	void insert( int i)
	{
		data.push_back( i );
		reorderUp();
	}

	bool isEmpty()
	{
		return data.size() == 0;
	}
};
