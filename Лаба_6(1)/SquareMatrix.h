#include "MyException.h"

//Prototypes
class Square_Matrix;
int count_elements(string matrix);

struct Matrix
{
	double* matrix;
	int size;
};

class TeX_convertible
{
public:
	virtual string convert() const = 0;
};

class Square_Matrix : TeX_convertible
{
private:
	struct Matrix* matrix = 0;

	void set_memory(int BUFFSIZE)
	{
		matrix = new struct Matrix;
		matrix->matrix = new double[BUFFSIZE * BUFFSIZE];
		matrix->size = BUFFSIZE;
	}

	void copy_matrix(const struct Matrix& _matrix)
	{
		for (int i = 0; i < _matrix.size * _matrix.size; i++)
			this->matrix->matrix[i] = _matrix.matrix[i];
	}
public:
	friend Square_Matrix operator*(double x, const Square_Matrix& matrix);
	friend ostream& operator<<(ostream& out, const Square_Matrix& obj);
	friend istream& operator>>(istream& in, const Square_Matrix& obj);
	friend double det(Square_Matrix& obj);
	friend Square_Matrix T(Square_Matrix& obj);
	friend Square_Matrix rev(Square_Matrix& obj);
	friend double tr(Square_Matrix& obj);
	friend Square_Matrix e(Square_Matrix& obj);

	Square_Matrix() // конструктор по умолчанию
	{
		set_memory(1); // выделяем память
		matrix->matrix[0] = 1;
	}

	Square_Matrix(int size) // создать единичную матрицу
	{
		set_memory(size);
		int check = 0;
		if (size <= 0)
			throw MyException("Invalid size of matrix: <= 0; ", 464);
		for (int i = 0; i < size * size; i++)
		{
			if (i == check)
			{
				this->matrix->matrix[i] = 1;
				check += size + 1;
			}
			else this->matrix->matrix[i] = 0;
		}
	}

	Square_Matrix(string matrix_name)
	{
		int kol = 0;
		if (!matrix_name.empty())
		{
			kol = count_elements(matrix_name);
			if (sqrt(double(kol)) / double(int(sqrt(double(kol)))) == 1) // если корень целое число
			{
				set_memory(int(sqrt(double(kol))));
				string::const_iterator iter = matrix_name.begin();
				string str;
				int amount = 0;
				while (iter != matrix_name.end())
				{
					if (*iter != ' ')
					{
						while (iter != matrix_name.end() && *iter != ' ') // получаем строку-число
						{
							str.push_back(*iter);
							iter++;
						}
						matrix->matrix[amount++] = atof(str.c_str()); // преобразуем в число
						str.clear(); // очищаем строку
						continue;
					}
					iter++; // пропускаем пробелы
				}
			}
			else { throw MyException(matrix_name, 228); }

		}
		else { set_memory(1); }
	}

	Square_Matrix(const Square_Matrix& obj)
	{
		set_memory(obj.matrix->size);
		copy_matrix(*obj.matrix);
	}

	Square_Matrix& operator=(const Square_Matrix& obj)
	{
		delete[] this->matrix->matrix;
		delete[] this->matrix;

		set_memory(obj.matrix->size);
		copy_matrix(*obj.matrix);

		return *this;
	}

	Square_Matrix operator+(const Square_Matrix& obj)
	{
		return Square_Matrix(*this) += obj;
	}

	Square_Matrix& operator+=(const Square_Matrix& obj)
	{
		if (this->matrix->size != obj.matrix->size)
			throw MyException(this->matrix->matrix, this->matrix->size, obj.matrix->matrix, obj.matrix->size, 322);
		else
		{
			for (int i = 0; i < obj.matrix->size * obj.matrix->size; i++)
				this->matrix->matrix[i] += obj.matrix->matrix[i];
		}

		return *this;
	}

	Square_Matrix operator-(const Square_Matrix& obj)
	{
		return Square_Matrix(*this) -= obj;
	}

