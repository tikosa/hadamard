#ifndef HADAMARD_H
#define HADAMARD_H

#include <memory>

template<typename C>
void fill(C& res, unsigned int n, unsigned int& size)
{
	if(n == 0)
	{
		res[0] = 1;
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

// @function hadamard
// H_1 = [ 1 ]
//
//       [ H_n-1   H_n-1] 
// H_n = |              |
//       [ H_n-1  -H_n-1]
template<typename C>
std::unique_ptr<C> hadamard(unsigned int n)
{
	unsigned int size = 1<<n; // 2^n
	std::unique_ptr<C> mat ( new C(size * size) );
	fill((*mat), n, size);
	return mat;
}

template<typename C>
void mult(C& first, C& sec, C& res, int size)
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

template<typename C>
std::unique_ptr<C> traverse(C& mat, int size)
{
	std::unique_ptr<C> res ( new C(mat.size()) );
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

#endif // HADAMARD_H
