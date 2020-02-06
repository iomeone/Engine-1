#include "Engine/Math/ConvexPoly2D.hpp"

//------------------------------------------------------------------------------------------------------------------------------
ConvexPoly2D::ConvexPoly2D()
{

}

//------------------------------------------------------------------------------------------------------------------------------
ConvexPoly2D::ConvexPoly2D(const std::vector<Vec2>& constructPoints)
{
	m_convexPolyPoints = constructPoints;
}

//------------------------------------------------------------------------------------------------------------------------------
ConvexPoly2D::~ConvexPoly2D()
{

}

//------------------------------------------------------------------------------------------------------------------------------
const std::vector<Vec2>& ConvexPoly2D::GetConvexPoly2DPoints() const
{
	return m_convexPolyPoints;
}

//------------------------------------------------------------------------------------------------------------------------------
void ConvexPoly2D::SetBitFieldsForBitBucketBroadPhase(const IntVec2& bitFields)
{
	m_bitFieldsXY = bitFields;
}

