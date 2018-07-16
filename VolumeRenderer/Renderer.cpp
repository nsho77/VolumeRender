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

	///눈좌표, 업벡터, 볼륨 센터 설정
	float3 eye_coord = float3(100.f, 200.f, 300.f);
	float3 up_vector = float3(0.f, 0.f, 1.f);
	float3 center_coord = { vol_width / 2.f, vol_height / 2.f, vol_depth / 2.f };

	/// 뷰벡터 계산
	float3 view_vector = center_coord - eye_coord;
	view_vector.normalize();

	/// x벡터 계산
	float3 x_vector = cross(view_vector, up_vector);
	x_vector.normalize();

	///y벡터 계산
	float3 y_vector = cross(view_vector, x_vector);
	y_vector.normalize();

	for (int i = 0; i < img_width; i++)
	{
		for (int j = 0; j < img_height; j++)
		{
			///시작 픽셀의 3차원 좌표 계산
			float3 cur_coord = eye_coord + 
				x_vector * (i-img_width/2) + y_vector * (j-img_height/2);

			unsigned char max_value = 0;
			for (float k = 0.f; k < 1000.f; k += 1.f)
			{
				///진행 픽셀의 현재 위치가 볼륨 바운더리 안에 들어왔다면 
				if (cur_coord.x >= 0.f && cur_coord.x < vol_width &&
					cur_coord.y >= 0.f && cur_coord.y < vol_height &&
					cur_coord.z >= 0.f && cur_coord.z < vol_depth)
				{
					///해당 위치에서의 볼륨 복셀을 가져옴
					unsigned char voxel = 
						m_pVolume->GetVoxel(cur_coord.x, cur_coord.y, cur_coord.z);

					///맥스값 비교
					max_value = __max(max_value, voxel);
				}

				///뷰벡터 방향으로 한칸 전진 (view_vector는 normalize를 했으므로, 1 만큼 전진함)
				cur_coord += view_vector;
			}

			///마지막 값 이미지에 대입
			image[img_height * j + i] = max_value;
		}
	}

	return true;
}