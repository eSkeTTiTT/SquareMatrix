#include "SquareMatrix.h"

string add_string(ifstream& fin, char symbol)
{
	string result;
	while (symbol != '}') {
		result.push_back(symbol);
		symbol = fin.get();
	}

	return result;
}

int search_index_function(vector<string> array, string operation)
{
	for (int i = 0; i < SIZE_OF_FUNCTION_ARRAY; i++)
	{
		if (array[i] == operation)
			return i;
	}

	return -1;
}

int main(int argc, char* argv[])
{
	string operation;
	string matrix_1, matrix_2;
	string number;

	vector<string> functions_array{"+", "-", "*", "==", "!=", "det", "rev", "T", "tr", "e"}; // индексы от 0 к 1
	
	ofstream cerrFile("CERR_LOG.txt");
	cerr.rdbuf(cerrFile.rdbuf());

	ofstream texFile("TeX.tex");
	texFile << "\\begin{document}\n";

	if (argc != 2) {
		cerr << "Invalid value of arguments!\n";
		return -1;
	}
	else {
		ifstream fin;
		char symbol;
		fin.open(argv[1]);
		if (fin) {
			symbol = fin.get();
			while (!fin.eof())
			{
				if (symbol >= '0' && symbol <= '9') {
					number.push_back(symbol);
					while ((symbol = fin.get()) != ' ' && !fin.eof() && symbol != '\n')
						number.push_back(symbol);
				}
				else if (matrix_1.empty() && symbol == '{') {
					symbol = fin.get(); // пропускаем скобку
					matrix_1 = add_string(fin, symbol);
					symbol = fin.get(); // пропускаем }
				}
				else if (matrix_2.empty() && symbol == '{') {
					symbol = fin.get();
					matrix_2 = add_string(fin, symbol);
					symbol = fin.get(); // пропускаем }
				}
				else if (symbol == ' ') {
					symbol = fin.get();
					continue;
				}
				else if (symbol == '\n' && !matrix_1.empty()) {
					try
					{
						Square_Matrix m1(matrix_1);
						Square_Matrix m2(matrix_2);
						Square_Matrix m3; // матрица результата
						int index; // для нахождения индекса операции
						double number_value = 0;
						double determenant = 0;
						bool check_equality = true;
						if (!number.empty())
							number_value = atof(number.c_str());	

						index = search_index_function(functions_array, operation);

						// выполнение операций
						switch (index)
						{
						case 0:
							m3 = m1 + m2;
							break;
						case 1:
							m3 = m1 - m2;
							break;
						case 2:
							if (matrix_2.empty())
								m3 = number_value * m1;
							else
								m3 = m1 * m2;
							break;
						case 3:
							check_equality = (m1 == m2);
							break;
						case 4:
							check_equality = (m1 != m2);
							break;
						case 5:
							determenant = det(m1);
							break;
						case 6:
							m3 = rev(m1);
							break;
						case 7:
							m3 = T(m1);
							break;
						case 8:
							determenant = tr(m1);
							break;
						case 9:
							m3 = e(m1);
							break;
						}

						// заполнение тех файла
						if (index >= 0 && index <= 2)
						{
							if (matrix_2.empty())
								texFile << number << " " << operation << " " << m1.convert() << " = " << m3.convert() << ".\n";
							else
								texFile << m1.convert() << " " << operation << " " << m2.convert() << " = " << m3.convert() << ".\n";
						}
						else if (index == 8 || index == 5)
							texFile << operation << " " << m1.convert() << " = " << determenant << ".\n";
						else if (index == 3 || index == 4)
							texFile << m1.convert() << " " << operation << " " << m2.convert() << " = " << check_equality << ".\n";
						else
							texFile << operation << " " << m1.convert() << " = " << m3.convert() << ".\n";

					}
					catch (MyException& ex)
					{
						cerr << ex.what() << "Code: " << ex.code() << endl;
					}

					
					operation.clear();
					matrix_1.clear();
					matrix_2.clear();
					number.clear();
					symbol = fin.get();
					texFile << "\\newline\n";
				}
				else { // осталась операция
					if (symbol == '\n') // когда строка состоит только из \n (matrix_1 пустая)
						symbol = fin.get();
					else {
						while (symbol != ' ') {
							operation.push_back(symbol);
							symbol = fin.get();
						}
					}
				}
			}
		}
		else {
			cerr << "Output file open error: " << argv[1] << endl;
			return -2;
		}
	}


	texFile << "\\end{document}";
	cerrFile.close();
	texFile.close();
	return 0;
}