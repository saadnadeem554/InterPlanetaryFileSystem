#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include "taskManagementSystem.h" 
#include <Windows.h>
#include "sha1.hpp"
//#include "support.h"
using namespace std;

taskManagementSystem::taskManagementSystem()
{
	numberMachines = 0;
	int no_machines;
	long double test = 0;

	do
	{

		cout << "ENTER NUMBER OF IDENTIFIER BITS: \t";
		cin >> identifier_bits;

		if (identifier_bits <= 0)
			cout << "\n\nIDENTIFIER BITS CANNOT BE 0 or less\n\n";
	} 
	while (identifier_bits <= 0);

	//cout<<"ENTER ORDER OF B-TREE: \t";
	//cin >>order;

	do
	{
		cout << "ENTER NUMBER OF MACHINES: \t";
		cin >> no_machines;

		test = pow(no_machines, 0.5);
		if ((test > identifier_bits))
			cout << "\n\nMACHINES CANNOT EXCEED POSIBLE FILES\n\n";
	}
	while (test > identifier_bits);

	cout << "PROCESS TO INSERT MACHINES\n\n";

	for(int i=0;i<no_machines;i++)
	{
		insertMachine();
	}
	Machine* temp = head;
}

string fullHash(string s1)
{
	SHA1 checksum;
	checksum.update(s1);
	string s2 = checksum.final();
	return s2;
}
string taskManagementSystem::hashingFunc(string s1)
{
	SHA1 checksum;
	checksum.update(s1);
	string s2= checksum.final();
	int bits = identifier_bits;
	bits/= 4;
	int upper = identifier_bits % 4;

	int size = s2.size();
	size--;
	if(upper == 0)
		s2 = s2.substr(size - bits + 1, size);
	else
	{
		s2 = s2.substr(size - bits, size);
		char msb = s2[0];
		int temp;
		if (msb >= 48 && msb <= 57)
		{
			temp = msb - 48;
		}
		else
		{
			switch (msb)
			{
			case 'f':
				temp = 15;
				break;
			case 'e':
				temp = 14;

				break;
			case 'd':
				temp = 13;

				break;
			case 'c':
				temp = 12;

				break;
			case 'b':
				temp = 11;

				break;
			case 'a':
				temp = 10;
				break;

			}
		}
		temp &= int(pow(2,upper))-1;
		if (temp >= 0 && temp <= 9)
		{
			msb = temp + 48;
		}
		else
		{
			switch (temp)
			{
			case 10:
				msb = 'a';
				break;
			case 11:
				msb = 'b';
				break;
			case 12:
				msb = 'c';
				break;
			case 13:
				msb = 'd';
				break;
			case 14:
				msb = 'e';
				break;
			case 15:
				msb = 'f';
				break;

			}
		}
		s2[0] = msb;
	}



	return s2;
}
void taskManagementSystem::displayRing()
{
	if (head == 0)
		return;
	cout << "MACHINES IN RING DHT\n\n";
	Machine* temp = head;
	while (temp->next != head)
	{
		cout << temp->name << " | " << temp->hash << " --> ";
		temp = temp->next;
	}
	cout << temp->name << " | " << temp->hash << endl;

}
void taskManagementSystem::displayRoutingTable()
{
	if (head == 0)
		return;
	string key;
	cout << "ENTER MACHINE KEY YOU WANT TO ROUTING TABLE OF\n\n";
	cin.ignore();

	getline(cin, key);
	Machine* temp = head;

	do
	{
		if (temp->hash == key)
		{
			temp->PrintRoutingTable();
			break;
		}
		temp = temp->next;
	}
	while (temp != head);

}
void taskManagementSystem::printBT()
{
	if (head == 0)
		return;
	string key;
	cout << "\n\nnote: cin.ignore() is used if it looks like infinite loop then press enter\n\n";

	cout << "ENTER MACHINE KEY YOU WANT THE BTREE OF\n\n";
	cin.ignore();

	getline(cin, key);
	Machine* temp = head;
	do
	{
		if (temp->hash == key)
		{
			temp->PrintBTree();
			break;
		}
		temp = temp->next;
	} while (temp != head);
	cout << endl;
}
void taskManagementSystem::insertMachine()
{
	char choice;
	string manualID;
	Machine* temp = 0;
	cout << "WOULD YOU LIKE TO ASSIGN A MACHINE ID? (y/n)\n\n";
	cin >> choice;
	if (choice == 'y')
	{
		cout << "ENTER MACHINE ID:\t";
		cin.ignore();
		getline(cin,manualID);
		string hash = manualID;
		string tempHash = hash;
		do
		{
			hash = hashingFunc(tempHash);
			tempHash = fullHash(tempHash);
		} while (machCMP(hash));
		temp = new Machine(manualID, hash, identifier_bits, order);
	}
	else
	{
		cout << "GENERATING MACHINE ID....\n";
		manualID = generateID();
		string hash = manualID;
		string tempHash = hash;
		do
		{
			hash = hashingFunc(tempHash);
			tempHash = fullHash(tempHash);
		} while (machCMP(hash));
		temp = new Machine(manualID, hash, identifier_bits, order);
		machID++;
	}
	if (head == NULL)
	{
		head = temp;
		temp->next = head;
	}
	else
	{
		Machine* temp2 = head;
		bool flag = 1;
		if (strcmp(head->hash, temp->hash))
		{
			while (temp2->next != head)
			{
				temp2 = temp2->next;
			}
			temp2->next = temp;
			temp->next = head;
			head = temp;
			flag = 0;
		}
		while (temp2->next != head && flag)
		{
			if (strcmp(temp->hash, temp2->hash) && strcmp(temp2->next->hash, temp->hash))
			{
				temp->next = temp2->next;
				temp2->next = temp;
				flag = 0;
				break;
			}
			temp2 = temp2->next;
		}
		if (flag)
		{
			temp->next = temp2->next;
			temp2->next = temp;
		}
	}
	numberMachines++;

	std::wstring wideDirectoryName(temp->hash.begin(), temp->hash.end());
	LPCWSTR directoryNameW = wideDirectoryName.c_str();

	if (CreateDirectoryW(directoryNameW, NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
		std::cout << "Folder created successfully.\n";
	}
	else {
		std::cerr << "Error creating the folder.\n";
	}

	// At this point machine is stored in ring dht
	
	//Functionality for bTree Splitting and Routing table creation
	DataList temp2 = temp->next->data->CreateList();

	Data* temp3 = temp2.head;
	while (temp3 != 0)
	{
		if (strcmp(temp->next->hash, temp->hash))//IF MACHINE IS NOT INSERTED AT TAIL THEN ONLY DATA LESS THAN MACHINE WILL BE ALLOCATED TO CURRENT MACHINE
		{
			if (strcmp(temp->hash, temp3->key))
			{
				string path = temp3->filepath;
				string content = readEntireFile(path);
				path = path.substr(temp->hash.size(), path.size() - 1);
				path = temp->hash + path;
				temp->next->data->DeleteNode(temp3->key);
				deleteFile(temp3->filepath);
				temp->data->insert(temp3->key, path, content);
			}
		}
		else // IF MACHINE INSERTED AT TAIL THEN ONLY DATA LESS THAN MACHINE AND GREATER THAN ROOT WILL BE ALLOCATED TO CURRENT MACHINE
		{
			if (strcmp(temp->hash, temp3->key) && strcmp(temp3->key,temp->next->hash) && temp3->key!= temp->next->hash)
			{
				string path = temp3->filepath;
				string content = readEntireFile(path);
				path = path.substr(temp->hash.size(), path.size() - 1);
				path = temp->hash + path;
				temp->next->data->DeleteNode(temp3->key);
				deleteFile(temp3->filepath);
				temp->data->insert(temp3->key, path, content);
			}
		}
		temp3 = temp3->next;
	}

	Machine* temp4 = head;
	do
	{
		temp4->CreateRouting(numberMachines);
		temp4 = temp4->next;
	} 
	while (temp4->next != head);
	temp4->CreateRouting(numberMachines);
}
void taskManagementSystem::deleteMachine()
{
	string key;
	Machine* temp = 0;
	cout << "ENTER MACHINE KEY YOU WANT TO DELETE\n\n";
	cin.ignore();
	getline(cin, key);
	
	if (head == NULL)
	{
		return;
	}
	else
	{
		Machine* temp2 = head;
		Machine* prev = head;
		while (prev->next != head)
		{
			prev = prev->next;
		}
		do
		{
			if (temp2->hash == key)
			{
				temp = temp2;
				break;
			}
			prev = prev->next;
			temp2 = temp2->next;
		} while (temp2 != head);
		if (temp == 0)
		{
			cout << "MACHINE NOT FOUND!!!\n\n";
			return;
		}
		if (temp == head)
		{
			head = head->next;
			prev->next= head;
		}
		prev->next = temp2->next;

		DataList list = temp->data->CreateList();
		Data* temp3 = list.head;

		if (temp->next == temp)
		{
			while (temp3 != 0)
			{
				deleteFile(temp3->filepath);
				temp3 = temp3->next;
			}
			head = 0;
		}
		else
		{


			while (temp3 != 0)
			{
				string path = temp3->filepath;
				string content = readEntireFile(path);
				path = path.substr(temp->hash.size(), path.size() - 1);
				path = temp->next->hash + path;
				temp->data->DeleteNode(temp3->key);
				deleteFile(temp3->filepath);
				temp->next->data->insert(temp3->key, path, content);
				temp3 = temp3->next;
			}
		}

	}

	std::wstring wideDirectoryName(temp->hash.begin(), temp->hash.end());
	LPCWSTR directoryNameW = wideDirectoryName.c_str();

    // Remove the directory
    if (RemoveDirectoryW(directoryNameW) || ERROR_DIR_NOT_EMPTY == GetLastError()) {
        std::wcout << L"Directory removed successfully.\n";
    } else {
        std::wcerr << L"Error removing the directory.\n";
    }
	numberMachines--;

	
	if (head != 0)
	{
		Machine* temp4 = head;
		do
		{
			temp4->CreateRouting(numberMachines);
			temp4 = temp4->next;
		} while (temp4->next != head);
		temp4->CreateRouting(numberMachines);
	}
}

string taskManagementSystem::generateID()
{
	string mach = "Machine ";
	mach+= char(machID + 48);
	return mach;
}
void taskManagementSystem::insertData()
{
	string filename;
	string content;

	cout << "\n\nnote: cin.ignore() is used if it looks like infinite loop then press enter\n\n";

	cin.ignore();
	cout<<"ENTER FILE NAME: \t";
	getline(cin, filename);

	cout<<"ENTER FILE CONTENT: \t";
	getline(cin, content);

	string hash = hashingFunc(content);

	head->insertData(filename, hash, content);
}
void taskManagementSystem::removeData()
{
	string key;
	cout << "\n\nnote: cin.ignore() is used if it looks like infinite loop then press enter\n\n";

	cin.ignore();
	cout << "ENTER FILE KEY: \t";
	getline(cin, key);

	head->deleteData(key);

}
void taskManagementSystem::search()
{
	string key;
	cout << "\n\nnote: cin.ignore() is used if it looks like infinite loop then press enter\n\n";

	cin.ignore();
	cout << "ENTER KEY: \t";
	getline(cin, key);


	string query;
	cout << "ENTER MACHINE NAME TO START QUERY FROM: \t";
	getline(cin, query);

	Machine* temp = head;
	string path;
	do
	{
		if (temp->name == query)
		{
			temp->search(key);
			break;
		}
		temp = temp->next;
	} while (temp != head);

}

void taskManagementSystem::menu()
{
	bool flag = 1;

	system("cls");

	while (flag)
	{
		cout << "WELCOME TO TASK MANAGEMENT SYSTEM\n\n";
		cout << "ENTER OPTIONS:- \n\n";

		cout << "1. INSERT MACHINE\n2. DELETE MACHINE\n3. INSERT FILE\n4. DELETE FILE\n5. PRINT BTREE\n6. PRINT ROUTING TABLE\n7. DISPLAY RING\n8. Search\n9. LOGOUT\n";
		int choice;
		cin >> choice;

		switch (choice)
		{
		case 1:
			insertMachine();
			break;
		case 2:
			deleteMachine();
			break;
		case 3:
			insertData();
			break;
		case 4:
			removeData();
			break;
		case 5:
			printBT();
			break;
		case 6:
			displayRoutingTable();
			break;
		case 7:
			displayRing();
			break;
		case 8:
			search();
			break;
		case 9:
			flag = 0;
			break;
		default:
			cout << "INVALID CHOICE\n\n";
			break;
		}
		system("pause");
		system("cls");
	}
}
taskManagementSystem::~taskManagementSystem()
{
	system("cls");
	cout << "INTITIATING SHUTDOWN PROCEDURE...";
	Machine* temp = head;
	if (head != 0)
	{
		do
		{
			DataList list = temp->data->CreateList();
			Data* temp2 = list.head;
			while (temp2 != 0)
			{
				if (remove(temp2->filepath.c_str()) == 0) {
					cout << "File deleted successfully from " << temp->name << "\n\n";
				}
				else {
					cout << "Error deleting the file.\n";
				}
				temp2 = temp2->next;
			}
			std::wstring wideDirectoryName(temp->hash.begin(), temp->hash.end());
			LPCWSTR directoryNameW = wideDirectoryName.c_str();

			// Remove the directory
			if (RemoveDirectory(directoryNameW) || ERROR_DIR_NOT_EMPTY == GetLastError()) {
				std::wcout << L"Directory removed successfully.\n";
			}
			else {
				std::wcerr << L"Error removing the directory.\n";
			}
			numberMachines--;

			temp = temp->next;
		} while (temp != head);
	}
	if (numberMachines == 0)
	{
		cout << "ALL MACHINES AND FILES DELETED, DIRECTORY IS EMPTY...\n\n";
		system("pause");	
	}
}
bool taskManagementSystem::machCMP(string key)
{
	if (head == 0)
		return 0;
	Machine* temp = head;
	do
	{
		if (temp->hash == key)
			return 1;
		temp = temp->next;
	} while (temp != head);
	return 0;
}