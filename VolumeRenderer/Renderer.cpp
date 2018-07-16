#include "stdafx.h"
#include "Renderer.h"


Renderer::Renderer()
{
	m_pVolume = nullptr;
}


Renderer::Renderer(unsigned char* volume, int width, int height, int depth)
{
	m_pVolume = shared_ptr<Volume>(new Volume(volume, width, height, depth));

}


Renderer::~Renderer()
{
}


bool Renderer::RenderSliceXDirection(unsigned char* image, 
	const int img_width, const int img_height, const int depth)
{
	int vol_width = m_pVolume->GetHeight();
	int vol_depth = m_pVolume->GetDepth();

	for (int i = 0; i < vol_width; i++)
	{
		for (int j = 0; j < vol_depth; j++)
		{
			image[img_width *j + i] = m_pVolume->GetVoxel(depth, i, j);
		}
	}

	return true;

}


bool Renderer::RenderSliceYDirection(unsigned char* image,
	const int img_width, const int img_height, const int depth)
{
	int vol_height = m_pVolume->GetHeight();
	int vol_depth = m_pVolume->GetDepth();

	for (int i = 0; i < vol_height; i++)
	{
		for (int j = 0; j < vol_depth; j++)
		{
			image[j*img_width + i] = m_pVolume->GetVoxel(i, depth, j);
		}
	}

	return true;
}


bool Renderer::RenderSliceZDirection(unsigned char* image,
	const int img_width, const int img_height, const int depth)
{
	int vol_width = m_pVolume->GetWidth();
	int vol_height = m_pVolume->GetHeight();

	for (int i = 0; i < vol_width; i++)
	{
		for (int j = 0; j < vol_height; j++)
		{
			image[j*img_width + i] = m_pVolume->GetVoxel(i, j, depth);
		}
	}

	return true;

}


bool Renderer::RenderMIPXDirection(unsigned char* image,
	const int img_width, const int img_height)
{
	int vol_width = m_pVolume->GetWidth();
	int vol_height = m_pVolume->GetHeight();
	int vol_depth = m_pVolume->GetDepth();

	for (int i = 0; i < vol_height; i++)
	{
		for (int j = 0; j < vol_depth; j++)
		{
			unsigned char max_value = 0;
			for (int k = 0; k < vol_width; k++)
				max_value = __max(max_value, m_pVolume->GetVoxel(k, i, j));
			
			image[img_height * j + i] = max_value;
		}
	}

	return true;
}

bool Renderer::RenderMIPYDirection(unsigned char* image,
	const int img_width, const int img_height)
{
	int vol_width = m_pVolume->GetWidth();
	int vol_height = m_pVolume->GetHeight();
	int vol_depth = m_pVolume->GetDepth();

	for (int i = 0; i < vol_width; i++)
	{
		for (int j = 0; j < vol_depth; j++)
		{
			unsigned char max_value = 0;
			for (int k = 0; k < vol_height; k++)
				max_value = __max(max_value, m_pVolume->GetVoxel(i, k, j));

			image[img_height * j + i] = max_value;
		}
	}

	return true;
}

bool Renderer::RenderMIPZDirection(unsigned char* image,
	const int img_width, const int img_height)
{
	int vol_width = m_pVolume->GetWidth();
	int vol_height = m_pVolume->GetHeight();
	int vol_depth = m_pVolume->GetDepth();

	for (int i = 0; i < vol_width; i++)
	{
		for (int j = 0; j < vol_height; j++)
		{
			unsigned char max_value = 0;
			for (int k = 0; k < vol_depth; k++)
				max_value = __max(max_value, m_pVolume->GetVoxel(i, j, k));

			image[img_height * j + i] = max_value;
		}
	}

	return true;
}

bool Renderer::RenderMIPAnyDirection(unsigned char* image,
	const int img_width, const int img_height)
{
	int vol_width = m_pVolume->GetWidth();
	int vol_height = m_pVolume->GetHeight();
	int vol_depth = m_pVolume->GetDepth();

	///����ǥ, ������, ���� ���� ����
	float3 eye_coord = float3(100.f, 200.f, 300.f);
	float3 up_vector = float3(0.f, 0.f, 1.f);
	float3 center_coord = { vol_width / 2.f, vol_height / 2.f, vol_depth / 2.f };

	/// �交�� ���
	float3 view_vector = center_coord - eye_coord;
	view_vector.normalize();

	/// x���� ���
	float3 x_vector = cross(view_vector, up_vector);
	x_vector.normalize();

	///y���� ���
	float3 y_vector = cross(view_vector, x_vector);
	y_vector.normalize();

	for (int i = 0; i < img_width; i++)
	{
		for (int j = 0; j < img_height; j++)
		{
			///���� �ȼ��� 3���� ��ǥ ���
			float3 cur_coord = eye_coord + 
				x_vector * (i-img_width/2) + y_vector * (j-img_height/2);

			unsigned char max_value = 0;
			for (float k = 0.f; k < 1000.f; k += 1.f)
			{
				///���� �ȼ��� ���� ��ġ�� ���� �ٿ���� �ȿ� ���Դٸ� 
				if (cur_coord.x >= 0.f && cur_coord.x < vol_width &&
					cur_coord.y >= 0.f && cur_coord.y < vol_height &&
					cur_coord.z >= 0.f && cur_coord.z < vol_depth)
				{
					///�ش� ��ġ������ ���� ������ ������
					unsigned char voxel = 
						m_pVolume->GetVoxel(cur_coord.x, cur_coord.y, cur_coord.z);

					///�ƽ��� ��
					max_value = __max(max_value, voxel);
				}

				///�交�� �������� ��ĭ ���� (view_vector�� normalize�� �����Ƿ�, 1 ��ŭ ������)
				cur_coord += view_vector;
			}

			///������ �� �̹����� ����
			image[img_height * j + i] = max_value;
		}
	}

	return true;
}