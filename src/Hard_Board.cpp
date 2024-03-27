#include "Hard_Board.h"

Hard_Board::Hard_Board()
{
}

Hard_Board::~Hard_Board()
{
}

void Hard_Board::Ask_DimX(void)
{
	int dim_x;
	cout << endl << " Insira a dimensão X da matriz: ";
	cin >> dim_x;
	if (!cin.good() or dim_x < 10)
	{
		PreventLoop();
		Ask_DimX();
	}
	else
		DimX = dim_x;
}

void Hard_Board::Ask_DimY(void)
{
	int dim_y;
	cout << endl << " Insira a dimensão Y da matriz: ";
	cin >> dim_y;
	if (!cin.good() or dim_y < 10)
	{
		PreventLoop();
		Ask_DimY();
	}
	else
		DimY = dim_y;
}

void Hard_Board::Create_matrix(void)
{
	cout << endl << "\tDimensões da sopa de letras mínimas: 10 x 10" << endl;
	Board::Create_matrix();
}

void Hard_Board::Fill_matrix(void)
{
	int l;
	srand(static_cast<unsigned>(time(nullptr)));
	const auto start = chrono::high_resolution_clock::now();
	chrono::duration<double> elapsed;
	for (int i = 0; i < num_words; i++)
	{
		l = words[i].length();
		if (l <= DimX and l <= DimY)
		{
			do {
				words[i].RandOrientation();
				words[i].RandPoint(DimX, DimY);
				auto end = chrono::high_resolution_clock::now();
				elapsed = end - start;
			} while (!Check_Crossing(i) and elapsed.count() < 1);
			if (Check_Crossing(i))
			{
				words[i].Set_state(NOT_FOUND);
				Insert_Word(i);
			}
		}
	}
	for (int i = 0; i < DimY; i++)
	{
		for (int j = 0; j < DimX; j++)
		{
			if (matrix[i][j] == ' ')
				matrix[i][j].Rand_letter();
		}
	}
}

void Hard_Board::Show_list(void)
{
	Board::Show_list();
}

void Hard_Board::Save(ofstream& os)
{
	os << "H: ";
	Board::Save(os);
}
