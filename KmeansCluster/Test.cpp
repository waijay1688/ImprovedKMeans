#include "KmeansCluster.h"
//#include "KmeansCluster.cpp"
#ifndef TEST_CPP
#define TEST_CPP

#include <iostream>
using namespace std;

int main()
{
	int i=0;
	double DataSet[200][2] = {0};
	
	for(i=0; i<200; i++)
	{
		DataSet[i][0] = (double)(rand()%500);
		DataSet[i][1] = (double)(rand()%500);
	}

	KmeansCluster* m_pKCluster = new KmeansCluster(200 , 2, 10, &DataSet[0][0]);

	cout<<"start clustering...\n";
	m_pKCluster->StartClustering();

	while ( m_pKCluster->SetupClustering() != true);

	cout<<"completed...\n";
	m_pKCluster->PrintCenter();
	return 0;
}

#endif