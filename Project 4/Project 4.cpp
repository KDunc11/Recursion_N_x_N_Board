#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Input
{
	char data = '\0'; //Holds character read in from file
	bool visitedIndex = false; //Keeps track if the index has been visited, default is false
};

void openAndProccesFile(string& fileName, char* argv[], int argc);
int findArea(int x, int y, Input inputArr[4500][3500], int outputArr[4500][3500], int row, int col, int treeCount);


int main(int argc, char* argv[])
{
	if (argc < 2) //If the argument count is only 1 or 2, then the program cannot open a source file 
		cout << "You must have at least 2 arguments, the program name and source file, 3 arguments for a destination of output" << "\n";

	if (argc > 2 && argc < 5) //Opens source file and processes the information
	{
		string fileName;
		fileName = argv[2];

		openAndProccesFile(fileName, argv, argc); //Calls funtion to handle processing the file
	}

	if (argc == 4)
	{
		string output = argv[3];
		cout << "Data was outputted to the specified source " << output << "\n";
	}

	cout << "\n_____Done running program, press enter to close the program_____" << endl;
	cin.ignore();
	return 0;
}

void openAndProccesFile(string& fileName, char* argv[], int argc)
{
	fstream inFile;

	inFile.open(fileName); //Opens a specified file

	if (inFile.is_open()) //Tries to open file
	{
		cout << "File was successfully opened"  << "\n" << endl;
		char ch, temp;
		int rowSize = 0;
		int colSize = 0;
		bool run_processing = true; 
		int treeCount = 0;

		Input inputArr[4500][3500]; //Creates an array to store the input data
		int outputArr[4500][3500]; //Creates an array to store the data to be outputted either to the console or to a file

		inFile >> temp >> temp >> rowSize >> temp >> temp >> temp >> colSize; //Used to read in the size of columns in rows, ignoring other characters

		for (int i = 0; i < rowSize; i++) //Reads in the characters from the file
		{
			for (int j = 0; j < colSize; j++)
			{
				inFile >> ch;
				inputArr[i][j].data = ch;
			}
		}

		for (int i = 0; i < rowSize; i++) //Goes through entire input array and checks for the forested area
		{
			for (int j = 0; j < colSize; j++)
			{
				if (inputArr[i][j].visitedIndex == false) //If the index hasn't been visited aleady then proceed
				{
					if (inputArr[i][j].data == 'g') //Checks for grassland
					{
						outputArr[i][j] = 0; //0 represents grass on the output array
						inputArr[i][j].visitedIndex = true; //marks the index as visited
					}
					else if (inputArr[i][j].data == 't') //Checks for trees
					{
						++treeCount; //If a tree is found then increment the area
						findArea(i, j, inputArr, outputArr, rowSize + 2, colSize + 2, treeCount); //Searches for more trees in the cluster
					}
				}
			}
		}

		cout << "_____Input_____" << "\n";

		for (int i = 0; i < rowSize; i++) //Displays the input array
		{
			for (int j = 0; j < colSize; j++)
			{
				cout << inputArr[i][j].data << " " << " ";
			}
			cout << endl;
		}

		cout << "\n" << "Found " << treeCount << " Different Areas of trees" << "\n" << endl; //Prints to the console the total number of different areas of trees found

		inFile.close(); //Closes the file

		if (argc == 4)
		{
			string outputFileName;
			outputFileName = argv[3]; //Sets the output file to be what was specified in the command arguments

			ofstream outFile;

			outFile.open(outputFileName); //Creates or overwrites the output file

			if (outFile.is_open()) //If output file can be opened then proceed
			{
				for (int i = 0; i < rowSize; i++) //Writes the output array into the output file
				{
					for (int j = 0; j < colSize; j++)
					{
						outFile << outputArr[i][j] << " " << " ";
					}
					outFile << endl;
				}
			}
			else
				cout << "\nFile could not be opened" << endl;

			outFile.close(); //Closes the output file
		}
		else
		{
			cout << "_____Output_____" << "\n";

			for (int i = 0; i < rowSize; i++) //Displays the output array to the console
			{
				for (int j = 0; j < colSize; j++)
				{
					cout << outputArr[i][j] << " " << " ";
				}
				cout << endl;
			}
		}
		run_processing = false;
	}
}

int findArea(int x, int y, Input inputArr[4500][3500], int outputArr[4500][3500], int row, int col, int treeCount)
{
	if (!inputArr[x][y].visitedIndex && inputArr[x][y].data == 't') //If the index hasn't been visited and is a tree proceeds
	{
		outputArr[x][y] = treeCount; //Marks the area on the output array 
		inputArr[x][y].visitedIndex = true; //Changes the index's visited condition to true

		/*Recursively call itself based on the following conditions to calculate the array of tree cluster*/
		if (x != 0) //Checks if possible to move up a row
			findArea(x - 1, y, inputArr, outputArr, row, col, treeCount); //Moves up one index
		if (x != row - 1) //Checks if possible to move down a row 
			findArea(x + 1, y, inputArr, outputArr, row, col, treeCount); //Moves down one index
		if (y != 0) //Checks if possible to move left in a column
			findArea(x, y - 1, inputArr, outputArr, row, col, treeCount); //Moves left one index
		if (y != col - 1) //Checks if possible to move right in a column
			findArea(x, y + 1, inputArr, outputArr, row, col, treeCount); //Moves right one index
	}
	return 0; //Returns 0 if the index was already visited, the index was grassland, or the function is finished calculating the array of a cluster of trees 
}