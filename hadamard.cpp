#include "hadamard.h"
#include <QVector>

void fill(QVector<int>& res, unsigned int n, unsigned int& size)
{
	if(n == 1)
	{
		res[0] = 1;
		res[1] = 1;
		res[size] = 1;
		res[size +1] = -1;
		return;
	}
	fill(res, n-1, size);

	// fill other parts of matrix
	int filledSize = 1<<(n-1);
	for(int i = 0; i < filledSize; ++i)
	{
		int line = i * size;
		for(int j = filledSize; j < (int)size; ++j)
		{
			res[line + j] = res[line + j - filledSize];
		}
	}

	for(int i = filledSize; i < (int)size; ++i)
	{
		int line = i * size;
		for(int j = 0; j < (int)size; ++j)
		{
			int k = 1;
			if( j >= filledSize)
				k = -1;
			res[line + j] = k * res[(i-filledSize)*size + j];
		}
	}
}

std::unique_ptr<QVector<int> > hadamard(unsigned int n)
{
	unsigned int size = 1<<n; // 2^n
	std::unique_ptr<QVector<int> > mat ( new QVector<int>(size * size) );
	fill((*mat), n, size);
	return mat;
}

void mult(QVector<int>& first, QVector<int>& sec, QVector<int>& res, int size)
{
	for(int i = 0; i < size; ++i)
	{
		int line = i * size;
		for(int j = 0; j < size; ++j)
		{
			res[line + j] = 0;
			for(int k = 0; k < size; ++k)
			{
				int column = k * size;
				res[line + j] += first[line + k] * sec[column + j];
			}
		}
	}
}

std::unique_ptr<QVector<int> > traverse(QVector<int>& mat, int size)
{
	std::unique_ptr<QVector<int> > res ( new QVector<int>(mat.size()) );
	for(int i = 0; i < size; ++i)
	{
		int line = i* size;
		for(int j = 0; j < size; ++j)
		{
			int col = j * size;
			(*res)[line + j] = mat[col + i];
		}
	}
	return res;
}
