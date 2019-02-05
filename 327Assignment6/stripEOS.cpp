/*

CISC327 Assignment 6

Stefan Urosevic   10146785
Zane Little       10179568
Meryl Gamboa      10173192

*/

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;


// Read a file into a vector, 
//  stopping at EOF (non-inclusive) 
vector<string> readFile(string fileName)
{
	//Creating the vector
	vector<string> vec;

	string line;

	//Opening the file
	ifstream file(fileName.c_str());
	if (file.is_open())
	{
		while (getline(file, line) && line != "EOS")
		{
			vec.push_back(line);
		}

		file.close();
	}
	else printf("Unable to open file.\n");

	return vec;
}

// Write a vector to a file.
void writeFile(vector<string> vec, string fileName)
{
	//open file
	ofstream file;
	file.open(fileName.c_str());

	if (file.is_open())
	{
		//write the list to the file
		int i;
		for (i = 0; i < vec.size(); i++)
		{
			file << (vec[i].c_str()) << '\n';
		}

		//Closing the file
		file.close();
	}
	else printf("Unable to open file.\n");
}


int main(int argc, char* argv[])
{
	//Getting the file names from the command line
	string fileName(argv[1]);

	vector<string> vec = readFile(fileName);
	writeFile(vec, fileName);

	return 0;
}