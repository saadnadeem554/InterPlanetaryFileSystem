#pragma once
#include <iostream>
#include "machine.h"
#include "support.h"

using namespace std;

Machine::Machine(string& name, string& hash, int idBits, int order)
{
	data = 0;
	this->name = name;
	this->hash = hash;
	identifier_bits = idBits;
	head = 0;
	next = 0;
	this->order = order;
	data = new BTree;//(order);
}

void Machine::PrintRoutingTable()
{
	if (head == 0)
		return;
	else
	{
		RoutingTable* t1 = head;
		int x = 1;
		cout << "\n\nROUTING TABLE FOR MACHINE: " << name << endl;
		while (t1->next != 0)
		{
			cout <<"INDEX: "<<x << " |MACHINE NAME: " << t1->data->name << "  |MACHINE HASH/ID: " << t1->data->hash << endl;
			t1 = t1->next;
			//cout << endl;
			x++;
		}
		cout << "INDEX: " << x << " |MACHINE NAME: " << t1->data->name << "  |MACHINE HASH/ID: " << t1->data->hash << endl;

	}
}
void Machine::PrintBTree()
{
	data->traverse();
}
void Machine::CreateRouting(int size)
{
	Machine* temp;
	deleteTable();
	for (int x = 1; x <= identifier_bits; x++)
	{
		long long int calculate = pow(2, x - 1);
		string s1 = addHash(hash, calculate);

		//////////MOD NEEDS TO BE DONE BEST LOGIC CAN BE TO USE AND FUNCTIONALITY USING TRANSLATE FUNCTIONS  x % 2^n == x & (2^n - 1) as max identifier space is in power of 2
		s1 = modHash(s1, identifier_bits);
		
		temp = next;
		if(strcmp(this->hash, next->hash))
		{
			if (!strcmp(s1, this->hash))
			{
				while (strcmp(s1, temp->hash))
				{
					if (s1 == temp->hash)
						break;
					temp = temp->next;
					if (temp->next == next)
					{
						//temp = temp->next;
						break;
					}
				}
			}
		}
		else
		{
			if (s1 != temp->hash)
			{
				Machine * prev = this;
				while (strcmp(s1, temp->hash) || strcmp(prev->hash,s1))
				{
					if (s1 == temp->hash)
						break;
					temp = temp->next;
					prev = prev->next;
					if (temp->next == next)
					{
						//temp = temp->next;
						break;
					}
				}
				if (temp->next == next)
				{
					temp = prev;
				}
			}
		}

		RoutingTable* r1 = new RoutingTable(temp);
		if (head == 0)
			head = r1;
		else
		{
			RoutingTable* t1 = head;
			while (t1->next != 0)
			{
				t1 = t1->next;
			}
			t1->next = r1;
			r1->prev = t1;
		}
	}
}
void Machine::deleteTable()
{
	if (head == 0)
		return;
	else
	{
		RoutingTable* t1 = head;
		RoutingTable* t2 = head;
		while (t1->next != 0)
		{
			t1 = t1->next;
		}
		while (t1->prev != 0)
		{
			t2 = t1;
			t1 = t1->prev;
			delete t2;
		}
		t2 = t1;
		head = 0;
		delete t2;
	}
}
void Machine::insertData(string filename,string key, string content)
{	
	Machine* temp = this;
	do
	{
		if (strcmp(temp->hash , key) && strcmp(temp->next->hash , key)) // LESS THAN ROOT
		{
			string path = temp->hash + '\\' + filename;
			cout << "Inserting file: " << filename << " key: " << key << " Into Btree." << endl;
			temp->data->insert(key, path, content);
			return;
		}
		if (strcmp(key, temp->hash) && strcmp(temp->next->hash, key)) // GREATER THAT CURRENT LESS THAN NEXT
		{
			string path= temp->next->hash + '\\' + filename;
			cout << "Inserting file: " << filename << " key: " << key << " Into Btree." << endl;
			temp->next->data->insert(key, path, content);
			return;
		}
		if (temp->next == this)
		{
			if (strcmp(key, temp->hash) && strcmp(key, temp->next->hash)) //GREATER THAN FURTHEST MACHINE AND GREATER THAN ROOT ALLOCATED TO ROOT
			{
				string path = temp->next->hash + '\\' + filename;
				cout << "Inserting file: " << filename << " key: " << key << " Into Btree." << endl;
				temp->next->data->insert(key, path, content);
				return;
			}
		}
		temp = temp->next;
	}
	while (temp != this);
}
void Machine::deleteData(string key)
{
	Machine* temp = this;
	do
	{
		if (strcmp(temp->hash, key) && strcmp(temp->next->hash, key)) // LESS THAN ROOT
		{
			Data* t1 = temp->data->findsamekey(key);
			if (t1 != 0)
			{
				deleteFile(t1->filepath);
				Data* c1 = t1->chain;
				while (c1 != 0)
				{
					deleteFile(c1->filepath);
					c1 = c1->chain;
				}
			}
			temp->data->DeleteNode(key);
			return;
		}
		if (strcmp(key, temp->hash) && strcmp(temp->next->hash, key)) // GREATER THAT CURRENT LESS THAN NEXT
		{
			Data* t1 = temp->next->data->findsamekey(key);
			if (t1 != 0)
			{
				deleteFile(t1->filepath);
				Data* c1 = t1->chain;
				while (c1 != 0)
				{
					deleteFile(c1->filepath);
					c1 = c1->chain;
				}
			}
			temp->next->data->DeleteNode(key);
			return;
		}
		if (temp->next == this)
		{
			if (strcmp(key, temp->hash) && strcmp(key, temp->next->hash)) //GREATER THAN FURTHEST MACHINE AND GREATER THAN ROOT ALLOCATED TO ROOT
			{
				Data* t1 = temp->next->data->findsamekey(key);
				if (t1 != 0)
				{
					deleteFile(t1->filepath);
					Data* c1 = t1->chain;
					while (c1 != 0)
					{
						deleteFile(c1->filepath);
						c1 = c1->chain;
					}
				}
				temp->next->data->DeleteNode(key);
				return;
			}
		}
		temp = temp->next;
	} while (temp != this);
}
void Machine::search(string key)
{
	RoutingTable* temp2 = head;
	if (strcmp(hash, next->hash))
	{
		if (strcmp(key, hash) || (strcmp(next->hash, key)))
		{
			return next->data->search(key);
		}
	}
	if (strcmp(key, hash) && strcmp(temp2->data->hash, key))
	{
		return temp2->data->data->search(key);
	}
	temp2 = temp2->next;
	while (temp2 != 0)
	{
		if (strcmp(key, temp2->prev->data->hash) && strcmp(temp2->data->hash, key))
		{
			return temp2->prev->data->search(key);
		}
		if (temp2->next == 0)
		{
			return temp2->data->search(key);
		}
		temp2 = temp2->next;
	}
	data->search(key);
}