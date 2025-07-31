#pragma once
#include "pch.h"
#include "SimpleMathHelper.h"

namespace MYTM
{
		/// 수동으로 만들어본 기본 변환
		D2D1_MATRIX_3X2_F MakeTranslationMatrix(D2D1_SIZE_F size);		// 이동
		D2D1_MATRIX_3X2_F MakeRotationMatrix_Origin(FLOAT angle);		// 원점에서의 회전, 스케일
		D2D1_MATRIX_3X2_F MakeScaleMatrix_Origin(D2D1_SIZE_F size);

		// 특정 점을 기준으로 한 회전
		D2D1_MATRIX_3X2_F MakeRotationMatrix(FLOAT angle, D2D1_POINT_2F center = D2D1::Point2F());

		// 특정 점을 기준으로 한 크기 변환
		D2D1_MATRIX_3X2_F MakeScaleMatrix(D2D1_SIZE_F size, D2D1_POINT_2F center = D2D1::Point2F());

		D2D1::Matrix3x2F MakeRenderMatrix(bool bUnityCoords = false, bool bMirror = false, float offsetX = 0, float offsetY = 0);


		// 디버그 출력을 위한 TM 문자열

		static void MakeMatrixToString(
				const D2D1_MATRIX_3X2_F& matrix,
				__out_ecount(bufSize) wchar_t* buffer,
				size_t bufSize
		);


		void DecomposeMatrix3x2
		(const D2D1::Matrix3x2F& M, MYHelper::Vector2F& outTranslation, float& outRotation, MYHelper::Vector2F& outScale);

		D2D1::Matrix3x2F RemovePivot(const D2D1::Matrix3x2F& M_local, const D2D1_POINT_2F& pivot);

		/// 사각형 rect 안에 point 가 포함되어 있으면 true 반환
		/// (경계 포함 검사)
		bool IsPointInRect(const D2D1_POINT_2F& point, const D2D1_RECT_F& rect) noexcept;
};
