#ifndef KMEANS_CLUSTER_CPP
#define KMEANS_CLUSTER_CPP

#include "KmeansCluster.h"
#include <iostream>
#include <math.h>
// 构造器
KmeansCluster::KmeansCluster(int dataNum, int dimension, int k,int maxK, double *DataSet,double*ClusterSet)
{
	int i=0;
	Completed=false;
	DataNum =dataNum;
	MaxK=maxK;
	Dimension = dimension;
	ClusterNum = K=k;
	DataMember = new Pattern[DataNum];
	ClusterMember = new Cluster[MaxK];

	for(i=0; i<DataNum; i++)
	{
		DataMember[i].Data = new double[Dimension];	
		DataMember[i].Distance = new double[MaxK];
	}	
	for(i=0; i<MaxK; i++)
	{
		ClusterMember[i].Center = new double[Dimension];
		ClusterMember[i].PatternIndex=new int[DataNum];
	}

	SetData(DataMember, DataSet, DataNum, Dimension);
	//为聚类中心赋初始值
	for(i=0; i<ClusterNum; i++)
	{
		CopyValue(ClusterMember[i].Center, ClusterSet+i*Dimension, Dimension);
		ClusterMember[i].PatternNum = 0;
		ClusterMember[i].ClusterIndex=i;
	}
}
// 析构器
KmeansCluster::~KmeansCluster(void)
{
	int i=0;
	for(i=0; i<DataNum; i++)
	{
		delete[] DataMember[i].Data;	
		delete[] DataMember[i].Distance;
	}
	delete[] DataMember;

	for(i=0; i<ClusterNum; i++)
	{
		delete[] ClusterMember[i].Center;
		delete[] ClusterMember[i].PatternIndex;
	}
	delete[] ClusterMember;

}

void KmeansCluster::AddPattern(Cluster *pData, int Idx, double *pValue, int Dimension)
{
	int i=0;
	double Center;
	for(i=0; i<Dimension; i++)
	{
		Center = pData[Idx].Center[i];
		Center = Center + (pValue[i] - Center) / (pData[Idx].PatternNum);
		pData[Idx].Center[i] = Center;
	}
}


void KmeansCluster::ReducePattern(Cluster *pData, int Idx, double *pValue, int Dimension)
{
	int i;
	double Center;
	for(i=0; i<Dimension; i++)
	{
		Center = pData[Idx].Center[i];
		Center = Center + (Center - pValue[i]) / (pData[Idx].PatternNum);
		pData[Idx].Center[i] = Center;
	}
}

void KmeansCluster::SetData(Pattern *pData, double *pValue, int DataNum, int Dimension)
{
	int i, j;
	for(i=0; i<DataNum; i++)
	{
		for(j=0; j<Dimension; j++)
			pData[i].Data[j] = pValue[i*Dimension+j];
	}
}

void KmeansCluster::CopyValue(double *pValue1, double *pValue2, int Dimension)
{
	int i;
	for(i=0; i<Dimension; i++)		
		pValue1[i] = pValue2[i];
}

int KmeansCluster::FindCenter(int nDataIndex)
{
	int i=0, Ret = 0;
	double* pValue=DataMember[nDataIndex].Distance;
	double Min = pValue[0];

	for(i=0; i<ClusterNum; i++)	
	{
		if(pValue[i]<Min)
		{
			Min = pValue[i];
			Ret = i;
		}
	}
	return Ret;
}

double KmeansCluster::SquareDistance(int nDataIndex,int nClusterIndex)
{
	int i;
	double *pValue1=DataMember[nDataIndex].Data;
	double *pValue2=ClusterMember[nClusterIndex].Center;
	double Distance = 0;
	for(i=0; i<Dimension; i++)
		Distance = Distance + (pValue2[i]-pValue1[i])*(pValue2[i]-pValue1[i]);
	return Distance;
}

