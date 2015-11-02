#ifndef HIERARCHICAL_CLUSTER_CPP
#define HIERARCHICAL_CLUSTER_CPP

#include "HierarchicalCluster.h"
#include <iostream>
#include <math.h>
// ¹¹ÔìÆ÷
HierarchicalCluster::HierarchicalCluster(int dataNum, int dimension, int clusterNum, double *DataSet)
{
	DataNum =dataNum;
	Dimension = dimension;
	ClusterNum = clusterNum;
	HCluster=new Cluster[DataNum];
	for(int i=0;i<DataNum;i++)
	{
		HCluster[i].PatternNum=1;
		HCluster[i].Center=new double[Dimension];
		for(int j=0;j<Dimension;j++)
			HCluster[i].Center[j]=DataSet[i*Dimension+j];
	}
}

// Îö¹¹Æ÷
HierarchicalCluster::~HierarchicalCluster(void)
{
	int i=0;

	for(i=0; i<DataNum; i++)
		delete[] HCluster[i].Center;
	delete[] HCluster;
}

void HierarchicalCluster::StartHierarchicalCluster()
{
	using namespace std;
	int HClusterNum=DataNum;
	while(HClusterNum>ClusterNum)
	{
		int tempX,tempY;
		FindCloseCluster(tempX,tempY,HCluster,HClusterNum);
		Unite(tempX,tempY,HCluster,HClusterNum);
	}
	for(int i=0;i<HClusterNum;i++)
	{
		cout<<"center ..."<<endl;
		for(int k=0;k<Dimension;k++)
		{
			cout<<HCluster[i].Center[k]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;
	
}



void HierarchicalCluster:: FindCloseCluster(int &tempX,int &tempY,Cluster *tempCluster,int &tempNum)
{

	double MinDistance=100000000;
	double TotalDistance=0;
	double tempDistance;
	for(int i=0;i<tempNum;i++)
		for(int j=i+1;j<tempNum;j++)
		{
			tempDistance=ClusterDistance(i,j,tempCluster)*tempCluster[i].PatternNum*tempCluster[j].PatternNum;
			if(tempDistance<MinDistance)
			{
				MinDistance=tempDistance;
				tempX=i;
				tempY=j;
			}
		}
	
}

double HierarchicalCluster::ClusterDistance(int nIndexX,int nIndexY,Cluster *tempCluster)
{
	int i;
	double *pValue1=tempCluster[nIndexX].Center;
	double *pValue2=tempCluster[nIndexY].Center;
	double Distance = 0;
	for(i=0; i<Dimension; i++)
		Distance = Distance + (pValue2[i]-pValue1[i])*(pValue2[i]-pValue1[i]);
	return Distance;
}


void HierarchicalCluster::Unite(int tempX,int tempY,Cluster* tempCluster,int& tempNum)
{
	int nPatternX=tempCluster[tempX].PatternNum;
	int nPatternY=tempCluster[tempY].PatternNum;
	if(tempX!=tempNum-1&&tempY!=tempNum-1)
	{
		for(int i=0;i<Dimension;i++)
		{
			tempCluster[tempX].Center[i]=(tempCluster[tempX].Center[i]*nPatternX+
				tempCluster[tempY].Center[i]*nPatternY)/(nPatternX+nPatternY);
			tempCluster[tempY].Center[i]=tempCluster[tempNum-1].Center[i];
		}	
		tempCluster[tempX].PatternNum=nPatternX+nPatternY;
		
		int nTempY=tempCluster[tempY].PatternNum=tempCluster[tempNum-1].PatternNum;
		
		tempNum--;
		return;
	}
	if(tempX==tempNum-1)
	{
		for(int i=0;i<Dimension;i++)
		{
			tempCluster[tempY].Center[i]=(tempCluster[tempX].Center[i]*nPatternX+
				tempCluster[tempY].Center[i]*nPatternY)/(nPatternX+nPatternY);
		}
		tempCluster[tempY].PatternNum=nPatternX+nPatternY;
		
		tempNum--;
		return;
	}
	if(tempY==tempNum-1)
	{
		for(int i=0;i<Dimension;i++)
		{
			tempCluster[tempX].Center[i]=(tempCluster[tempX].Center[i]*nPatternX+
				tempCluster[tempY].Center[i]*nPatternY)/(nPatternX+nPatternY);
		}
		tempCluster[tempX].PatternNum=nPatternX+nPatternY;
		
		tempNum--;
		return;
	}
}

void HierarchicalCluster::getCenter(double *pValue)
{
	for(int i=0;i<ClusterNum;i++)
		for(int j=0;j<Dimension;j++)
			pValue[i*Dimension+j]=HCluster[i].Center[j];
}

#endif