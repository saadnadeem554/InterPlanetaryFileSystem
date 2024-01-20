#pragma once
#include "Data.h"
class BTreeNode
{
public:
	Data* keys;
	Data* chain;
	//Data* data;

	BTreeNode** Childptr;
	int numkeys;
	bool isleaf;

	BTreeNode(bool isLeaf);
	void search(string key);
	//void search2(string key, string& ans);

	void split(int index, BTreeNode* splitee);
	void insert2(string key, string filepath);
	void traverse();
	void rectrav(DataList& head);
	DataList CreateList();
	void findsamekey(string key, Data*& dat);


	void Delete(string k);
	int FindReplacementFromNext(int idx);
	int FindReplacementFromBack(int idx);
	
};
class BTree
{
public:
	BTreeNode* root;
	

	BTree();
	void search(string key);
	//string search2(string key);
	DataList CreateList();
	void insert(string key, string filepath, string content);
	void DeleteNode(string key);
	Data* findsamekey(string key);
	void traverse() 
	{
		if (root != NULL)
		{
			root->traverse();
		}
		else
			return;
	}
};




