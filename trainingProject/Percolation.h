#pragma once
#include <iostream>
#include <time.h>

#define VIRTUAL_SITES_COUNT 2


class Percolation 
{
private:
	int * aID;
	int * aSz;
	bool * aOpen;
	int nLength;

	bool isIndValid(int nInd) { return nInd > 0 && nInd < nLength; }
	void connect(int p, int q)
	{
		int x = root(p);
		int y = root(q);

		if (x == y) return;

		if (aSz[x] < aSz[y])
		{
			aID[x] = y;
			aSz[y] += aSz[x];
		}
		else
		{
			aID[y] = x;
			aSz[x] += aSz[y];
		}
	}

	int root(int p)
	{
		while (p != aID[p])
			p = aID[p];
		return p;
	}

	bool find(int p, int q)
	{
		return root(p) == root(q);
	}

	int getIndex(int nRow, int nCol)
	{
		if (nRow == 0)
			nRow = sqrt(nLength - 2);

		if (nCol == 0)
			nCol = sqrt(nLength - 2);

		return nRow * nCol;
	}

public:
	Percolation(int n);                // create n-by-n grid, with all sites blocked
	void open(int row, int col);    // open site (row, col) if it is not open already
	bool isOpen(int row, int col);  // is site (row, col) open?
	bool isFull(int row, int col);  // is site (row, col) full?
	int numberOfOpenSites();       // number of open sites
	bool percolates();       // does the system percolate?
};

Percolation::Percolation(int n) 
{
	nLength = n*n + VIRTUAL_SITES_COUNT;
	aID = new int[nLength];
	aSz = new int[nLength];
	aOpen = new bool[nLength];
	for (int i = 0; i < nLength; ++i)
	{
		aID[i] = i;
		aSz[i] = 1;
		aOpen[i] = false;
	}

	connect(0, 1);
	connect(0, 2);
	connect(0, 3);
	connect(0, 4);

	connect(nLength - 1, nLength - 2);
	connect(nLength - 1, nLength - 3);
	connect(nLength - 1, nLength - 4);
	connect(nLength - 1, nLength - 5);
}

void Percolation::open(int row, int col)
{
	int nInd = getIndex(row, col);
	bool bValid = isIndValid(nInd);
	if (bValid)
	{
		aOpen[nInd] = true;
		if (isIndValid(nInd + 1) && aOpen[nInd + 1])
			connect(nInd, nInd + 1);
		if (isIndValid(nInd - 1) && aOpen[nInd - 1])
			connect(nInd, nInd - 1);
		if (isIndValid(nInd - sqrt(nLength - 2)) && aOpen[nInd - (int)(sqrt(nLength))])
			connect(nInd, nInd - sqrt(nLength - 2));
		if (isIndValid(nInd + sqrt(nLength - 2)) && aOpen[nInd + (int)(sqrt(nLength))])
			connect(nInd, nInd + sqrt(nLength - 2));
	}
}

bool Percolation::isOpen(int row, int col)
{
	if (isIndValid(row * col))
		return aOpen[row * col];
	else
		return false;
}

bool Percolation::isFull(int row, int col)
{
	return false;
}

int Percolation::numberOfOpenSites()
{
	int nNum = 0;
	for (int i = 0; i < nLength; ++i)
		if (aOpen[i])
			nNum++;
	return nNum;
}

bool Percolation::percolates()
{
	return find(0, nLength - 1);
}
