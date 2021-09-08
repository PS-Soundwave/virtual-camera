#include "CVirtualCamera.h"

CVirtualCamera::CVirtualCamera(TCHAR *pName, LPUNKNOWN lpunk, CLSID clsid, HRESULT *pHr) : CSource(pName, lpunk, clsid) {
    pin = std::make_unique<CVirtualCameraPin>(nullptr, pHr, this, PIN_NAME);
}

CUnknown* WINAPI CVirtualCamera::CreateInstance(LPUNKNOWN pUnk, HRESULT *pHr) {
    return new CVirtualCamera(nullptr, pUnk, CLSID_VIRTUAL_CAMERA, pHr);
}
