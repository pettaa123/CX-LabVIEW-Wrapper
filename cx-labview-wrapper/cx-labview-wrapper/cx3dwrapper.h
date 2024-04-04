#pragma once

#ifdef CXLIBDLL_EXPORTS
#define CXLIBDLL_API __declspec(dllexport)
#else
#define CXLIBDLL_API __declspec(dllimport)
#endif

#include "fundtypes.h"
#include "extcode.h"

// This prolog and epilog should be used around any typedef that is passed from LabVIEW
// to a DLL for parameters configured as LabVIEW type or Adapt to Type
#include "lv_prolog.h"
#include "platdefines.h"

// LabVIEW created typedef
typedef struct {
    int32 dimSizes[2];
    uint16_t elt[1];
} Arr2D_U16;
typedef Arr2D_U16** Arr2D_U16Hdl;

#include "lv_epilog.h"

#if IsOpSystem64Bit
#define uPtr uQ //unsigned Quad aka 64-bit
#else
#define uPtr uL //unsigned Long aka 32-bit
#endif

extern "C" CXLIBDLL_API int myTest();

extern "C" CXLIBDLL_API int32_t createPointcloud(const char *calibId,const char *calibFile,const char *pcdFile, Arr2D_U16Hdl arr);

extern "C" CXLIBDLL_API int32_t writeTif_U16(const char* file, Arr2D_U16Hdl arr);
extern "C" CXLIBDLL_API int32_t readTif_U16(const char* file, Arr2D_U16Hdl arr);



