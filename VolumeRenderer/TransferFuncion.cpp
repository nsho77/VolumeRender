#include "stdafx.h"
#include "TransferFuncion.h"


TransferFuncion::TransferFuncion()
{
	m_PalleteC[0] = nullptr;
	m_PalleteC[1] = nullptr;
	m_PalleteC[2] = nullptr;
	m_PalleteA = nullptr;
}


TransferFuncion::TransferFuncion(int start_color[3], int end_color[3],
	int start_alpha, int end_alpha)
{
	SetColorPallete(start_color, end_color);
	SetAlphaPallete(start_alpha, end_alpha);
}

TransferFuncion::~TransferFuncion()
{
}

void TransferFuncion::SetColorPallete(int start_color[3], int end_color[3])
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
	}
}


void TransferFuncion::SetAlphaPallete(int start_alpha, int end_alpha)
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
}


float TransferFuncion::GetPalleteCValue(int color, int intensity)
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


float TransferFuncion::GetPalleteAValue(int intensity)
{
	if (intensity >= MAX_INTENSITY)
		return -1.f;

	return  m_PalleteA.get()[intensity];

}