#ifndef HIERARCHICAL_CLUSTER_H
#define HIERARCHICAL_CLUSTER_H

class HierarchicalCluster
{
private:
	struct Cluster
	{
		int ClusterIndex;
		double *Center;
		int PatternNum;
		double Allowance;
	};

	int DataNum; //����������Ŀ
	int Dimension; //����ά��
	int ClusterNum; //������
	
	Cluster *HCluster;
public:
	HierarchicalCluster(int dataNum, int dimension, int clusterNum, double *DataSet);	
	
	~HierarchicalCluster(void);
	void getCenter(double *pValue);
	void StartHierarchicalCluster();

	void FindCloseCluster(int &tempX,int &tempY,Cluster *tempCluster,int &tempNum);
	double ClusterDistance(int nIndexX,int nIndexY,Cluster *tempCluster);
	void Unite(int tempX,int tempY,Cluster* tempCluster,int& tempNum);
};

#endif