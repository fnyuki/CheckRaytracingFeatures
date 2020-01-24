#pragma once

#define SHOW_MESSAGE_AS_DIALOG 0

#include <windows.h>
#include <comdef.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <string_view>
#include <type_traits>
#if !SHOW_MESSAGE_AS_DIALOG
#	include <iostream>
#endif

template <class T>
void SafeRelease(T*& p)
{
	static_assert(std::is_base_of_v<IUnknown, T>);
	if (p != nullptr)
	{
		p->Release();
		p = nullptr;
	}
}

inline void ShowDialog(const std::string_view title, const std::string_view detail)
{
#if !SHOW_MESSAGE_AS_DIALOG
	std::cout << title.data() << ": " << detail.data() << std::endl;
#else
	MessageBox(nullptr, detail.data(), title.data(), 0);
#endif
}

inline const char* GetErrorMessage(const HRESULT hr)
{
	return _com_error(hr).ErrorMessage();
}

constexpr const char* GetRaytracingTierString(const D3D12_RAYTRACING_TIER tier)
{
	return tier == D3D12_RAYTRACING_TIER_NOT_SUPPORTED	? "Not supported" :
	       tier == D3D12_RAYTRACING_TIER_1_0			? "1.0" :
	       tier == D3D12_RAYTRACING_TIER_1_1			? "1.1" :
	       "Not found";
};

bool CreateFactory(HRESULT& hr, IDXGIFactory7*& factory);

bool CreateAdapter(IDXGIFactory7* const factory, HRESULT& hr, IDXGIAdapter1*& adapter);

bool CreateDevice(IDXGIAdapter1* const adapter, HRESULT& hr, ID3D12Device6*& device);

bool CheckRaytracingFeatureSupport(ID3D12Device6* const device, HRESULT& hr, D3D12_FEATURE_DATA_D3D12_OPTIONS5& featureData);