#pragma once
#include <string>
using namespace std;

class Data
{
public:
	string key;
	string filepath;
	Data* next;
	Data* chain;
	Data()
	{
		next = nullptr;
		chain = nullptr;
	}
	Data(string& k1, string& c1)
	{
		key = k1;
		filepath = c1;
		next = nullptr;
		chain = nullptr;
	}
};

class DataList
{
public:
	Data* head;
	DataList()
	{
		head = nullptr;
	}
	void add(Data& node)
	{
		Data* n1 = new Data(node.key, node.filepath);
		if (head == nullptr)
		{
			head = n1;
			return;
		}
		Data* tmp = head;
		while (tmp->next != nullptr)
		{
			tmp = tmp->next;
		}
		tmp->next = n1;
	}
	void deleteList(Data* temp)
	{
		if (temp == 0)
		{
			return;
		}
		else
		{
			deleteList(temp->next);
			temp->next = 0;
			return;
		}
	}
	~DataList()
	{
		deleteList(head);
		head = nullptr;
	}
};

