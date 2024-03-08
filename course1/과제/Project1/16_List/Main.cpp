#include <list>
#include "List.h"
#include <iostream>

using namespace std;

int main()
{
	List<int> li;
	

	li.push_back(1);
	li.push_back(2);
	li.push_back(3);
	li.push_back(4);
	li.push_back(5);
	li.push_front(1);
	li.push_front(2);
	li.push_front(3);
	li.push_front(4);
	li.push_front(5);

	for (auto it = li.begin(); it != li.end(); ++it)
	{
		cout << *it << endl;
	}

	for (auto it = li.begin(); it != li.end();)
	{
		if (*it == 3)
		{
			it = li.erase(it);
		}
		else
		{
			++it;
		}
	}

	cout << endl << endl;

	for (auto it = li.begin(); it != li.end(); ++it)
	{
		cout << *it << endl;
	}
	
	cout << endl << endl;

	li.remove(1);

	for (auto it = li.begin(); it != li.end(); ++it)
	{
		cout << *it << endl;
	}

	cout << endl << endl;

	while (!li.empty())
	{
		cout << li.back() << endl;
		li.pop_back();
	}

	cout << "pop_back" << endl;

	for (auto it = li.begin(); it != li.end(); ++it)
	{
		cout << *it << endl;
	}

	li.push_back(1);
	li.push_back(2);
	li.push_back(3);
	li.push_back(4);
	li.push_back(5);
	li.push_front(1);
	li.push_front(2);
	li.push_front(3);
	li.push_front(4);
	li.push_front(5);

	li.clear();

	cout << "clear" << endl;

	for (auto it = li.begin(); it != li.end(); ++it)
	{
		cout << *it << endl;
	}

	return 0;
}