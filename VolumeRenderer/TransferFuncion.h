#pragma once
#include <memory>
using namespace std;

#define MAX_INTENSITY 256

class TransferFuncion
{
public:
	TransferFuncion();
	TransferFuncion(int start_color[3], int end_color[3],
		int start_alpha, int end_alpha);
	~TransferFuncion();
private:
	shared_ptr<float> m_PalleteC[3];
	shared_ptr<float> m_PalleteA;

public:
	void SetColorPallete(int start_color[3], int end_color[3]);
	void SetAlphaPallete(int start_alpha, int end_alpha);
	float GetPalleteCValue(int color, int intensity);
	float GetPalleteAValue(int intensity);
};

