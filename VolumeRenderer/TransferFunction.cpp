#include "stdafx.h"
#include "TransferFunction.h"


TransferFunction::TransferFunction()
{
	m_PalleteC[0] = nullptr;
	m_PalleteC[1] = nullptr;
	m_PalleteC[2] = nullptr;
	m_PalleteA = nullptr;

	m_PalleteC2D[0] = nullptr;
	m_PalleteC2D[1] = nullptr;
	m_PalleteC2D[2] = nullptr;
	m_PalleteA2D = nullptr;
}


TransferFunction::TransferFunction(int start_color[3], int end_color[3],
	int start_alpha, int end_alpha)
{
	SetColorPallete(start_color, end_color);
	SetAlphaPallete(start_alpha, end_alpha);
}

TransferFunction::~TransferFunction()
{
}

void TransferFunction::SetColorPallete(int start_color[3], int end_color[3])
{
	for (int i = 0; i < 3; i++)
	{
		if (!m_PalleteC[i])
		{
			m_PalleteC[i] = shared_ptr<float>(new float[MAX_INTENSITY]);
			memset(m_PalleteC[i].get(), 0, sizeof(float)*MAX_INTENSITY);
		}

		for (int j = start_color[i]; j < end_color[i]; j++)
		{
			float start = static_cast<float>(start_color[i]);
			float end = static_cast<float>(end_color[i]);
			m_PalleteC[i].get()[j] = (static_cast<float>(j) - start) / (end - start) * 255.f;
		}

		for (int k = end_color[i]; k < MAX_INTENSITY; k++)
		{
			m_PalleteC[i].get()[k] = 255.f;
		}
		if (!m_PalleteC2D[i])
		{
			m_PalleteC2D[i] = shared_ptr<float>(new float[MAX_INTENSITY*MAX_INTENSITY]);
			memset(m_PalleteC2D[i].get(), 0, sizeof(float)*MAX_INTENSITY*MAX_INTENSITY);
		}

		for (int j = 0; j < MAX_INTENSITY; j++)
		{
			for (int k = 0; k < MAX_INTENSITY; k++)
			{
				float diff = fabs(static_cast<float>(j - k));
				if (diff == 0.f) continue;

				float sum = 0.f;
				if (j < k)
				{
					for (int q = j; q <= k; q++)
						sum += m_PalleteC[i].get()[q];
				}
				else
				{
					for (int q = k; q <= j; q++)
						sum += m_PalleteC[i].get()[q];
				}
				float avg = sum / diff;

				m_PalleteC2D[i].get()[MAX_INTENSITY*k + j] = avg;

			}
		}
	}
}


void TransferFunction::SetAlphaPallete(int start_alpha, int end_alpha)
{
	if (!m_PalleteA)
	{
		m_PalleteA = shared_ptr<float>(new float[MAX_INTENSITY]);
		memset(m_PalleteA.get(), 0, sizeof(float)*MAX_INTENSITY);
	}

	for (int i = start_alpha; i < end_alpha; i++)
	{
		float start = static_cast<float>(start_alpha);
		float end = static_cast<float>(end_alpha);

		m_PalleteA.get()[i] = (static_cast<float>(i) - start) / (end - start);
	}

	for (int j = end_alpha; j < 255; j++)
	{
		m_PalleteA.get()[j] = 1.f;
	}

	if (!m_PalleteA2D)
	{
		m_PalleteA2D = shared_ptr<float>(new float[MAX_INTENSITY*MAX_INTENSITY]);
		memset(m_PalleteA2D.get(), 0, sizeof(float)*MAX_INTENSITY*MAX_INTENSITY);
	}

	for (int j = 0; j < MAX_INTENSITY; j++)
	{
		for (int i = 0; i < MAX_INTENSITY; i++)
		{
			float diff = fabs(static_cast<float>(j - i));
			if (diff == 0)
				continue;

			float sum = 0.f;
			if (i < j)
			{
				for (int k = i; k <= j; k++)
					sum += m_PalleteA.get()[k];
			}
			else
			{
				for (int k = j; k <= i; k++)
					sum += m_PalleteA.get()[k];
			}
			float avg = sum / diff;
			
			m_PalleteA2D.get()[MAX_INTENSITY*j + i] = avg;
		}
	}
}


float TransferFunction::GetPalleteCValue(int color, int intensity)
{
	if (intensity >= MAX_INTENSITY)
		return -1.f;


	switch (color)
	{
	case 0:
		return m_PalleteC[0].get()[intensity];
		break;
	case 1:
		return m_PalleteC[1].get()[intensity];
		break;
	case 2:
		return m_PalleteC[2].get()[intensity];
		break;
	default: /// color 값이 잘못 들어옴... -1 return
		return  -1.f;
		break;
	}

}


float TransferFunction::GetPalleteCValue(int color, float intensity)
{
	if (intensity+1.f >= MAX_INTENSITY)
		return -1.f;


	switch (color)
	{
	case 0:
		return GetPalleteSingleCValue(0, intensity);
		break;
	case 1:
		return GetPalleteSingleCValue(1, intensity);
		break;
	case 2:
		return GetPalleteSingleCValue(2, intensity);
		break;
	default: /// color 값이 잘못 들어옴... -1 return
		return  -1.f;
		break;
	}
}



float TransferFunction::GetPalleteSingleCValue(int color, float intensity)
{
	int minus = intensity;
	int plus = intensity + 1;
	float weight = intensity - static_cast<float>(minus);

	float minus_value = m_PalleteC[color].get()[minus];
	float plus_value = m_PalleteC[color].get()[plus];

	return (1.f - weight) * minus_value + weight * plus_value;
}


float TransferFunction::GetPalleteAValue(int intensity)
{
	if (intensity >= MAX_INTENSITY)
		return -1.f;

	return  m_PalleteA.get()[intensity];

}



float TransferFunction::GetPalleteAValue(float intensity)
{
	if (intensity + 1.f >= MAX_INTENSITY)
		return -1.f;

	int minus = intensity;
	int plus = intensity + 1;
	float weight = intensity - static_cast<float>(minus);

	float minus_value = m_PalleteA.get()[minus];
	float plus_value = m_PalleteA.get()[plus];

	return (1.f - weight) * minus_value + weight * plus_value;
}