	Square_Matrix& operator-=(const Square_Matrix& obj)
	{
		if (this->matrix->size != obj.matrix->size)
			throw MyException(this->matrix->matrix, this->matrix->size, obj.matrix->matrix, obj.matrix->size, 322);
		else
		{
			for (int i = 0; i < obj.matrix->size * obj.matrix->size; i++)
				this->matrix->matrix[i] -= obj.matrix->matrix[i];
		}

		return *this;
	}

	Square_Matrix operator*(const Square_Matrix& obj)
	{
		return Square_Matrix(*this) *= obj;
	}

	Square_Matrix& operator*=(const Square_Matrix& obj)
	{
		if (this->matrix->size != obj.matrix->size)
			throw MyException(this->matrix->matrix, this->matrix->size, obj.matrix->matrix, obj.matrix->size, 322);
		else
		{
			Square_Matrix A(*this);
			double sum = 0, eps = 1e-6;
			for (int i = 0; i < obj.matrix->size * obj.matrix->size; i++) // идем по строке в первой матрице
			{
				for (int j = 0; j < obj.matrix->size; j++) // идем по столбцам во второй матрице
				{
					for (int count = 0; count < obj.matrix->size; count++) // считаем сумму произведение строки на столбец
					{
						sum += A.matrix->matrix[i] * obj.matrix->matrix[j];
						i++; j += obj.matrix->size;
					}

					j -= obj.matrix->size * obj.matrix->size; // возвращаемся к номеру столбца
					i -= obj.matrix->size; // возвращаемся к началу строки

					if (abs(sum) > eps)
						this->matrix->matrix[i + j] = sum;
					else
						this->matrix->matrix[i + j] = 0;

					sum = 0;
				}
				i += obj.matrix->size - 1;
			}
		}

		return *this;
	}

	Square_Matrix operator*(const double& obj) //матрица на число
	{
		return Square_Matrix(*this) *= obj;
	}

	Square_Matrix operator*=(const double& obj)
	{
		for (int i = 0; i < this->matrix->size * this->matrix->size; i++)
			this->matrix->matrix[i] *= obj;

		return *this;
	}

	bool operator==(const Square_Matrix& obj)
	{
		if (this->matrix->size != obj.matrix->size)
			throw MyException(this->matrix->matrix, this->matrix->size, obj.matrix->matrix, obj.matrix->size, 322);
		else
		{
			double eps = 1e-6;
			for (int i = 0; i < obj.matrix->size * obj.matrix->size; i++)
			{
				if (abs(this->matrix->matrix[i] - obj.matrix->matrix[i]) > eps)
					return false;
			}
		}

		return true;
	}

	bool operator!=(const Square_Matrix& obj)
	{
		if (this->matrix->size != obj.matrix->size)
			throw MyException(this->matrix->matrix, this->matrix->size, obj.matrix->matrix, obj.matrix->size, 322);
		else
		{
			double eps = 1e-6;
			for (int i = 0; i < obj.matrix->size * obj.matrix->size; i++)
			{
				if (abs(this->matrix->matrix[i] - obj.matrix->matrix[i]) > eps)
					return true;
			}
		}

		return false;
	}

	double* operator[](int index)
	{
		if (index >= this->matrix->size || index < 0)
			throw MyException("Going beyond borders; ", 1337);

		double* a = new double[this->matrix->size];
		for (int i = 0; i < this->matrix->size; i++)
			a[i] = this->matrix->matrix[index * this->matrix->size + i];

		return a;
	}

	int get_size()
	{
		return this->matrix->size;
	}

	string convert() const override
	{
		string message;
		message.reserve(this->matrix->size * this->matrix->size);

		message += "\\begin{pmatrix}\n";

		for (int i = 0; i < this->matrix->size * this->matrix->size; i++)
		{
			message += to_string(this->matrix->matrix[i]) + " ";

			if ((i + 1) % this->matrix->size == 0)
			{
				if (i + 1 != this->matrix->size * this->matrix->size)
					message += "\\\\\n";
			}
			else
				message += "& ";
		}

		message += "\n\\end{pmatrix}\n";

		return message;
	}

