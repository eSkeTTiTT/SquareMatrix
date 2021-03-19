#include "includes.h"

/*
Instruction:

code 228 - not square
code 322 - size does not match
code 1337 - going beyond borders
code 1488 - determenant = 0

*/

class MyException
{
public:
	MyException(const char* msg, int value) // 1337 and default
	{
		this->_dataState = value;
		GetString(msg);
	}

	MyException(double* arr_1, int lenght1, double* arr_2, int lenght2, int value) // 322
	{
		this->_dataState = value;
		this->_message = "Matrices { ";
		for (int i = 0; i < lenght1 * lenght1; i++)
		{
			this->_message += to_string(arr_1[i]);
			this->_message += " ";
		}

		this->_message += "} and { ";

		for (int i = 0; i < lenght2 * lenght2; i++)
		{
			this->_message += to_string(arr_2[i]);
			this->_message += " ";
		}

		this->_message += "} do not match; ";
	}

	MyException(double* arr, int lenght, int value) // 1488
	{
		this->_dataState = value;
		this->_message = "Matrix { ";
		for (int i = 0; i < lenght * lenght; i++)
		{
			this->_message += to_string(arr[i]);
			this->_message += " ";
		}

		this->_message += "}; Determenant = 0; ";
	}

	MyException(string matrix_name, int value) // 228
	{
		this->_dataState = value;
		matrix_name = "{ " + matrix_name + " }: Matrix is not square; ";
		this->_message = matrix_name;
	}

	int code()
	{
		return this->_dataState;
	}

	string what()
	{
		return this->_message;
	}
private:
	int _dataState;
	string _message;

	void GetString(const char* msg) // переводим char* в string
	{
		int i = 0;
		while (msg[i] != '\0')
			this->_message.push_back(msg[i++]);
	}
};