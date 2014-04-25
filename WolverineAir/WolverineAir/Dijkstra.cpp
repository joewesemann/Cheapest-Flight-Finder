#include "stdafx.h"
#include "Dijkstra.h"
#include <stdio.h>

Dijkstra::Dijkstra(int s)
{
	int i;
	size = s;
	graphc = new int*[size]; // create dynamic two dimensional array to store the flight cost graphc
	for (i = 0; i < size; ++i)
		graphc[i] = new int[size];
	graphm = new int*[size]; // create dynamic two dimensional array to store the flight mileage graphc
	for (i = 0; i < size; ++i)
		graphm[i] = new int[size];
}

Dijkstra::~Dijkstra()
{
	int i;
	for (i = 0; i < size; ++i)
		delete[] graphc[i];
	delete[] graphc;
	for (i = 0; i < size; ++i)
		delete[] graphm[i];
	delete[] graphm;
}

// Getter method that returns the number of unique cities (i.e. size of the two-demensional array is 2 * size)
int Dijkstra::getsize()
{
	return size;
}
// Initializes the graphc array (two-dimensional array)
void Dijkstra::init()
{
	int i, j;
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (i == j)
			{
				graphc[i][i] = 0;
				graphm[i][i] = 0;
			}
			else
			{
				graphc[i][j] = INFINITY_MARK;
				graphm[i][j] = INFINITY_MARK;
			}
		}
	}
	return;
}

// Insert the cost and mileage information into the graphc and graphm matrices respectively
void Dijkstra::insert(int from, int to, int cost, int mileage)
{
	if (from >= size) return; // error, the index for the "from" city is out of bounds.
	if (to >= size) return; // error, the index for the "to" city is out of bounds.
	graphc[from][to] = cost; 
	graphm[from][to] = mileage; 
}

// Print the cost graph matrix
void Dijkstra::printcg()
{
	int i, j;
	cout << "\n";
	cout << "  Flight cost matrix:\n";
	cout << "\n";
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (graphc[i][j] == INFINITY_MARK)
				cout << "  Inf";
			else
				cout << "  " << setw(3) << graphc[i][j];
		}
		cout << "\n";
	}
}

// Print the mileage graph matrix
void Dijkstra::printmg()
{
	int i, j;
	cout << "\n";
	cout << "  Flight mileage matrix:\n";
	cout << "\n";
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (graphm[i][j] == INFINITY_MARK)
				cout << "  Inf";
			else
				cout << "  " << setw(3) << graphm[i][j];
		}
		cout << "\n";
	}
}

//    Dijkstra's minimum distance algorithm to find least cost for a flight:
//    Starts with the origin city connected to the graph, and this is indicated 
//    by setting connected[x] = TRUE, where 'x' is the index for the origin city.
//    Initialize minc[i] to the one step distance from city 0 to city 'i'.
//    Search among the unconnected cities for the city mv whose minimum
//    cost is smallest, and connect it to the tree.  For each remaining
//    unconnected city 'i', we check to see whether the cost from origin city to mv
//    to 'i' is less than that recorded in minc[i], and if so, we can reduce
//    the cost. After size-1 steps, we have connected all the cities to the origin 
//    city, and computed the correct minimum flight cost.
//    Input: int graphc[size][size], the distance of the direct link between cities 
//    'i' and 'j'. 
//    Output: int cost[] and mileage[], the minimum cost and mileage from origin 
//    city ('fromcity') to each city.
void Dijkstra::leastcost(int fromcity, int cost[], int mileage[], string flightpath[], int **oconnections, int **dconnections)
{
	bool *connected;
	int i, j, mv;
	connected = new bool[size];
	for (i = 0; i < size; i++)
		connected[i] = false;
	connected[fromcity] = true; //  Origin city is connected to the tree
	//  Initialize the minimum flight cost and mileage to the one-step distance.
	for (i = 0; i < size; i++)
	{
		cost[i] = graphc[fromcity][i];
		mileage[i] = graphm[fromcity][i];
	}
	//  Attach one more city on each iteration
	for (j = 0; j < size; j++)
	{
		mv = nearest(cost, connected); //  Find the nearest unconnected city
		if (mv == -1)
			break; // Exit, the search terminated early, graphc may not be connected
		connected[mv] = true; //  Mark this city as connected
		//  Determined the minimum flight cost to city MV, see if that reduces the minimum cost to other cities
		mincost(mv, connected, cost, mileage, flightpath, oconnections, dconnections);
	}
	delete[] connected;
}

//    Finds the nearest unconnected city.
//    Input: int minc[], the currently computed minimum flight cost from origin city to each city.
//    Input: bool connected[], is true for each connected city, whose minimum flight cost to origin city has been determined.
//    int originc, the cost from origin city to the nearest unconnected destination city.
//    Return: int ncity, the index of the nearest unconnected city.
int Dijkstra::nearest(int minc[], bool connected[])
{
	int originc = INFINITY_MARK;
	int ncity = -1;
	for (int i = 0; i < size; i++)
	{
		if (!connected[i] && (minc[i] < originc))
		{
			originc = minc[i];
			ncity = i;
		}
	}
	return ncity;
}

//    Updates the minimum cost vector.
//    We've just determined the minimum flight cost to city 'mc'.  For each city 'i' which is 
//	  not connected yet, check whether the route from the origin city to mc to 'i' is shorter than the 
//    currently known minimum cost.
//    Input: int mc, the city whose minimum cost to the origin city has just been determined.
//    Input: bool connected[], is true for each connected city, whose minimum cost to the origin city has been determined.
//    Input/output: int minc[], minm[], the currently computed minimum costs amd mileage from the origin city to each city.
void Dijkstra::mincost(int mc, bool connected[], int minc[], int minm[], string flightpath[], int **oconnections, int **dconnections)
{
	char buffer[100] = "";
	int i, j;
	for (i = 0; i < size; i++)
	{
		if (!connected[i])
		{
			if (graphc[mc][i] < INFINITY_MARK) //  eliminate the possiblity that the link is infinite
			{
				if (minc[mc] + graphc[mc][i] < minc[i])
				{
					// mc (origin city) to i (destination city)
					minc[i] = minc[mc] + graphc[mc][i]; // add up the flight costs for the minimum distance's total cost
					minm[i] = minm[mc] + graphm[mc][i]; // add up the flight mileage for the minimum distance's total mileage
					sprintf_s(buffer, "%s Origin:%d -> Dest:%d", flightpath[mc].c_str(), mc, i); // keep track of the flight paths: mc (origin city) to i (destination city)
					flightpath[i] = buffer;
					j = 0;
					while ((oconnections[mc][j] != -1) && (j < size)) // make j point to the memory of mc to prepend connecting flight origins
						oconnections[i][j] = oconnections[mc][j++];
					j = 0;
					while ((dconnections[mc][j] != -1) && (j < size)) // make j point to the memory of mc to prepend connecting flight destinations
						dconnections[i][j] = dconnections[mc][j++];
					j = 0;
					while ((oconnections[i][j] != -1) && (j < size)) j++; // make j point to the next memory location in oconnections for the next connecting flight origin
					oconnections[i][j] = mc; // set the origin flight city to the oconnections matrix
					j = 0;
					while ((dconnections[i][j] != -1) && (j < size)) j++; // make j point to the next memory location in dconnections for the next connecting flight destination
					dconnections[i][j] = i; // set the destination flight city to the dconnections matrix
				}
			}
		}
	}
}
