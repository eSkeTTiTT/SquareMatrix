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

double CountDet(double** ptr, int size) // ������� ������������
{
	double det;
	int minor_j, k; // k - ������� -1
	double** minor;  // ����� ��� ����� ������ �� �������� �������

	if (size == 1)
		return ptr[0][0];
	else if (size == 2)
		return ptr[0][0] * ptr[1][1] - ptr[0][1] * ptr[1][0];
	else
	{
		minor = new double* [size - 1];  // ������ ������ �� ������� ������
		det = 0;
		k = 1;  // ���� ������
		for (int i = 0; i < size; i++)  // ���������� �� ������� �������
		{
			minor_j = 0;
			for (int j = 0; j < size; j++)
				if (i != j) // ��������� i ������
					minor[minor_j++] = ptr[j] + 1;  // ����� + 1 ��������� ������ �������

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

bool check_matrix(double* arr, int size) // ��������� ������� �� �������� ������ eps
{
	double eps = 1e-1;
	for (int i = 0; i < size * size; i++)
	{
		if (arr[i] > eps)
			return false;
	}

	return true;
}