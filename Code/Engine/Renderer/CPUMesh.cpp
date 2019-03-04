#include "Engine/Renderer/CPUMesh.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Commons/ErrorWarningAssert.hpp"
#include "Engine/Math/AABB3.hpp"
#include "Engine/Math/MathUtils.hpp"

CPUMesh::CPUMesh()
{
	Clear();
}

//------------------------------------------------------------------------
void CPUMeshAddQuad( CPUMesh *out, AABB2 quad)
{
	out->Clear(); 

	out->SetColor( Rgba::WHITE ); 
	// out->SetNormal( vec3::BACK ); 

	// 0 --- 1
	// |   / |
	// | /   |
	// 2 --- 3
	out->SetUV( Vec2(0.0f, 0.0f) ); 
	out->AddVertex( quad.GetTopLeft() ); 

	out->SetUV( Vec2(1.0f, 0.0f) ); 
	out->AddVertex( quad.GetTopRight() ); 

	out->SetUV( Vec2(0.0f, 1.0f) ); 
	out->AddVertex( quad.GetBottomLeft() ); 

	out->SetUV( Vec2(1.0f, 1.0f) ); 
	out->AddVertex( quad.GetBottomRight() ); 

	out->AddIndexedTriangle( 0, 2, 1 ); 
	out->AddIndexedTriangle( 2, 3, 1 ); 
}

void CPUMeshAddBoxFace(CPUMesh *out, AABB2 quad)
{
	out->SetColor( Rgba::WHITE ); 
	// out->SetNormal( vec3::BACK ); 

	// 0 --- 1
	// |   / |
	// | /   |
	// 2 --- 3
	out->SetUV( Vec2(0.0f, 0.0f) ); 
	out->AddVertex( quad.GetTopLeft() ); 

	out->SetUV( Vec2(1.0f, 0.0f) ); 
	out->AddVertex( quad.GetTopRight() ); 

	out->SetUV( Vec2(0.0f, 1.0f) ); 
	out->AddVertex( quad.GetBottomLeft() ); 

	out->SetUV( Vec2(1.0f, 1.0f) ); 
	out->AddVertex( quad.GetBottomRight() ); 

	out->AddIndexedTriangle( 0, 2, 1 ); 
	out->AddIndexedTriangle( 2, 3, 1 ); 
}

void CPUMeshAddCube( CPUMesh *out, AABB3 box, bool clearMesh )
{
	if(clearMesh)
	{
		out->Clear();
	}

	out->SetColor( Rgba::WHITE ); 

	AABB2 boxFace;

	//Add front face
	// 0 --- 1
	// |   / |
	// | /   |
	// 2 --- 3
	out->SetUV( Vec2(0.0f, 0.0f) ); 
	out->AddVertex( box.m_frontTopLeft); 
	out->SetUV( Vec2(1.0f, 0.0f) ); 
	out->AddVertex( box.m_frontTopRight ); 
	out->SetUV( Vec2(0.0f, 1.0f) ); 
	out->AddVertex( box.m_frontBottomLeft ); 
	out->SetUV( Vec2(1.0f, 1.0f) ); 
	out->AddVertex( box.m_frontBottomRight ); 

	out->AddIndexedTriangle( 0, 2, 1 ); 
	out->AddIndexedTriangle( 2, 3, 1 ); 

	//Add back face
	// 4 --- 5
	// |   / |
	// | /   |
	// 6 --- 7
	out->SetUV( Vec2(0.0f, 0.0f) ); 
	out->AddVertex( box.m_backTopLeft); 
	out->SetUV( Vec2(1.0f, 0.0f) ); 
	out->AddVertex( box.m_backTopRight ); 
	out->SetUV( Vec2(0.0f, 1.0f) ); 
	out->AddVertex( box.m_backBottomLeft ); 
	out->SetUV( Vec2(1.0f, 1.0f) ); 
	out->AddVertex( box.m_backBottomRight ); 

	out->AddIndexedTriangle( 4, 6, 5 ); 
	out->AddIndexedTriangle( 6, 7, 5 ); 

	//Add top face
	// 8 --- 9
	// |   / |
	// | /   |
	// 10 --- 11
	out->SetUV( Vec2(0.0f, 0.0f) ); 
	out->AddVertex( box.m_backTopRight); 
	out->SetUV( Vec2(1.0f, 0.0f) ); 
	out->AddVertex( box.m_backTopLeft ); 
	out->SetUV( Vec2(0.0f, 1.0f) ); 
	out->AddVertex( box.m_frontTopLeft ); 
	out->SetUV( Vec2(1.0f, 1.0f) ); 
	out->AddVertex( box.m_frontTopRight ); 

	out->AddIndexedTriangle( 8, 10, 9 ); 
	out->AddIndexedTriangle( 10, 11, 9 );

	//Add bottom face
	// 12 --- 13
	// |   / |
	// | /   |
	// 14 --- 15
	out->SetUV( Vec2(0.0f, 0.0f) ); 
	out->AddVertex( box.m_frontBottomLeft); 
	out->SetUV( Vec2(1.0f, 0.0f) ); 
	out->AddVertex( box.m_frontBottomRight ); 
	out->SetUV( Vec2(0.0f, 1.0f) ); 
	out->AddVertex( box.m_backBottomRight ); 
	out->SetUV( Vec2(1.0f, 1.0f) ); 
	out->AddVertex( box.m_backBottomLeft ); 

	out->AddIndexedTriangle( 12, 14, 13 ); 
	out->AddIndexedTriangle( 14, 15, 13 );

	//Add left face
	// 16 --- 17
	// |   / |
	// | /   |
	// 18 --- 19
	out->SetUV( Vec2(0.0f, 0.0f) ); 
	out->AddVertex( box.m_backTopRight); 
	out->SetUV( Vec2(1.0f, 0.0f) ); 
	out->AddVertex( box.m_frontTopLeft); 
	out->SetUV( Vec2(0.0f, 1.0f) ); 
	out->AddVertex( box.m_backBottomRight ); 
	out->SetUV( Vec2(1.0f, 1.0f) ); 
	out->AddVertex( box.m_frontBottomLeft ); 

	out->AddIndexedTriangle( 16, 18, 17 ); 
	out->AddIndexedTriangle( 18, 19, 17 );

	//Add right face
	// 20 --- 21
	// |   / |
	// | /   |
	// 22 --- 23
	out->SetUV( Vec2(0.0f, 0.0f) ); 
	out->AddVertex( box.m_frontTopRight); 
	out->SetUV( Vec2(1.0f, 0.0f) ); 
	out->AddVertex( box.m_backTopLeft); 
	out->SetUV( Vec2(0.0f, 1.0f) ); 
	out->AddVertex( box.m_frontBottomRight ); 
	out->SetUV( Vec2(1.0f, 1.0f) ); 
	out->AddVertex( box.m_backBottomLeft ); 

	out->AddIndexedTriangle( 20, 22, 21 ); 
	out->AddIndexedTriangle( 22, 23, 21 );
}

