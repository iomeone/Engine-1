//------------------------------------------------------------------------------------------------------------------------------
#include "Engine/Renderer/TextureView.hpp"
#include "Engine/Renderer/Sampler.hpp"

//------------------------------------------------------------------------------------------------------------------------------
#define DX_SAFE_RELEASE(dx_resource)   if ((dx_resource) != nullptr) { dx_resource->Release(); dx_resource = nullptr; }

//------------------------------------------------------------------------------------------------------------------------------
TextureView::TextureView()
{

}

TextureView::~TextureView()
{
	DX_SAFE_RELEASE(m_view);
	DX_SAFE_RELEASE(m_source);
}