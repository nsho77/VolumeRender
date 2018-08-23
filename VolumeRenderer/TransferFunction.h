#pragma once
#include <memory>
using namespace std;

#define MAX_INTENSITY 256

class TransferFunction
{
public:
	TransferFunction();
	TransferFunction(int start_color[3], int end_color[3],
		int start_alpha, int end_alpha);
	~TransferFunction();
private:
	shared_ptr<float> m_PalleteC[3];
	shared_ptr<float> m_PalleteA;
	shared_ptr<float> m_PalleteC2D[3];
	shared_ptr<float> m_PalleteA2D;

private:
	float GetPalleteSingleCValue(int color, float intensity);

public:
	void SetColorPallete(int start_color[3], int end_color[3]);
	void SetAlphaPallete(int start_alpha, int end_alpha);
	float GetPalleteCValue(int color, int intensity);
	float GetPalleteCValue(int color, float intensity);
	float GetPalleteAValue(int intensity);
	float GetPalleteAValue(float intensity);

	float GetPalleteA2DValue(float prev_intensity, float intensity);
};

