#pragma once

# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <ctime>

# define INFINITY_MARK 2147483647
using namespace std;

class Dijkstra
{
private:
	int size; // number of cities, i.e. size of the graphs (both the cost and mileage graphs)
	int **graphc; // dynamic two dimensional array to store the flight cost graph
	int **graphm; // dynamic two dimensional array to store the flight mileage graph
	int nearest(int mind[], bool connected[]);
	void mincost(int mv, bool connected[], int minc[], int minm[], string flightpath[], int **oconnections, int **dconnections);

public:
	Dijkstra(int s);
	~Dijkstra();
	void init();
	void insert(int from, int to, int cost, int mileage);
	int getsize();
	void printcg();
	void printmg();
	void leastcost(int fromcity, int cost[], int mileage[], string flightpath[], int **oconnections, int **dconnections);
};

