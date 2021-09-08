#ifndef CVIRTUALCAMERA_H
#define CVIRTUALCAMERA_H

#include <memory>
#include <string>

#include "baseclasses/streams.h"

#include "CVirtualCameraPin.h"

class CVirtualCamera : public CSource {
public:
    static constexpr LPCWSTR NAME = L"Alpha Test Camera";
    static constexpr LPCWSTR PIN_NAME = L"Output";
    static constexpr CLSID CLSID_VIRTUAL_CAMERA = {0xB6AC2701, 0x2612, 0x483F, {0x88, 0xCF, 0x98, 0x45, 0x1E, 0xD7, 0xE0, 0xF2}};

    CVirtualCamera(TCHAR *pName, LPUNKNOWN lpunk, CLSID clsid, HRESULT *pHr);
    CVirtualCamera(const CVirtualCamera &o) = delete;
    CVirtualCamera(CVirtualCamera &&o) = delete;

    ~CVirtualCamera() override = default;

    CVirtualCamera& operator=(const CVirtualCamera &o) = delete;
    CVirtualCamera& operator=(CVirtualCamera &&o) = delete;

    static CUnknown* WINAPI CreateInstance(LPUNKNOWN pUnk, HRESULT *pHr);

private:
    std::unique_ptr<CVirtualCameraPin> pin;
};

#endif
