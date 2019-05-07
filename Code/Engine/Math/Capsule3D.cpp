#include "Engine/Math/Capsule3D.hpp"

//------------------------------------------------------------------------------------------------------------------------------
Capsule3D::Capsule3D()
{

}

//------------------------------------------------------------------------------------------------------------------------------
Capsule3D::Capsule3D(Vec3 start, Vec3 end, float radius /*= 1.f*/)
{
	m_start = start;
	m_end = end;
	m_radius = radius;
}

//------------------------------------------------------------------------------------------------------------------------------
Capsule3D::~Capsule3D()
{

}
