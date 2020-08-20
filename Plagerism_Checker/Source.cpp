#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

float GetSimilarity(string testFilePath, string ogFilePath);

int main()
{
	cout << "Enter the directory containing the files" << endl;

	string dir;
	cin >> dir;
	ofstream results;
	results.open("results.csv", ofstream::trunc);
	
	//Loops through the test files
	for (int i = 1; i <= 100; i++)
	{
		// Get full filepath to test file
		string testFilePath = dir;
		testFilePath += "/" + to_string(i) + ".txt";

		// Loops through the original files
		for (int j = 0; j < 5; j++)
		{
			// Get full filepath to original file
			string ogFilePath = dir;

			switch (j)
			{
			case 0:
				ogFilePath += "/a.txt";
				break;
			case 1:
				ogFilePath += "/b.txt";
				break;
			case 2:
				ogFilePath += "/c.txt";
				break;
			case 3:
				ogFilePath += "/d.txt";
				break;
			case 4:
				ogFilePath += "/e.txt";
				break;
			}

			float similarity = GetSimilarity(testFilePath, ogFilePath);
			
			// Writes results to result file
			results << similarity << ",";
			cout << similarity << ",";
		}

		results << endl;
		cout << endl;
	}
	
	results.close();

	return 0;
}

/// Returns similarity between two files.
/// Similarity value is between 0 to 1, where 0 means the two files are unique, and 1 means they are exactly the same.
float GetSimilarity(string testFilePath, string ogFilePath)
{
	// Get file contents
	ifstream testFileStream(testFilePath);
	ifstream ogFileStream(ogFilePath);

	if (!testFileStream || !ogFileStream)
	{
		cerr << "Error cannot find files. Directory must contain files 1 to 100.txt and a to e.txt" << endl;
		return -1.0;
	}

	string testFile((istreambuf_iterator<char>(testFileStream)), (istreambuf_iterator<char>()));
	string ogFile((istreambuf_iterator<char>(ogFileStream)), (istreambuf_iterator<char>()));

	testFileStream.close();
	ogFileStream.close();

	// Matrix that holds all distances
	vector<vector<int>> distMatrix(ogFile.length(), vector<int>(testFile.length(), 0));

	// Initialization of Levenshtein Algorithm
	for (unsigned int i = 1; i < ogFile.length(); i++)
	{
		distMatrix[i][0] = i;
	}

	for (unsigned int j = 1; j < testFile.length(); j++)
	{
		distMatrix[0][j] = j;
	}

	// Recurrence Relation
	for (unsigned int j = 1; j < testFile.length(); j++)
	{
		for (unsigned int i = 1; i < ogFile.length(); i++)
		{
			int deletion = distMatrix[i-1][j] + 1;
			int insertion = distMatrix[i][j-1] + 1;
			int substitution = distMatrix[i-1][j-1];
			int minimum;

			// Compare i-th character of test file with j-th character of original file
			if (testFile.at(j) != ogFile.at(i))
			{
				substitution += 1;
			}

			// Get minimum value between deletion, insertion, and subsitution
			minimum = min(deletion, insertion);
			minimum = min(minimum, substitution);

			distMatrix[i][j] = minimum;
		}
	}

	// Return final distance	
	int levenshtein = distMatrix[ogFile.length()-1][testFile.length()-1];
	float percent = (float)(max(testFile.length(), ogFile.length()) - levenshtein) / (float)max(testFile.length(), ogFile.length());

	return percent;
}