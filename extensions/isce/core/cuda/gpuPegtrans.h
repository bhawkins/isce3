//
// Author: Joshua Cohen
// Copyright 2017
//

#ifndef ISCELIB_GPUPEGTRANS_H
#define ISCELIB_GPUPEGTRANS_H

#include "gpuEllipsoid.h"
#include "gpuPeg.h"

namespace isceLib {
    struct gpuPegtrans {
        double mat[3][3];
        double matinv[3][3];
        double ov[3];
        double radcur;

        __host__ __device__ gpuPegtrans(double rdc) : radcur(rdc) {}                // Value constructor
        __host__ __device__ gpuPegtrans() : gpuPegtrans(0.) {}                      // Default constructor (delegated)
        __host__ __device__ gpuPegtrans(const gpuPegtrans&) = delete;               // Delete copy constructors (managing the internal memory of this is tricky and
        __host__ __device__ gpuPegtrans& operator=(const gpuPegtrans&) = delete;    //  this Pegtrans class only gets created on the device)

        __device__ void radar2xyz(gpuEllipsoid&,gpuPeg&);
        __device__ void xyz2sch(double*,double*);
    };
}

#endif
