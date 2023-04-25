#include "texture.h"
#include "debug.h"
#include "graphics.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(std::string filePath, DXGI_FORMAT texFormat)
{
    int32_t width;
    int32_t height;
    int32_t channels;
    
    // load raw image data with stb_image
    unsigned char *img = stbi_load(filePath.c_str(), &width, &height, &channels, 0);

    if(img == NULL)
    {
        Debug("couldn't load image file");
    }

    // create texture
    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.Format = texFormat;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

    HRESULT HR = Graphics::device->CreateTexture2D(&textureDesc, NULL, textureResource.GetAddressOf());
    Debug(HR, "failed to create texture 2D");
    Graphics::context->UpdateSubresource(textureResource.Get(), 0, NULL, img, (width * 4) * sizeof(unsigned char), 0);
    
    // create sampler state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    HR = Graphics::device->CreateSamplerState(&sampDesc, samplerState.GetAddressOf());
    Debug(HR, "failed to create sampler state when creating texture");

    // create shader resource view
    D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = textureDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MostDetailedMip = 0;
	viewDesc.Texture2D.MipLevels = -1;
    HR = Graphics::device->CreateShaderResourceView(textureResource.Get(), &viewDesc, resourceView.GetAddressOf());
    Debug(HR, "failed to create SRV for texture");
}