#pragma once
#include "machine.h"
#include "support.h"

class taskManagementSystem
{
public:
	Machine* head;
	int identifier_bits;
	int machID;
	int numberMachines;
	int order;

	taskManagementSystem();

	string hashingFunc(string s1);
	void insertMachine();
	void deleteMachine();
	string generateID();
	void insertData();
	void removeData();
	void search();
	void menu();
	void displayRing();
	void displayRoutingTable();
	void printBT();
	bool machCMP(string key);





	~taskManagementSystem();
};