	~Square_Matrix()
	{
		//cout << this << endl;
		delete[] this->matrix->matrix;
		delete[] this->matrix;
	}
};

// Friend Functions
Square_Matrix operator*(double x, const Square_Matrix& matrix)
{
	Square_Matrix new_matrix(matrix);
	for (int i = 0; i < matrix.matrix->size * matrix.matrix->size; i++)
		new_matrix.matrix->matrix[i] *= x;

	return new_matrix;
}

ostream& operator<<(ostream& out, const Square_Matrix& obj)
{
	out << "Matrix:" << endl;
	for (int i = 0; i < obj.matrix->size * obj.matrix->size; i++)
	{
		out << obj.matrix->matrix[i] << " ";

		if ((i + 1) % obj.matrix->size == 0)
			out << endl;
	}

	return out;
}

istream& operator>>(istream& in, Square_Matrix& obj)
{
	string str;
	getline(in, str);
	Square_Matrix new_obj(str);
	obj = new_obj;

	return in;
}

int count_elements(string matrix) // функция проверки квадратности матрицы
{
	int kol = 0;
	string::const_iterator iter = matrix.begin();
	while (iter != matrix.end())
	{
		if (*iter != ' ')
		{
			while (iter != matrix.end() && *iter != ' ')
				iter++;

			kol++;
			continue;
		}
		iter++;
	}

	return kol;
}

double det(Square_Matrix& obj)
{
	int size = obj.matrix->size;
	double** ptr = new double* [size];
	double res;

	convert_array(ptr, obj.matrix->matrix, size);
	res = CountDet(ptr, size);

	// очитска памяти
	for (int i = 0; i < size; i++) delete[] ptr[i];
	delete[] ptr;

	return res;
}

Square_Matrix T(Square_Matrix& obj)
{
	Square_Matrix new_obj(obj);
	for (int i = 0; i < obj.matrix->size; i++)
	{
		for (int j = 0; j < obj.matrix->size; j++)
			new_obj.matrix->matrix[i * obj.matrix->size + j] = obj[j][i];
	}

	return new_obj;
}

Square_Matrix rev(Square_Matrix& obj)
{
	Square_Matrix new_obj(obj);
	int size = obj.matrix->size;
	double** ptr = new double* [size - 1];
	double determenant;
	double coeff;
	int index1 = 0, index2 = 0;

	determenant = det(obj);
	if (determenant == 0)
		throw MyException(obj.matrix->matrix, size, 1488);
	
	coeff = 1 / determenant;

	for (int i = 0; i < size - 1; i++)
		ptr[i] = new double[size - 1];

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			for (int k = 0; k < size; k++)
			{
				for (int l = 0; l < size; l++)
				{
					if (i != k && j != l)
					{
						ptr[index1][index2++] = obj[k][l];
						if (index2 == size - 1)
						{
							index1++;
							index2 = 0;
						}
					}
				}
			}

			new_obj.matrix->matrix[i * size + j] = pow(double(-1), double(i) + double(j)) * CountDet(ptr, size - 1);
			index1 = 0;
		}
	}

	// очитска памяти
	for (int i = 0; i < size - 1; i++) delete[] ptr[i];
	delete[] ptr;

	return coeff * T(new_obj);
}

double tr(Square_Matrix& obj)
{
	double res = 0;
	for (int i = 0; i < obj.matrix->size; i++)
		res += obj[i][i];

	return res;
}

Square_Matrix e(Square_Matrix& obj)
{
	Square_Matrix new_obj(obj.matrix->size); // матрица на проверку (слагаемое)
	Square_Matrix new_obj_2(obj); // obj в степени
	Square_Matrix pred_new_obj(obj.matrix->size); // сохраняем предыдущее вычисление
	int i = 1;

	while (1)
	{
		pred_new_obj = new_obj;
		new_obj = (1 / fact(i++)) * new_obj_2;
		if (check_matrix(new_obj.matrix->matrix, new_obj.matrix->size))
			break;

		new_obj += pred_new_obj;
		new_obj_2 *= obj;
	}

	return pred_new_obj;
}