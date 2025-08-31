#include "pch.h"
#include "Transform.h"

void D2DTM::Transform::SetPivotPreset(PivotPreset preset, const D2D1_SIZE_F& size)
{
		switch (preset)
		{
		case PivotPreset::TopLeft:
				m_pivot = { 0.0f, 0.0f };
				break;
		case PivotPreset::TopRight:
				m_pivot = { size.width, 0.0f };
				break;
		case PivotPreset::BottomLeft:
				m_pivot = { 0.0f, size.height };							// y에 마이너스 부호 뺌
				break;
		case PivotPreset::BottomRight:
				m_pivot = { size.width, size.height };						// y에 마이너스 부호 뺌
				break;
		case PivotPreset::Center:
				m_pivot = { size.width * 0.5f, (size.height * 0.5f) };		// y에 마이너스 부호 뺌

				break;
		}
}


void  D2DTM::Transform::UpdateMatrices()
{

		m_matrixLocal =
				D2D1::Matrix3x2F::Scale(m_scale.x, m_scale.y, m_pivot) *
				D2D1::Matrix3x2F::Rotation(m_rotation, m_pivot) *
				D2D1::Matrix3x2F::Translation(m_position.x, m_position.y);

		if (m_parent)
				m_matrixWorld = m_matrixLocal * m_parent->GetWorldMatrix();
		else
				m_matrixWorld = m_matrixLocal;

		m_dirty = false;
}