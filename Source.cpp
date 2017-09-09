#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include <cmath>
#include <algorithm>

using namespace std;

int readInt(istream& in) {
	int value;
	in >> value;
	if (in.good()) {
		return value;
	}
	else {
		// not cleaning up stream because program will exit on errors
		return -1;
	}
}

void printMapNumbers(ostream& out, vector<vector<int> >& m) 
{
	for (int i = 0; i < m.size(); i++) 
	{
		out << "Row " << i << endl;
		for (int j = 0; j < m.at(i).size(); j++) 
		{
			out << m.at(i).at(j) << " ";
		}
		out << endl;
	}
}

// TODO: Break this up into smaller functions
// calculates and colors the maroon paths across the map
void greedyPaths(int ncols, int nrows, vector<double>& r_values, vector<double>& g_values, vector<double>& b_values, vector<double>& r2_values, vector<double>& g2_values, vector<double>& b2_values, vector<double>& temp) {
	
	// This "for" loop starts the paths at the west most part of the map and progresses south until the end of the map
	for (int k = 0; k < nrows - 1; k++) 
	{
		int i = 0;
		int j = k;
		double change = 0;

		// This "for" loop beins at a point decided by the above "for" loop and progresses East, coloring the route maroon to the end of the map
		for (i; i < (ncols - 1); i++) 
		{

			// the "move south-east" variable. initialized to 1000 if at the bottom of the map.
			double b = 1000;
			if (j < (nrows - 1)) 
			{
				b = fabs(r_values.at(i + (j * (ncols))) - r_values.at((i + 1) + (j + 1) * (ncols)));
			}

			// the "move north-east" variable. initialized to 1000 if at the top of the map. 
			double c = 1000;
			if (j != 0) 
			{
				c = fabs(r_values.at(i + (j * (ncols))) - r_values.at((i + 1) + (j - 1) * (ncols)));
			}

			// the "move east" variable.
			double a = fabs(r_values.at(i + (j * (ncols))) - r_values.at((i + 1) + j * (ncols)));


			// greedy logic in these "if" and "else if" statements
			if ((c < b) && (c < a) && (j != 0)) 
			{
				j = j - 1;
				change = change + c;
			}

			else if ((b <= c) && (b < a) && (j != (nrows - 1))) 
			{
				j = j + 1;
				change = change + b;
			}

			else if ((a <= b) && (a <= c)) 
			{
				j = j;
				change = change + a;
			}

			// second set of identical vectors are colored so that the first set will remain uncolored and not affect the logic
			r2_values.at(i + (j * (ncols))) = 80;
			g2_values.at(i + (j * (ncols))) = 0;
			b2_values.at(i + (j * (ncols))) = 0;
		}
		j = 0;

		// stores change in elevation for the greedyGold function later on
		temp.push_back(change);
	}
}

// calculates the gold path based on the list of elevation changes found in the vector temp
void greedyGold(vector<double>& temp, int ncols, int nrows, vector<double>& r_values, vector<double>& g_values, vector<double>& b_values, vector<double>& r2_values, vector<double>& g2_values, vector<double>& b2_values) 
{

	// calculates the min of temp and also finds the index of the min number
	double index = - 7;
	double min = temp.at(0);
	for (int i = 1; i < temp.size(); i++) {
		if (min > temp.at(i)) {
			min = temp.at(i);
			index = i;
		}
	}


	// copied and pasted from greedyPaths function above with slight modification
	// dosn't use temp vector or 2nd "for" loop since it only has to execute once
		int i = 0;
		int j = index;

		for (i; i < (ncols - 1); i++) 
		{

			double b = 1000;
			if (j < (nrows - 1)) 
			{
				b = fabs(r_values.at(i + (j * (ncols))) - r_values.at((i + 1) + (j + 1) * (ncols)));
			}

			double c = 1000;
			if (j != 0) 
			{
				c = fabs(r_values.at(i + (j * (ncols))) - r_values.at((i + 1) + (j - 1) * (ncols)));
			}

			double a = fabs(r_values.at(i + (j * (ncols))) - r_values.at((i + 1) + j * (ncols)));



			if ((c < b) && (c < a) && (j != 0)) 
			{
				j = j - 1;
			}

			else if ((b <= c) && (b < a) && (j != (nrows - 1))) 
			{
				j = j + 1;
			}

			else if ((a <= b) && (a <= c)) 
			{
				j = j;
			}

			r2_values.at(i + (j * (ncols))) = 244;
			g2_values.at(i + (j * (ncols))) = 175;
			b2_values.at(i + (j * (ncols))) = 0;
		}
	}


