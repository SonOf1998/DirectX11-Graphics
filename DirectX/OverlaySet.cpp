#include "pch.h"
#include "OverlaySet.h"

// from DirectX
#include "OverlayObject.h"

OverlaySet::OverlaySet(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
}

OverlaySet::~OverlaySet() = default;


void OverlaySet::AddOverlayItem(std::unique_ptr<OverlayObject>&& overlayItem)
{
	overlays.push_back(std::move(overlayItem));
}

void OverlaySet::KillOverlayWithTTL()
{
	for (auto& overlay : overlays)
	{
		overlay->Kill();
	}
}

void OverlaySet::MarkForUpdate()
{
	for (auto& overlay : overlays)
	{
		overlay->MarkForUpdate();
	}
}

void OverlaySet::Render(ID3D11DeviceContext * device, Pipeline * pipeline, Camera * camera, Light * light)
{
	for (auto& overlay : overlays)
	{
		overlay->Render(device, pipeline, camera, light);
	}
}

void OverlaySet::Animate(float t, float dt)
{
	for (auto& overlay : overlays)
	{
		overlay->Animate(t, dt);
	}

	auto newEnd = std::remove_if(overlays.begin(), overlays.end(), [](std::unique_ptr<OverlayObject>& obj) { return !obj->StillLiving();  });
	overlays.erase(newEnd, overlays.end());
}
