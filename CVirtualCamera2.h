#ifndef CVIRTUALCAMERA_H
#define CVIRTUALCAMERA_H

#include <memory>
#include <string>
#include <thread>

#include "libdshowcapture/source/output-filter.hpp"

class CVirtualCamera2 : public DShow::OutputFilter {
public:
    CVirtualCamera2();
    CVirtualCamera2(const CVirtualCamera2 &o) = delete;
    CVirtualCamera2(CVirtualCamera2 &&o) = delete;

    ~CVirtualCamera2() override;

    CVirtualCamera2& operator=(const CVirtualCamera2 &o) = delete;
    CVirtualCamera2& operator=(CVirtualCamera2 &&o) = delete;

    STDMETHODIMP Pause() override;
private:
    HANDLE start;
    HANDLE stop;
    std::thread thread;
};

#endif