void printPPMFile(ofstream& file, int nrows, int ncols,
	vector<double> r_values, vector<double> g_values, vector<double>b_values) 
{
	// TODO check for output error
	file << "P3\n";
	file << ncols << " " << nrows << "\n" << "255\n";
	for (int i = 0; i < r_values.size(); i++) {
		file << r_values.at(i) << " " << g_values.at(i) << " " << b_values.at(i) << " ";
	}
	file.close();
}

int main()
{
	int nrows, ncols;
	cout << "Enter number of rows and columns:\n";
	nrows = readInt(cin);
	if (nrows == -1) 
	{
		cerr << "invalid input. bye!\n";
		return -1;
	}
	ncols = readInt(cin);
	if (ncols == -1) 
	{
		cerr << "invalid input. bye!\n";
		return -1;
	}

	cout << "Enter name of input file:\n";
	string data_file_name;
	cin >> data_file_name;
	if (!cin.good()) 
	{
		cerr << "Failed to read name of input file. Giving up, bye!\n";
	}

	ifstream in_file{ data_file_name };
	if (!in_file.is_open()) 
	{
		cerr << "Failed to open input file " << data_file_name << ". Giving up, bye!\n";
	}

	vector<vector<int> > map(nrows, vector<int>(ncols));
	for (int i = 0; i < nrows; i++) 
	{
		for (int j = 0; j < ncols; j++) 
		{
			map.at(i).at(j) = readInt(in_file);
			if (map.at(i).at(j) == -1) 
			{ // error in the input
				cout << "Error during input from file (i is " << i << ", j is " << j << "). Giving up, bye!\n";
				//printMapNumbers(cout, map);
				return -1;
			}
		}
	}

	// determine min and max
	int max = 0;
	int min = map.at(0).at(0);
	for (int i = 0; i < nrows; i++) 
	{
		for (int j = 0; j < ncols; j++) 
		{
			if (map.at(i).at(j) > max) 
			{
				max = map.at(i).at(j);
			}
			if (map.at(i).at(j) < min) 
			{
				min = map.at(i).at(j);
			}
		}
	}
	cout << "min is " << min << "; max is " << max << endl;

	vector<double> r_values(nrows*ncols);
	vector<double> g_values(nrows*ncols);
	vector<double> b_values(nrows*ncols);
	



	ofstream rgb_file{ data_file_name + ".ppm" };
	// compute gray scale
	for (int i = 0; i < nrows; i++) 
	{
		for (int j = 0; j < ncols; j++) 
		{
			int elev = map.at(i).at(j);
			double delev = double(elev - min) / double(max - min) * 255;
			//cout << "delev " << delev << " ";
			int scaled_value = int(delev);
			int k = i*ncols + j;
			r_values[k] = g_values[k] = b_values[k] = scaled_value;
		}
	}

	// identical vectors so that maroon path coloring won't affect greedy path calculation
	vector<double> r2_values = r_values;
	vector<double> g2_values = g_values;
	vector <double> b2_values = b_values;

	// keeps track of elevation changes
	vector<double> temp;
 
	greedyPaths(ncols, nrows, r_values, g_values, b_values, r2_values, g2_values, b2_values, temp);

	// Here is the function that calculates the gold path
	// a possible improvement here is to combine them into one overloaded function
	greedyGold(temp, ncols, nrows, r_values, g_values, b_values, r2_values, g2_values, b2_values);

	system("pause");

	printPPMFile(rgb_file, nrows, ncols, r2_values, g2_values, b2_values);

	//printMapNumbers(cout, map);

}