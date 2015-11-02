#include "KmeansCluster.h"
//#include "KmeansCluster.cpp"
#include "HierarchicalCluster.h"
//#include "HierarchicalCluster.cpp"
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

	HierarchicalCluster* pHCluster=new HierarchicalCluster(200, 2, 10, &DataSet[0][0]);
	pHCluster->StartHierarchicalCluster();
	double* ClusterData=new double[10*2];
	pHCluster->getCenter(ClusterData);
	
	cout<<"start clustering...\n";
	KmeansCluster* pKCluster = new KmeansCluster(200 , 2, 10,15, &DataSet[0][0],ClusterData);
	pKCluster->StartClustering();

	while ( pKCluster->SetupClustering() != true)
		cout<<"cluster..."<<endl;

	cout<<"completed...\n";
	pKCluster->GetAllowance();
	pKCluster->PrintCenter();
	return 0;
}

#endif