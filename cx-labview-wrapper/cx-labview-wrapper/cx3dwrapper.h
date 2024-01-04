#pragma once
#include "fundtypes.h"
#include "extcode.h"

#ifdef CX3DLIBDLL_EXPORTS
#define CX3DLIBDLL_API __declspec(dllexport)
#else
#define CX3DLIBDLL_API __declspec(dllimport)
#endif

// This prolog and epilog should be used around any typedef that is passed from LabVIEW
// to a DLL for parameters configured as LabVIEW type or Adapt to Type
#include "lv_prolog.h"
#include "platdefines.h"

/* LabVIEW created typedef */
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

extern "C" CX3DLIBDLL_API int32_t createPointcloud(char *calibId, char *calibFile, char *pcdFile, Arr2D_U16Hdl arr);

extern "C" CX3DLIBDLL_API int32_t writeTif_U16(char* file, Arr2D_U16Hdl arr);
extern "C" CX3DLIBDLL_API int32_t readTif_U16(char* file, Arr2D_U16Hdl arr);




