// WolverineAir.cpp 
// CS 2420-601
// Assignment 7 - Graph
// Joe Wesemann

#include "stdafx.h"
#include "Dijkstra.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;


struct flight
{
	string startAirport;
	string endAirport;
	int mileage;
	int price;
};

bool sortByStart(const flight &lhs, const flight &rhs)
{
	return lhs.startAirport < rhs.startAirport;
}

int getcity(string *c, string city, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		if (c[i] == city)
			return i;
	}
	return 0; // city was not found, this is an error
}

int getflight(flight *f, string start, string end, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		if ((f[i].startAirport == start) && (f[i].endAirport == end))
			return i;
	}
	return -1; // flight was not found, this is an error
}

void printlist(int size, int **list, string message)
{
	int i, j;
	cout << "\n";
	cout << "  " << message.c_str() << "\n";
	cout << "\n";
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
			cout << "  " << setw(3) << list[i][j];
		cout << "\n";
	}
}

void printlist(int size, int *list, string message)
{
	cout << "\n";
	cout << "  " << message.c_str() << "\n";
	cout << "\n";
	for (int i = 0; i < size; i++)
		cout << "  " << setw(2) << i << "  " << setw(2) << list[i] << "\n";
	cout << "\n";
}

void printlist(int size, string *list, string message)
{
	cout << "\n";
	cout << "  " << message.c_str() << "\n";
	cout << "\n";
	for (int i = 0; i < size; i++)
		cout << "  " << setw(2) << i << setw(2) << list[i].c_str() << "\n";
	cout << "\n";
}

