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
	int vol_height = m_pVolume->GetHeight();
	int vol_depth = m_pVolume->GetDepth();

	for (int i = 0; i < vol_height; i++)
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
			
			image[img_width * j + i] = max_value;
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

			image[img_width * j + i] = max_value;
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

			image[img_width * j + i] = max_value;
		}
	}

	return true;
}


void Renderer::GetRayBound(float t[2], float3 start_coord, float3 view_vector)
{
	const float EPS = 0.00001f;
	/// 1 2를 구한다.
	float kx[2] = { -20000, 20000 }, ky[2] = { -20000, 20000 }, kz[2] = { -20000, 20000 };

	if (fabs((float)view_vector.x) > EPS)
	{
		kx[0] = (0.f - start_coord.x) / view_vector.x;
		kx[1] = (m_pVolume->GetWidth() - start_coord.x) / view_vector.x;
		if (kx[0] > kx[1])
		{
			/// in > out
			float temp = kx[0];
			kx[0] = kx[1];
			kx[1] = temp;
		}
	}

	if (fabs((float)view_vector.y) > EPS)
	{
		ky[0] = (0.f - start_coord.y) / view_vector.y;
		ky[1] = (m_pVolume->GetHeight() - start_coord.y) / view_vector.y;
		if (ky[0] > ky[1])
		{
			/// in > out
			float temp = ky[0];
			ky[0] = ky[1];
			ky[1] = temp;
		}
	}

	if (fabs((float)view_vector.z) > EPS)
	{
		kz[0] = (0.f - start_coord.z) / view_vector.z;
		kz[1] = (m_pVolume->GetDepth() - start_coord.z) / view_vector.z;
		if (kz[0] > kz[1])
		{
			/// in > out
			float temp = kz[0];
			kz[0] = kz[1];
			kz[1] = temp;
		}
	}

	t[0] = __max(__max(kx[0], ky[0]), kz[0]);
	t[1] = __min(__min(kx[1], ky[1]), kz[1]);

	t[0] = t[0] + 0.01f;
	t[1] = t[1] - 0.01f;

}

bool Renderer::RenderMIPAnyDirection(unsigned char* image,
	const int img_width, const int img_height, 
	float angle)
{
	int vol_width = m_pVolume->GetWidth();
	int vol_height = m_pVolume->GetHeight();
	int vol_depth = m_pVolume->GetDepth();

	///눈좌표, 업벡터, 볼륨 센터 설정
	float3 eye_coord = float3(255.f / 2.f, 255.f, 224.f/2.f);
	float3 up_vector = float3(0.f, 0.f, -1.f);
	float3 center_coord = { vol_width / 2.f, vol_height / 2.f, vol_depth / 2.f };

	/// 눈좌표 rotate
	angle = angle * 3.141592f / 180.f;
	float cos_ = cosf(angle);
	float sin_ = sinf(angle);
	float x2 = center_coord.x * center_coord.x;
	float y2 = center_coord.y * center_coord.y;
	float z2 = center_coord.z * center_coord.z;
	float oneMinusCos = 1.f - cos_;

	/*float rotate_matrix[3][3] = { 0 };
	rotate_matrix[0][0] = (oneMinusCos * x2) + cos_;
	rotate_matrix[0][1] = (oneMinusCos * center_coord.x * center_coord.y) - (center_coord.z * sin_);
	rotate_matrix[0][2] = (oneMinusCos * center_coord.y * center_coord.z) + (center_coord.y * sin_);

	rotate_matrix[1][0] = (oneMinusCos * center_coord.x * center_coord.y) + (center_coord.z * sin_);
	rotate_matrix[1][1] = (oneMinusCos * y2) + cos_;
	rotate_matrix[1][2] = (oneMinusCos * center_coord.y * center_coord.z) - (center_coord.x * sin_);

	rotate_matrix[2][0] = (oneMinusCos * center_coord.x * center_coord.z) - (center_coord.y * sin_);
	rotate_matrix[2][1] = (oneMinusCos * center_coord.y * center_coord.z) + (center_coord.x * sin_);
	rotate_matrix[2][2] = (oneMinusCos * z2) + cos_;*/


	float rotate_matrix[3][3] = {
		{cos_, (-1)*sin_, 0.f },
		{sin_, cos_ , 0.f },
		{0.f,0.f,1.f}
	};
	
	
	float rotate_eye[3] = { eye_coord.x, eye_coord.y, eye_coord.z };
	float res_arr[3] = { 0.f };
	for (int i = 0; i < 3; i++)
	{
		float res = 0.f;
		for (int j = 0; j < 3; j++)
		{
			res = res + rotate_eye[j] * rotate_matrix[i][j];
		}
		res_arr[i] = res;
	}

	eye_coord.x = res_arr[0]; eye_coord.y = res_arr[1]; eye_coord.z = res_arr[2];


	/// 뷰벡터 계산
	/// 연산자 오버로딩이 필요하다
	float3 view_vector = center_coord - eye_coord;
	//printf("view_vector :%f %f %f\n",view_vector.x, view_vector.y, view_vector.z);
	/// 구조체 안에 함수를 정의한다.
	view_vector.normalize();
	//printf("view_vector :%f %f %f\n", view_vector.x, view_vector.y, view_vector.z);

//
//	/// x벡터 계산
	float3 x_vector = cross(view_vector, up_vector);
	//printf("x_vector :%f %f %f\n", x_vector.x, x_vector.y, x_vector.z);
	x_vector.normalize();
	//printf("x_vector :%f %f %f\n", x_vector.x, x_vector.y, x_vector.z);
//
//	///y벡터 계산
	float3 y_vector = cross(view_vector, x_vector);
	y_vector.normalize();

	for (int i = 0; i < img_width; i++)
	{
		for (int j = 0; j < img_height; j++)
		{
			///시작 픽셀의 3차원 좌표 계산
			float3 cur_coord = eye_coord + 
				x_vector * (i-img_width/2) + y_vector * (j-img_height/2);

			float t[2] = { 0.f, 0.f };
			GetRayBound(t, cur_coord, view_vector);

			unsigned char max_value = 0;
			for (float k = t[0]; k < t[1]; k += 1.f)
			{
				float3 adv_coord = cur_coord + view_vector * k;

				///진행 픽셀의 현재 위치가 볼륨 바운더리 안에 들어왔다면 
				if (adv_coord.x >= 0.f && adv_coord.x < vol_width-1 &&
					adv_coord.y >= 0.f && adv_coord.y < vol_height-1 &&
					adv_coord.z >= 0.f && adv_coord.z < vol_depth-1)
				{
					///해당 위치에서의 볼륨 복셀을 가져옴
					float voxel = 
						m_pVolume->GetVoxel(adv_coord.x, adv_coord.y, adv_coord.z);

					///맥스값 비교
					max_value = __max(max_value, voxel);
				}

				///뷰벡터 방향으로 한칸 전진 (view_vector는 normalize를 했으므로, 1 만큼 전진함)
				//cur_coord = cur_coord + view_vector;
			}

			///마지막 값 이미지에 대입
			image[img_height * j + i] = static_cast<unsigned char>(max_value);
		}
	}

	return true;
}

float3 Renderer::cross(float3 vec1, float3 vec2)
{
	float res_x = vec1.y*vec2.z - vec1.z*vec2.y;
	float res_y = vec1.z*vec2.x - vec1.x*vec2.z;
	float res_z = vec1.x*vec2.y - vec1.y*vec2.x;

	float3 res = float3(res_x, res_y, res_z);
	return res;
}