#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cmath>
#define SIZE_OF_FUNCTION_ARRAY 10

using namespace std;

double CountDet(double** ptr, int size) // считает определитель
{
	double det;
	int minor_j, k; // k - степень -1
	double** minor;  // минор как набор ссылок на исходную матрицу

	if (size == 1)
		return ptr[0][0];
	else if (size == 2)
		return ptr[0][0] * ptr[1][1] - ptr[0][1] * ptr[1][0];
	else
	{
		minor = new double* [size - 1];  // массив ссылок на столбцы минора
		det = 0;
		k = 1;  // знак минора
		for (int i = 0; i < size; i++)  // разложение по первому столбцу
		{
			minor_j = 0;
			for (int j = 0; j < size; j++)
				if (i != j) // исключить i строку
					minor[minor_j++] = ptr[j] + 1;  // здесь + 1 исключает первый столбец

			det += k * ptr[i][0] * CountDet(minor, size - 1);
			k = -k;
		}

		delete[] minor;
		return det;
	}
}

void convert_array(double** ptr, double* arr, int size)
{
	int index = 0;
	for (int i = 0; i < size; i++)
		ptr[i] = new double[size];

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
			ptr[i][j] = arr[index++];
	}
}

double fact(int N)
{
	if (N < 0)
		return 0; 
	if (N == 0) 
		return 1; 
	else 
		return N * fact(N - 1);
}

bool check_matrix(double* arr, int size) // проверяет матрицу на элементы меньше eps
{
	double eps = 1e-1;
	for (int i = 0; i < size * size; i++)
	{
		if (arr[i] > eps)
			return false;
	}

	return true;
}