int _tmain(int argc, _TCHAR* argv[])
{
	string fileName;
	flight f;
	string line;
	int *cost;
	int *mileage;
	int i, j, k, size;
	string *flightpath;
	int **oconnections, **dconnections;

	cout << "Joe Wesemann\n";
	cout << "CS 2420 - Program 7 - Graph\n\n";
	cout << "************************** WELCOME TO WOLVERINE AIR  **************************\n\nPlease enter the flight information file: ";
	cin >> fileName;
	ifstream file(fileName);
	//catch error if file cannot be found
	if (!file.is_open())
	{
		cout << "\nCannot find file in the current folder!\n";
		system("PAUSE");
		return EXIT_FAILURE;
	}

	//build the flight vector
	vector <flight> flights;
	while (getline(file, line))
	{
		istringstream(line) >> f.startAirport >> f.endAirport >> f.mileage >> f.price;
		flights.push_back(f);
	}
	
	//sorts the flight list into alphebetical order
	sort(flights.begin(), flights.end(), sortByStart);
	
	//cities vector
	vector <string> cities;
	int numOfCities = 0;

	//loop through flights list and create cities list
	size = flights.size();
	for (i = 0; i < size; i++)
	{
		if (find(cities.begin(), cities.end(), flights[i].startAirport) == cities.end())
		{
			cities.push_back(flights[i].startAirport);
			numOfCities++;
		}
		
		if (find(cities.begin(), cities.end(), flights[i].endAirport) == cities.end())
		{
			cities.push_back(flights[i].endAirport);
			numOfCities++;
		}
	}

	//insert each flight into a two dimensional matrix
	Dijkstra da(cities.size());
	da.init();

	//loop for each flight
	size = flights.size();
	for (i = 0; i < size; i++)
	{
		da.insert(getcity(cities.data(), flights[i].startAirport, cities.size()), getcity(cities.data(), flights[i].endAirport, cities.size()), flights[i].price, flights[i].mileage);
		//cout << "startAirport: " << flights[i].startAirport << " " << getcity(cities.data(), flights[i].startAirport, cities.size()) << endl;
		//cout << "endAirport: " << flights[i].endAirport << " " << getcity(cities.data(), flights[i].endAirport, cities.size()) << endl;
	} 

	//functions that print the matrix's if you need to debug
	//da.printcg();
	//da.printmg();

	//print list of all direct flights
	cout << "\nDirect Flights\n";
	cout << "Source" << "\t" << "Dest" << "\t" << "Cost" << "\t" << "Mileage" << endl;
	cout << "------" << "\t" << "----" << "\t" << "----" << "\t" << "-------" << endl;
	for (i = 0; i < size; ++i)
		cout << flights[i].startAirport << "\t" << flights[i].endAirport << "\t" << "$" << flights[i].price << "\t" << flights[i].mileage << endl;

	//begin search for cheapest flights 
	string userInput;
	while (userInput != "QUIT")
	{
		cout << "\nEnter the source airport abbreviation to see the 'Best Price Report'\nor 'quit' to quit the program: ";
		cin >> userInput;
		transform(userInput.begin(), userInput.end(), userInput.begin(), toupper);
		if (userInput == "QUIT")
		{
			cout << "\nThanks for using Wolverine Air!\n";
			system("PAUSE");
			return EXIT_FAILURE;
		}

		//check to see if the source airport exsists 
		if (find(cities.begin(), cities.end(), userInput) == cities.end())
		{
			cout << "\nThe Source Airport '" << userInput << "' has no flights or does not exsist!\n";
			system("PAUSE");
		}
		else
		{
			size = da.getsize();
			cost = new int[size];
			mileage = new int[size];
			flightpath = new string[size];
			oconnections = new int*[size]; // create dynamic two dimensional array to store the origin connecting flight information
			dconnections = new int*[size]; // create dynamic two dimensional array to store the destination connecting flight information
			for (i = 0; i < size; i++)
			{
				oconnections[i] = new int[size];
				for (j = 0; j < size; j++)
					oconnections[i][j] = -1;
			}
			for (i = 0; i < size; i++)
			{
				dconnections[i] = new int[size];
				for (j = 0; j < size; j++)
					dconnections[i][j] = -1;
			}
			da.leastcost(getcity(cities.data(), userInput, cities.size()), cost, mileage, flightpath, oconnections, dconnections);
			//printlist(size, cost, "Minimum flight cost from city entered:");
			//printlist(size, mileage, "Minimum flight mileage from city entered:");
			//printlist(size, flightpath, "Flight paths from city entered:");
			//printlist(size, oconnections, "Origin connecting flights:");
			//printlist(size, dconnections, "Destination connecting flights:");

			//print out the best price report from origin city (userInput) to all cities (cities[])
			cout << "\nBest Price Report for flights from: " << userInput << endl;
			cout << "      " << "\t" << "    " << "\t" << "    " << "\t" << "       " << "\t\t" << "Connecting Flight Information" << endl;
			cout << "Source" << "\t" << "Dest" << "\t" << "Cost" << "\t" << "Mileage" << "\t\t" << "Source" << "\t" << "Dest" << "\t" << "Cost" << "\t" << "Mileage" << endl;
			cout << "------" << "\t" << "----" << "\t" << "----" << "\t" << "-------" << "\t\t" << "------" << "\t" << "----" << "\t" << "----" << "\t" << "-------" << endl;
			for (i = 0; i < size; ++i)
			{
				if (cost[i] != INFINITY_MARK)
				{
					if (getcity(cities.data(), userInput, cities.size()) != i) // do not print the flight where the origin city (userInput) is the same as the destination city (i)
					{
						cout << userInput << "\t" << cities[i] << "\t" << "$" << cost[i] << "\t" << mileage[i] << "\t\t";
						if (oconnections[i][0] == -1)
							cout << userInput << " -> " << cities[i] << "\t" << "$" << cost[i] << "\t" << mileage[i] << endl;
						j = 0;
						while (oconnections[i][j] != -1)
						{
							if (j == 0)
							{
								k = getflight(flights.data(), userInput, cities[oconnections[i][j]], flights.size());
								cout << flights[k].startAirport << " -> " << flights[k].endAirport << "\t" << "$" << flights[k].price << "\t" << flights[k].mileage << endl;
							}
							k = getflight(flights.data(), cities[oconnections[i][j]], cities[dconnections[i][j]], flights.size());
							if (k != -1)
								cout << "\t\t\t\t\t" << flights[k].startAirport << " -> " << flights[k].endAirport << "\t" << "$" << flights[k].price << "\t" << flights[k].mileage << endl;
							j++;
						}
						cout << "-------------------------------------------------------------------\n";
					}
				}
			}
			for (i = 0; i < size; ++i)
				delete[] dconnections[i];
			for (i = 0; i < size; ++i)
				delete[] oconnections[i];
			delete[] dconnections;
			delete[] oconnections;
			delete[] flightpath;
			delete[] mileage;
			delete[] cost;

			system("PAUSE");
		}
	}

	system("PAUSE");
	return 0;
}

