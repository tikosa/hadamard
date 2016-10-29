#ifndef HADAMARD_H
#define HADAMARD_H

#include <memory>
#include <QVector>

std::unique_ptr<QVector<int> > hadamard(unsigned int n);

void mult(QVector<int>& first, QVector<int>& sec, QVector<int>& res, int size);

std::unique_ptr<QVector<int> > traverse(QVector<int>& mat, int size);
#endif // HADAMARD_H
