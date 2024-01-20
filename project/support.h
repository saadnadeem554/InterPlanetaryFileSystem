#pragma once
#include<sstream>
#include <fstream>
using namespace std;

//////////////////////SUPPORT FUNCTIONS////////////////////////////
inline bool strcmp(string& s1, string& s2)
{
	int size = s1.size();
	for (int i = 0; i < size; i++)
	{
		if (s1[i] < s2[i])
			return 0;
		if (s1[i] > s2[i])
			return 1;
	}
	return 1;
}

inline int translate(char x)
{
	if (x >= 'a' && x <= 'f')
		return x - 87;
	else if (x >= '0' && x <= '9')
		return x - 48;
	else
		return 0;
}
inline char translate(int x)
{
	if (x >= 0 && x <= 9)
		return x + 48;
	else if (x >= 10 && x <= 15)
		return x + 87;
	else
		return 0;
}

inline string addHash(string h1, long long int calculate)
{
	int size = h1.size();
	int carry = 0;

	stringstream oss;
	oss << std::hex << calculate;
	string num = oss.str();
	int size2 = num.size();

	string ans;
	int size3 = size;
	ans.resize(size3);

	int h0 = 0;
	int d0 = 0;
	for (int i = size - 1, j = size2 - 1, k = size3 - 1; i >= 0 ; i--, j--, k--)
	{
		if (i >= 0)
			h0 = translate(h1[i]);
		else
			h0 = 0;
		if (j >= 0)
			d0 = translate(num[j]);
		else
			d0 = 0;
		int sum = h0 + d0 + carry;
		carry = sum / 16;
		sum = sum % 16;

		ans[k] = translate(sum);
	}
	int x = 0;
	for (; x < size3; x++)
	{
		if (translate(ans[x]))
			break;
		else
			ans[x] = '0';
	}
	//ans = ans.substr(x, size3 - 1);
	return ans;
}
inline string modHash(string hash, int maxBits)
{
	string maxHash;
	maxHash.resize(hash.size());
	int size= hash.size();
	for (int i = size - 1; i > 0; i--)
	{
		maxHash[i] = 'f';
	}
	int msb = maxBits % 4;
	switch (msb)
	{
	case 0:
		maxHash[0] = 'f';
		break;
	case 1:
		maxHash[0] = '1';
		break;
	case 2:
		maxHash[0] = '3';
		break;
	case 3:
		maxHash[0] = '7';
		break;
	}
	string newHash;
	newHash.resize(size);
	for (int i = 0; i < size; i++)
	{
		int h0 = translate(hash[i]);
		int m0 = translate(maxHash[i]);
		int sum = h0 & m0;
		newHash[i] = translate(sum);
	}
	return newHash;
}

inline std::string readEntireFile(const std::string& fileName) {
	std::ifstream file(fileName);

	if (!file.is_open()) {
		std::cerr << "Error opening file: " << fileName << std::endl;
		return "";
	}

	std::ostringstream contentStream;
	std::string line;

	// Read the file line by line
	while (std::getline(file, line)) {
		// Append the line to the string
		contentStream << line << '\n';
	}

	// Close the file
	file.close();

	// Get the content from the stringstream
	std::string content = contentStream.str();

	return content;
}
inline void deleteFile(string path)
{
	if (remove(path.c_str()) == 0) {
		cout << "File deleted successfully\n\n";
	}
}

///////////////////////////////////////////////////////////////////