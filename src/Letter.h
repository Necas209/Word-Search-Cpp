#pragma once
#include "Point.h"

#define LOWERCASE 0
#define UPPERCASE 1
#define NOT_USED 0

class Letter
{
private:
	char letter;
	int ascii;
	int letter_case;  // 0 - minuscula, 1 - maiuscula
	int state;
	Point point;
public:
	Letter();
	virtual ~Letter();
	void Set_point(int x, int y);
	void Set_L(char letter, int ascii, int letter_case);
	void Set_state(const int state) { this->state = state; };
	int Get_state(void) const { return state; };
	void Rand_letter(void);
	void Read(ifstream& is);
	void Save(ofstream& os);
	void ShowLetter(void);
	void operator=(Letter l);
	void operator=(char c);
	bool operator==(char c);
	friend ostream& operator<<(ostream& os, Letter l);
};

