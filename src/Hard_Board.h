#pragma once
#include "Board.h"

class Hard_Board :
	public Board
{
public:
	Hard_Board();
	~Hard_Board();
	void Ask_DimX(void);
	void Ask_DimY(void);
	void Create_matrix(void);
	void Fill_matrix(void);
	void Show_list(void);
	void Save(ofstream& os);
};

