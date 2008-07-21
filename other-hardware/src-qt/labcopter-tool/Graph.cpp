
#include "Graph.h"

Graph::Graph(){	
	//timestamps = new QList<int>;
	//values = new QList<int>;
}

void Graph::append(int timestamp, int value){
	timestamps.append(timestamp);
	values.append(value);
}