void CPUMeshAddUVSphere( CPUMesh *out, Vec3 center, float radius, uint wedges /*= 32*/, uint slices /*= 16 */ )
{
	out->Clear();

	int ustep = wedges + 1;
	int vstep = slices + 1;

	float phi;			//Angle along the j,k plane
	float theta;		//Angle along the i,k plane

	//Map out all the vertices
	for(int vIndex = 0; vIndex < vstep; vIndex++)
	{
		float v = static_cast<float>(vIndex) / static_cast<float>(slices);
		phi = RangeMapFloat(v, 0.f, 1.f, 90.f, 270.f);

		for(int uIndex = 0; uIndex < ustep; uIndex++)
		{
			float u = static_cast<float>(uIndex) / static_cast<float>(wedges);
			theta = u * 360.f;
			Vec3 position = GetSphericalToCartesian(radius, theta, phi) + center;
			out->SetUV(Vec2(u,v));
			out->AddVertex(position);
		}
	}

	//Map out all the Indices
	for(int y = 0; y < static_cast<int>(slices); y++)
	{
		for(int x = 0; x < static_cast<int>(wedges); x++)
		{
			uint TL = y * ustep + x;
			uint TR = TL + 1;
			uint BL = TL + ustep;
			uint BR = BL + 1;
			out->AddIndexedQuad(TL, TR, BL, BR);
		}
	}

}

//------------------------------------------------------------------------
void CPUMesh::AddIndexedTriangle( uint i0, uint i1, uint i2 )
{
	ASSERT_RECOVERABLE( i0 < m_vertices.size() , "Index is greater than the number of vertices");
	ASSERT_RECOVERABLE( i1 < m_vertices.size() , "Index is greater than the number of vertices");
	ASSERT_RECOVERABLE( i2 < m_vertices.size() , "Index is greater than the number of vertices");
	//Push it into the vertex vector 

	m_indices.push_back(i0);
	m_indices.push_back(i1);
	m_indices.push_back(i2);
}

uint CPUMesh::GetIndexCount() const
{
	return static_cast<int>(m_indices.size());
}

uint CPUMesh::GetVertexCount() const
{
	return static_cast<int>(m_vertices.size());
}

void CPUMesh::Clear()
{
	m_vertices.clear();
	m_indices.clear();

	m_stamp.position = Vec3::ZERO;
	m_stamp.color = Rgba::WHITE;
	m_stamp.uv = Vec2::ZERO;
}

CPUMesh::~CPUMesh()
{
	Clear();
}

void CPUMesh::SetColor( Rgba color )
{
	m_stamp.color = color;
}

void CPUMesh::SetUV( Vec2 uv )
{
	m_stamp.uv = uv;
}

uint CPUMesh::AddVertex( VertexMaster const &m )
{
	m_vertices.push_back(m);
	
	uint index = static_cast<uint>(m_vertices.size());
	index--;
	
	return index;
}

uint CPUMesh::AddVertex( Vec3 const &pos )
{
	VertexMaster m = m_stamp;
	m.position = pos;

	m_vertices.push_back(m);

	uint index = static_cast<uint>(m_vertices.size());
	index--;

	return index;
}

void CPUMesh::AddIndexedQuad( uint topLeft, uint topRight, uint bottomLeft, uint bottomRight )
{
	AddIndexedTriangle(bottomLeft, topRight, topLeft);
	AddIndexedTriangle(bottomLeft, bottomRight, topRight);
}