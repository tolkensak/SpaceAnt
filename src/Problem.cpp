#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

int Grass[51][3],N,M;
std::fstream fin,fout;

void Exchange(int m, int n)
{
	int k;
	for (int j=0;j<3;j++)
	{
		k=Grass[m][j];
		Grass[m][j]=Grass[n][j];
		Grass[n][j]=k;
	}
}

void FindFirst()
{
	int minY=Grass[1][2],k=1;
	for (int i=2;i<=N;i++)
		if (Grass[i][2]<minY)
		{
			minY=Grass[i][2];
			k=i;
		}
	Grass[0][1]=0;
	Grass[0][2]=minY;
	Exchange(1,k);
}

void FindNext(int n)
{
	double cosFi,cosFi0;
	double la,lb,lc,lb1,lc1;
	int k=n+1;
	la=pow(Grass[n][1]-Grass[n-1][1],2)+pow(Grass[n][2]-Grass[n-1][2],2);
	lb=pow(Grass[n][1]-Grass[n+1][1],2)+pow(Grass[n][2]-Grass[n+1][2],2);
	lc=pow(Grass[n+1][1]-Grass[n-1][1],2)+pow(Grass[n+1][2]-Grass[n-1][2],2);
	cosFi0=(lc-lb-la)/(2*sqrt(la*lb));
	for (int j=n+2;j<=N;j++)
	{
		lb1=pow(Grass[n][1]-Grass[j][1],2)+pow(Grass[n][2]-Grass[j][2],2);
		lc1=pow(Grass[j][1]-Grass[n-1][1],2)+pow(Grass[j][2]-Grass[n-1][2],2);
		cosFi=(lc1-lb1-la)/(2*sqrt(la*lb1));
		if ( (cosFi>cosFi0) || (cosFi==cosFi0) && (lb1<lb) )
		{
			k=j;
			cosFi0=cosFi;
			lb=lb1;
		}
	}
	Exchange(n+1,k);
}

void Go()
{
	FindFirst();
	for (int i=1;i<N;i++)
		FindNext(i);
	for (i=1;i<=N;i++)
		fout<<Grass[i][0]<<' ';
}


void Will()
{
	fin.open("G.in",std::ios::in);
	if (!fin.is_open())
	{
		std::cout<<"Can`t find file 'G.in' !";
		return;
	}
	fout.open("G.out",std::ios::out);
	if (!fout.is_open())
	{
		std::cout<<"Can`t create file 'G.out' !";
		return;
	}
	fin>>M;
}

void Does()
{
	for (int i=1;i<=M;i++)
	{
		fin>>N;
		fout<<N<<' ';
		for (int j=1;j<=N;j++)
			fin>>Grass[j][0]>>Grass[j][1]>>Grass[j][2];
		Go();
		fout<<std::endl;
	}
}


void Done()
{
	fin.close();
	fout.close();
}


void main()
{
	Will();
	Does();
	Done();
}
