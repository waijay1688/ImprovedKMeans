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

	int DataNum; //聚类样本数目
	int Dimension; //样本维数
	int ClusterNum; //分类数
	
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