// 开始聚类，生成初始簇中心，仅仅只是赋初始值和形成其第一步聚类中心和聚类
void KmeansCluster::StartClustering()
{
	int i=0, j=0;
	
	
	for(i=0; i<DataNum; i++)
	{	
		DataMember[i].PatternIndex=i;
		//计算DataMember到各个Center的距离
		for(j=0; j<K; j++)
		{
			DataMember[i].Distance[j] = SquareDistance(i,j);
		}
		//标属DataMember所属的Center
		int ClusterIdxA = DataMember[i].Center = FindCenter(i);
		//修改所属Center的属性
		int patternNum=ClusterMember[ClusterIdxA].PatternNum;
		ClusterMember[ClusterIdxA].PatternIndex[patternNum]=i;
		ClusterMember[ClusterIdxA].PatternNum += 1;
		AddPattern(ClusterMember, ClusterIdxA, DataMember[i].Data, Dimension);
		Completed=false;
	}
}

bool KmeansCluster::GetCompleted()
{
	return Completed;
}

void KmeansCluster::ChangeCluster(int nDataIndex,int nClusterIndex)
{
	//更改上一步所属的聚类中心属性值
	int ClusterIdxA = DataMember[nDataIndex].Center;
	
	int *pPattern=ClusterMember[ClusterIdxA].PatternIndex;
	int nPattern=ClusterMember[ClusterIdxA].PatternNum;
	ClusterMember[ClusterIdxA].PatternNum =nPattern-1;
	for(int i=0;i<nPattern;i++)
	{
		if(pPattern[i]==nDataIndex)
		{
			//在这曾出现一个大bug 注意nPattern-1和nPattern
			//由于曾用nPattern，而pPattern[nPattern]又未初始化，故再后来交换时使得交换的数据未初始化
			//Cluster的Pattern索引出错直接导致后来访问Pattern的越界错误
			pPattern[i]=pPattern[nPattern-1];
			break;
		}
	}
	ReducePattern(ClusterMember, ClusterIdxA, DataMember[nDataIndex].Data, Dimension);
	
	//更改新的所属聚类中心属性值
	int ClusterIdxB = DataMember[nDataIndex].Center = nClusterIndex;
	pPattern=ClusterMember[ClusterIdxB].PatternIndex;
	nPattern=ClusterMember[ClusterIdxB].PatternNum;
	pPattern[nPattern]=nDataIndex;
	ClusterMember[ClusterIdxB].PatternNum =nPattern+1;
	AddPattern(ClusterMember, ClusterIdxB, DataMember[nDataIndex].Data, Dimension);		
}
// 一个聚类步
bool KmeansCluster::SetupClustering()
{
	Completed=true;
	//一次聚类循环：1.重新归类；2.修改类中心
	for(int i=0; i<DataNum; i++)  
	{
		//计算DataMember到各个Center的聚类的距离
		for(int j=0; j<ClusterNum ;j++)
		{
			DataMember[i].Distance[j] = SquareDistance(i,j);
		}
		//上一步得到的DataMember的聚类中心标识	
		int Center = DataMember[i].Center;
		//所属聚类中心是否变化
		int newCenter;
		if(Center!=(newCenter=FindCenter(i)))
		{
			if(!Depart(i,newCenter,DataMember,ClusterMember,Dimension))
			{
				ChangeCluster(i,newCenter);
				DataMember[i].Center=newCenter;
			}
			else
			if(ClusterNum==MaxK)
			{
				int tempX,tempY;
				FindCloseCluster(tempX,tempY,ClusterMember,DataMember,ClusterNum);
				Unite(tempX,tempY,ClusterMember,DataMember,ClusterNum);
			}			
			else
			if(ClusterNum<MaxK)
				AddCenter(i,DataMember,ClusterMember,Dimension);
			
			Completed=false;
		}					
	} 
	//判断聚类是否完成，ClusteringCompleted=true,聚类停止
	return GetCompleted();
}

void KmeansCluster::PrintCenter()
{
	using namespace std;
	if(!Completed)
	{
		cout<<"clustering has not completed yet."<<endl;
		return;
	}
	for(int i=0;i<ClusterNum;i++)
	{
		cout<<"cluster "<<i<<" : "<<endl;
		cout<<"Allowance "<<ClusterMember[i].Allowance<<" "<<endl;
		for(int k=0;k<Dimension;k++)
			cout<<ClusterMember[i].Center[k]<<" ";
		cout<<endl;
		cout<<endl;
		for(int j=0;j<DataNum;j++)
		{
			if(DataMember[j].Center==i)
			{
				for(k=0;k<Dimension;k++)
					cout<<DataMember[j].Data[k]<<" ";
				cout<<endl;
			}
		}
		cout<<endl;
	}
}

