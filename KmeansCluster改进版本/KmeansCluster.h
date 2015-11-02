#ifndef KMEANS_CLUSTER_H
#define KMEANS_CLUSTER_H

class KmeansCluster
{
private:
	struct Pattern
	{
		int PatternIndex;
		double *Data; // 内嵌数据
		int Center;	// 数据所属簇号
		double *Distance; // 数据和簇中心的距离
	};
	struct Cluster
	{
		int ClusterIndex;
		double *Center;
		int PatternNum;
		int* PatternIndex;
		double Allowance;
	};

	bool Completed;
	int DataNum; //聚类样本数目
	int Dimension; //样本维数
	int ClusterNum; //分类数

	int K;
	int MaxK;

	Pattern *DataMember;
	
	Cluster *ClusterMember;
public:
	KmeansCluster(int dataNum, int dimension, int K,int MaxK, double *DataSet,double*ClusterSet);	
	
	~KmeansCluster(void);

	void AddPattern(Cluster *pData, int Idx, double *pValue, int Dimension);

	void ReducePattern(Cluster *pData, int Idx, double *pValue, int Dimension);

	void SetData(Pattern *pData, double *pValue, int DataNum, int Dimension);
	
	void CopyValue(double *pValue1, double *pValue2, int Dimension);

	int FindCenter(int nDataIndex);
	
	double SquareDistance( int nDataIndex,int nClusterIndex);

	bool GetCompleted();

	void ChangeCluster(int nDataIndex,int nClusterIndex);

	// 开始聚类
	void StartClustering();
	bool SetupClustering();

	void PrintCenter();

	void FindCloseCluster(int &tempX,int &tempY,Cluster* tempCluster,Pattern* tempPattern,int& tempNum);
	double ClusterDistance(int nIndexX,int nIndexY,Cluster *tempCluster);
	void Unite(int tempX,int tempY,Cluster* tempCluster,Pattern* tempPattern,int& tempNum);

	bool Depart(int nIndexData,int nIndexCluster,Pattern* pDataMember, Cluster* pClusterMember,int nDimension);
	void AddCenter(int nIndexData,Pattern* pDataMember, Cluster* pClusterMember,int nDimension);

	void GetAllowance();
};

#endif