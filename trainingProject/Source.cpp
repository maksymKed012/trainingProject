#include <iostream>
#include <time.h>

class UnionFind
{
	int* aArray;
	int nLength;

public:

	UnionFind(int n) : nLength(n)
	{
		aArray = new int[n];
		for (int i = 0; i < n; ++i)
		{
			aArray[i] = i;
		}
	}

	void connect(int x, int y)
	{
		int xid = aArray[x];
		int yid = aArray[y];

		for (int i = 0; i < nLength; ++i)
		{
			if (aArray[i] == xid)
				aArray[i] = yid;
		}
	}

	bool connected(int x, int y)
	{
		return aArray[x] == aArray[y];
	}
};

class QUnionFind
{
	int * aID;
	int nLength;

public:
	QUnionFind(int n) : nLength(n)
	{
		aID = new int[n];

		for (int i = 0; i < nLength; ++i)
		{
			aID[i] = i;
		}
	}

	int root(int i)
	{
		while (i != aID[i])
			i = aID[i];
		return i;
	}

	bool find(int p, int q)
	{
		return root(p) == root(q);
	}

	void connect(int p, int q)
	{
		int i = root(p);
		int j = root(q);
		aID[i] = j;
	}
};

class QUnionFind_Plus
{
	int* aID;
	int* aSZ;
	int nLength;

public:

	QUnionFind_Plus(int n) : nLength(n)
	{
		aID = new int[nLength];
		aSZ = new int[nLength];
		for (int i = 0; i < nLength; ++i)
		{
			aID[i] = i;
			aSZ[i] = 1;
		}
	}

	int root(int i)
	{
		while (i != aID[i])
			i = aID[i];
		return i;
	}

	bool find(int p, int q)
	{
		return root(p) == root(q);
	}

	void connect(int p, int q)
	{
		int i = root(p);
		int j = root(q);

		if (i == j) return;
		if (aSZ[i] < aSZ[j])
		{
			aID[i] = j;
			aSZ[j] += aSZ[i];
		}
		else
		{
			aID[j] = i;
			aSZ[j] += aSZ[i];
		}
	}

};

/*QUnionFind_Plus* qufp = new QUnionFind_Plus(10);

qufp->connect(3, 4);
qufp->connect(2, 9);

bool bRes = qufp->find(0, 3);
bRes = qufp->find(3, 4);*/
#include <memory>
#include "Percolation.h"

int main()
{
	std::unique_ptr<Percolation> p(new Percolation(5));

	int nTries = 0;

	while (!p->percolates())
	{
		int nSiteX = std::rand() % 4;
		int nSiteY = std::rand() % 4;
		p->open(nSiteX, nSiteY);
		nTries++;
	}


	return 0;
}