void KmeansCluster:: FindCloseCluster(int &tempX,int &tempY,Cluster *tempCluster,Pattern* tempPattern,int &tempNum)
{
	double MinDistance=100000000;
	double TotalDistance=0;
	double tempDistance;
	for(int i=0;i<tempNum;i++)
		for(int j=i+1;j<tempNum;j++)
		{
			double XYDistance=sqrt(ClusterDistance(i,j,tempCluster));
			tempDistance=XYDistance*tempCluster[i].PatternNum*tempCluster[j].PatternNum;
			if(tempDistance<MinDistance)
			{
				MinDistance=tempDistance;
				tempX=i;
				tempY=j;
			}
		}	
}


double KmeansCluster::ClusterDistance(int nIndexX,int nIndexY,Cluster *tempCluster)
{
	int i;
	double *pValue1=tempCluster[nIndexX].Center;
	double *pValue2=tempCluster[nIndexY].Center;
	double Distance = 0;
	for(i=0; i<Dimension; i++)
		Distance = Distance + (pValue2[i]-pValue1[i])*(pValue2[i]-pValue1[i]);
	return Distance;
}
//xiu gai xulie hao ,yihou bushang
void KmeansCluster::Unite(int tempX,int tempY,Cluster* tempCluster,Pattern* tempPattern,int& tempNum)
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
		for(i=nPatternX;i<nPatternX+nPatternY;i++)
		{
			int nTemp;
			nTemp=tempCluster[tempX].PatternIndex[i]=tempCluster[tempY].PatternIndex[i-nPatternX];
			tempPattern[nTemp].Center=tempX;
		}
		int nTempY=tempCluster[tempY].PatternNum=tempCluster[tempNum-1].PatternNum;
		for(i=0;i<nTempY;i++)
		{
			int nTemp;
			nTemp=tempCluster[tempY].PatternIndex[i]=tempCluster[tempNum-1].PatternIndex[i];
			tempPattern[nTemp].Center=tempY;
		}
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
		for(i=nPatternY;i<nPatternX+nPatternY;i++)
		{
			int nTemp;
			tempCluster[tempY].PatternIndex[i]=nTemp=tempCluster[tempX].PatternIndex[i-nPatternY];
			tempPattern[nTemp].Center=tempY;
		}
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
		for(i=nPatternX;i<nPatternX+nPatternY;i++)
		{
			int nTemp;
			tempCluster[tempX].PatternIndex[i]=nTemp=tempCluster[tempY].PatternIndex[i-nPatternX];
			tempPattern[nTemp].Center=tempX;
		}
		tempNum--;
		return;
	}
}



bool KmeansCluster:: Depart(int nIndexData,int nIndexCluster,Pattern* pDataMember, Cluster* pClusterMember,int nDimension)
{
	int nPattern=pClusterMember[nIndexCluster].PatternNum;
	int *pPattern=pClusterMember[nIndexCluster].PatternIndex;
	double TotalDistence=0;
	for(int i=0;i<nPattern;i++)
	{
		TotalDistence+=SquareDistance(pPattern[i], nIndexCluster);
	}
	double temp=SquareDistance(nIndexData,nIndexCluster)*nPattern/TotalDistence/(nPattern+1);
	if(temp>1.3||temp<0.7)
		return true;
	return false;
}

void KmeansCluster::AddCenter(int nIndexData,Pattern* pDataMember, Cluster* pClusterMember,int nDimension)
{
	CopyValue(pClusterMember[ClusterNum].Center,pDataMember[nIndexData].Data,nDimension);
	pDataMember[nIndexData].Center=ClusterNum;
	pClusterMember[ClusterNum].ClusterIndex=ClusterNum;
	pClusterMember[ClusterNum].PatternNum=1;
	pClusterMember[ClusterNum].PatternIndex[0]=nIndexData;
	this->ClusterNum++;
}

void KmeansCluster::GetAllowance()
{
	for(int i=0;i<ClusterNum;i++)
	{
		ClusterMember[i].Allowance=0;
		for(int j=0;j<ClusterMember[i].PatternNum;j++)
		ClusterMember[i].Allowance+=SquareDistance(ClusterMember[i].PatternIndex[j],i);
		ClusterMember[i].Allowance/=ClusterMember[i].PatternNum;
		ClusterMember[i].Allowance=sqrt(ClusterMember[i].Allowance);
	}
}
#endif