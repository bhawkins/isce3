// -*- C++ -*-
// -*- coding: utf-8 -*-
//
// Author: Liang Yu
// Copyright 2019
//

#include "gpuCrossMul.h"
#include "gpuSignal.h"
#include "gpuLooks.h"
#include "isce/signal/Signal.h"
#include "isce/signal/Filter.h"
#include "isce/cuda/helper_cuda.h"
#include "isce/cuda/helper_functions.h"

// debug includes
#include <fstream>
#include <iostream>
#include <stdio.h>

#define THRD_PER_BLOCK 1024 // Number of threads per block (should always %32==0)

/*
output
    gpuComplex *ifgram (n_cols*n_rows)
input
    gpuComplex *refSlcUp ((oversample*n_ff)t*n_rows)
    gpuComplex *secSlcUp
    int n_rows
    int n_cols
    int n_fft
    int oversample
*/
template <typename T>
__global__ void interferogram_g(gpuComplex<T> *ifgram, 
        gpuComplex<T> *refSlcUp, 
        gpuComplex<T> *secSlcUp, 
        int n_rows, 
        int n_cols, 
        int n_fft, 
        int oversample_i,
        T oversample_f) 
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;

    // make sure index within ifgram size bounds
    if (i < n_rows * n_cols) {
        auto i_row = i / n_cols;
        auto i_col = i % n_cols;

        for (int j = 0; j < oversample_i; ++j) {
            auto ref_val = refSlcUp[i_row*oversample_i*n_fft + i_col];
            auto sec_val_conj = secSlcUp[i_row*oversample_i*n_fft + i_col];
            auto sec_val = gpuComplex<T>(sec_val_conj.r, -sec_val_conj.i);
            ifgram[i] += ref_val * sec_val_conj;
        }
        ifgram[i] /= oversample_f;
    }
}


template <>
__global__ void calculate_coherence_g<float>(float *ref_amp,
        float *sec_amp,
        gpuComplex<float> *ifgram_mlook,
        int n_elements)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;

    // make sure index within ifgram size bounds
    if (i < n_elements) {
        ifgram_mlook[i] = abs(ifgram_mlook[i]) / sqrtf(ref_amp[i] * sec_amp[i]);
    }
}


template <>
__global__ void calculate_coherence_g<double>(double *ref_amp,
        double *sec_amp,
        gpuComplex<double> *ifgram_mlook,
        int n_elements)
{
    int i = blockDim.x * blockIdx.x + threadIdx.x;

    // make sure index within ifgram size bounds
    if (i < n_elements) {
        ifgram_mlook[i] = abs(ifgram_mlook[i]) / sqrt(ref_amp[i] * sec_amp[i]);
    }
}


void isce::cuda::signal::gpuCrossmul::
doppler(isce::core::LUT1d<double> refDoppler, 
        isce::core::LUT1d<double> secDoppler)
{
    _refDoppler = refDoppler;
    _secDoppler = secDoppler;
}


void isce::cuda::signal::gpuCrossmul::
crossmul(isce::io::Raster& referenceSLC, 
        isce::io::Raster& secondarySLC,
        isce::io::Raster& interferogram,
        isce::io::Raster& coherence)
{

    _doCommonRangeBandFilter = false;
    isce::io::Raster rngOffsetRaster("/vsimem/dummy", 1, 1, 1, GDT_CFloat32, "ENVI");
    crossmul(referenceSLC, 
            secondarySLC,
            rngOffsetRaster,
            interferogram,
            coherence);

}

void isce::cuda::signal::gpuCrossmul::
crossmul(isce::io::Raster& referenceSLC,
        isce::io::Raster& secondarySLC,
        isce::io::Raster& rngOffsetRaster,
        isce::io::Raster& interferogram,
        isce::io::Raster& coherenceRaster)
{
    size_t nrows = referenceSLC.length();
    size_t ncols = referenceSLC.width();

    // setting the parameters of the multi-looking oject
    if (_doMultiLook) {
        // Making sure that the number of rows in each block (blockRows) 
        // to be an integer number of azimuth looks.
        blockRows = (blockRows/_azimuthLooks)*_azimuthLooks;
    }

    size_t blockRowsMultiLooked = blockRows/_azimuthLooks;
    size_t ncolsMultiLooked = ncols/_rangeLooks;

    // number of blocks to process
    size_t nblocks = nrows / blockRows;
    if (nblocks == 0) {
        nblocks = 1;
    } else if (nrows % (nblocks * blockRows) != 0) {
        nblocks += 1;
    }

    // signal object for upsampling
    isce::cuda::signal::gpuSignal<float> signalNoUpsample(CUFFT_C2C);
    isce::cuda::signal::gpuSignal<float> signalUpsample(CUFFT_C2C);
    
    // Compute FFT size (power of 2)
    size_t nfft;
    signalNoUpsample.nextPowerOfTwo(ncols, nfft);

    // set upsampling FFT plans
    signalNoUpsample.rangeFFT(nfft, blockRows);
    signalUpsample.rangeFFT(nfft*oversample, blockRows);

    // set not upsampled parameters
    auto n_slc = nfft*blockRows;
    auto slc_size = n_slc * sizeof(gpuComplex<float>);

    // storage for a block of reference SLC data
    std::valarray<std::complex<float>> refSlc(n_slc);
    gpuComplex<float> *d_refSlc;
    checkCudaErrors(cudaMalloc(reinterpret_cast<void **>(&d_refSlc), slc_size));

    // storage for a block of secondary SLC data
    std::valarray<std::complex<float>> secSlc(n_slc);
    gpuComplex<float> *d_secSlc;
    checkCudaErrors(cudaMalloc(reinterpret_cast<void **>(&d_secSlc), slc_size));

    // set upsampled parameters
    auto n_slcUpsampled = oversample * nfft * blockRows;
    auto slcUpsampled_size = n_slcUpsampled * sizeof(gpuComplex<float>);

    // upsampled block of reference SLC 
    std::valarray<std::complex<float>> refSlcUpsampled(n_slcUpsampled);
    gpuComplex<float> *d_refSlcUpsampled;
    checkCudaErrors(cudaMalloc(reinterpret_cast<void **>(&d_refSlcUpsampled), slcUpsampled_size));

    // upsampled block of secondary SLC
    std::valarray<std::complex<float>> secSlcUpsampled(n_slcUpsampled);
    gpuComplex<float> *d_secSlcUpsampled;
    checkCudaErrors(cudaMalloc(reinterpret_cast<void **>(&d_secSlcUpsampled), slcUpsampled_size));

    // shift impact
    std::valarray<std::complex<float>> shiftImpact(n_slcUpsampled);
    gpuComplex<float> *d_shiftImpact;
    lookdownShiftImpact(oversample,
            nfft,
            blockRows,
            shiftImpact);
    checkCudaErrors(cudaMalloc(reinterpret_cast<void **>(&d_shiftImpact), slcUpsampled_size));
    checkCudaErrors(cudaMemcpy(d_shiftImpact, &shiftImpact[0], slcUpsampled_size, cudaMemcpyHostToDevice));

    // interferogram
    auto n_ifgram = ncols * blockRows;
    auto ifgram_size = n_ifgram * sizeof(gpuComplex<float>);
    std::valarray<std::complex<float>> ifgram(n_ifgram);
    gpuComplex<float> *d_ifgram;
    checkCudaErrors(cudaMalloc(reinterpret_cast<void **>(&d_ifgram), ifgram_size));

    // range offset
    std::valarray<double> rngOffset(ncols*blockRows);
    gpuComplex<double> *d_rngOffset;
    auto rngOffset_size = ncols*nrows*sizeof(double);
    if (_doCommonRangeBandFilter) {
        // only malloc if we're using...
        checkCudaErrors(cudaMalloc(reinterpret_cast<void **>(&d_rngOffset), rngOffset_size));
    }

    // multilooked products container and parameters
    std::valarray<std::complex<float>> ifgram_mlook(0);
    std::valarray<float> coherence(0);
    float n_mlook(blockRowsMultiLooked*ncolsMultiLooked);

    // CUDA device memory allocation
    gpuComplex<float> *d_ifgram_mlook;
    float *d_ref_amp_mlook;
    float *d_sec_amp_mlook;

    if (_doMultiLook) {
        std::valarray<std::complex<float>> ifgram_mlook(blockRowsMultiLooked*ncolsMultiLooked);
        std::valarray<float> coherence(blockRowsMultiLooked*ncolsMultiLooked);
        auto mlook_size = blockRowsMultiLooked*ncolsMultiLooked*sizeof(float);
        checkCudaErrors(cudaMalloc(reinterpret_cast<void **>(&d_ifgram_mlook), 2*mlook_size)); // 2* because imaginary
        checkCudaErrors(cudaMalloc(reinterpret_cast<void **>(&d_ref_amp_mlook), mlook_size));
        checkCudaErrors(cudaMalloc(reinterpret_cast<void **>(&d_sec_amp_mlook), mlook_size));
    }

    // filter objects
    isce::cuda::signal::gpuAzimuthFilter<float> azimuthFilter;
    isce::cuda::signal::gpuRangeFilter<float> rangeFilter;

    // determine block layout
    dim3 block(THRD_PER_BLOCK);
    dim3 grid_hi((refSlc.size()*oversample+(THRD_PER_BLOCK-1))/THRD_PER_BLOCK);
    dim3 grid_reg((refSlc.size()+(THRD_PER_BLOCK-1))/THRD_PER_BLOCK);
    dim3 grid_lo((blockRowsMultiLooked*ncolsMultiLooked+(THRD_PER_BLOCK-1))/THRD_PER_BLOCK);

    // configure azimuth filter
    if (_doCommonAzimuthBandFilter) {
        std::valarray<std::complex<float>> refAzimuthSpectrum(nfft*blockRows);
        azimuthFilter.constructAzimuthCommonbandFilter(
                _refDoppler,
                _secDoppler,
                _commonAzimuthBandwidth,
                _prf,
                _beta,
                refSlc, refAzimuthSpectrum,
                nfft, blockRows);
    }
    
    // loop over all blocks
    for (size_t i_block = 0; i_block < nblocks; ++i_block) {
        std::cout << "i_block: " << i_block << std::endl;       
        // start row for this block
        size_t rowStart;
        rowStart = i_block * blockRows;
        
        //number of lines of data in this block. blockRowsData<= blockRows
        //Note that blockRows is fixed number of lines
        //blockRowsData might be less than or equal to blockRows.
        //e.g. if nrows = 512, and blockRows = 100, then 
        //blockRowsData for last block will be 12
        size_t blockRowsData;
        if ((rowStart + blockRows) > nrows) {
            blockRowsData = nrows - rowStart;
        } else {
            blockRowsData = blockRows;
        }

        // fill the valarray with zero before getting the block of the data
        refSlc = 0;
        secSlc = 0;
        refSlcUpsampled = 0;
        secSlcUpsampled = 0;
        ifgram = 0;

        // get a block of reference and secondary SLC data
        // and a block of range offsets
        // This will change once we have the functionality to 
        // get a block of data directly in to a slice
        std::valarray<std::complex<float>> dataLine(ncols);
        for (size_t line = 0; line < blockRowsData; ++line){
            referenceSLC.getLine(dataLine, rowStart + line);
            refSlc[std::slice(line*nfft, ncols, 1)] = dataLine;
            secondarySLC.getLine(dataLine, rowStart + line);
            secSlc[std::slice(line*nfft, ncols, 1)] = dataLine;
        }
        checkCudaErrors(cudaMemcpy(d_refSlc, &refSlc[0], slc_size, cudaMemcpyHostToDevice));
        checkCudaErrors(cudaMemcpy(d_secSlc, &secSlc[0], slc_size, cudaMemcpyHostToDevice));

        // apply azimuth filter (do inplace)
        if (_doCommonAzimuthBandFilter) {
            azimuthFilter.filter(d_refSlc);
            azimuthFilter.filter(d_secSlc);
        }

        // apply range filter (do inplace)
        if (_doCommonRangeBandFilter) {
            // Read range offsets
            std::valarray<double> offsetLine(ncols);
            for (size_t line = 0; line < blockRowsData; ++line){
                rngOffsetRaster.getLine(offsetLine, rowStart + line);
                rngOffset[std::slice(line*ncols, ncols, 1)] = offsetLine;
            }
            checkCudaErrors(cudaMemcpy(d_rngOffset, &rngOffset[0], rngOffset_size, cudaMemcpyHostToDevice));

            rangeFilter.filterCommonRangeBand(
                    reinterpret_cast<float *>(&d_refSlc), 
                    reinterpret_cast<float *>(&d_secSlc), 
                    reinterpret_cast<float *>(&d_rngOffset));
        }

        // upsample reference and secondary done on device
        upsample(signalNoUpsample,
                signalUpsample,
                d_refSlc, 
                d_refSlcUpsampled,
                d_shiftImpact);
        upsample(signalNoUpsample,
                signalUpsample,
                d_secSlc, 
                d_secSlcUpsampled,
                d_shiftImpact);
        
        // run kernels to compute oversampled interforgram
        // refSignal overwritten with upsampled interferogram
        // reduce from nfft*oversample*blockRows to ncols*blockRows
        float oversample_f = float(oversample);
        interferogram_g<<<grid_reg, block>>>(
                d_ifgram,
                d_refSlcUpsampled,
                d_secSlcUpsampled,
                nrows, ncols, nfft, oversample, oversample_f);

        if (_doMultiLook) {
            // reduce ncols*nrow to ncolsMultiLooked*blockRowsMultiLooked
            multilooks_g<<<grid_lo, block>>>(
                    d_ifgram_mlook,
                    d_refSlc,
                    ncols,
                    blockRowsMultiLooked,
                    oversample,                     // row resize factor of hi to lo
                    1,                              // col resize factor of hi to lo
                    nrows*ncols,                    // number of lo res elements
                    n_mlook);

            // get data to HOST
            checkCudaErrors(cudaMemcpy(&ifgram_mlook[0], d_ifgram_mlook, ifgram_mlook.size()*sizeof(float)*2, cudaMemcpyDeviceToHost));

            // write reduce+abs and set blocks
            multilooks_power_g<<<grid_lo, block>>>(
                    d_ref_amp_mlook,
                    d_refSlc,
                    2,
                    ncols,
                    blockRowsMultiLooked,
                    oversample,                     // row resize factor of hi to lo
                    1,                              // col resize factor of hi to lo
                    nrows*ncols,                    // number of lo res elements
                    n_mlook);

            multilooks_power_g<<<grid_lo, block>>>(
                    d_sec_amp_mlook,
                    d_secSlc,
                    2,
                    ncols,
                    blockRowsMultiLooked,
                    oversample,                     // row resize factor of hi to lo
                    1,                              // col resize factor of hi to lo
                    nrows*ncols,                    // number of lo res elements
                    n_mlook);

            // perform coherence calculation in place overwriting d_ifgram_mlook
            calculate_coherence_g<<<grid_lo, block>>>(d_ref_amp_mlook, 
                    d_sec_amp_mlook, 
                    d_ifgram_mlook, 
                    ifgram_mlook.size());

            // get data to HOST; overwrite multilooked ifgram with multilooked coherence
            checkCudaErrors(cudaMemcpy(&ifgram_mlook[0], d_ifgram_mlook, ifgram_mlook.size()*sizeof(float)*2, cudaMemcpyDeviceToHost));

            // set blocks accordingly
            interferogram.setBlock(ifgram_mlook, 0, rowStart/_azimuthLooks, 
                        ncols/_rangeLooks, blockRowsData/_azimuthLooks);

            coherenceRaster.setBlock(coherence, 0, rowStart/_azimuthLooks,
                        ncols/_rangeLooks, blockRowsData/_azimuthLooks);

        } else {
            // get data to HOST
            checkCudaErrors(cudaMemcpy(&ifgram[0], d_ifgram, ifgram_size, cudaMemcpyDeviceToHost));
        
            // set the block of interferogram
            interferogram.setBlock(ifgram, 0, rowStart, ncols, blockRowsData);
        }

    }

    // liberate all device memory
    checkCudaErrors(cudaFree(d_refSlc));
    checkCudaErrors(cudaFree(d_secSlc));
    checkCudaErrors(cudaFree(d_refSlcUpsampled));
    checkCudaErrors(cudaFree(d_secSlcUpsampled));
    checkCudaErrors(cudaFree(d_shiftImpact));
    checkCudaErrors(cudaFree(d_ifgram));
    if (_doCommonRangeBandFilter) {
        checkCudaErrors(cudaFree(d_rngOffset));
    }
    if (_doMultiLook) {
        checkCudaErrors(cudaFree(d_ifgram_mlook));
        checkCudaErrors(cudaFree(d_ref_amp_mlook));
        checkCudaErrors(cudaFree(d_sec_amp_mlook));
    }

}


/**
 * @param[in] oversample upsampling factor
 * @param[in] nfft fft length in range direction
 * @param[in] blockRows number of rows of the block of data
 * @param[out] shiftImpact frequency responce (a linear phase) to a sub-pixel shift in time domain introduced by upsampling followed by downsampling 
 */
void lookdownShiftImpact(size_t oversample, 
        size_t nfft, 
        size_t blockRows, 
        std::valarray<std::complex<float>> &shiftImpact)
{
    // range frequencies given nfft and oversampling factor
    std::valarray<double> rangeFrequencies(oversample*nfft);

    // sampling interval in range
    double dt = 1.0/oversample;

    // get the vector of range frequencies
    //filter object
    isce::signal::Filter<float>::fftfreq(oversample*nfft, dt, rangeFrequencies);
    

    // in the process of upsampling the SLCs, creating upsampled interferogram
    // and then looking down the upsampled interferogram to the original size of
    // the SLCs, a shift is introduced in range direction.
    // As an example for a signal with length of 5 and :
    // original sample locations:   0       1       2       3        4
    // upsampled sample locations:  0   0.5 1  1.5  2  2.5  3   3.5  4   4.5
    // Looked dow sample locations:   0.25    1.25    2.25    3.25    4.25
    // Obviously the signal after looking down would be shifted by 0.25 pixel in
    // range comared to the original signal. Since a shift in time domain introduces
    // a liner phase in frequency domain, we compute the impact in frequency domain.

    // the constant shift based on the oversampling factor
    double shift = 0.0;
    shift = (1.0 - 1.0/oversample)/2.0;

    // compute the frequency response of the subpixel shift in range direction
    std::valarray<std::complex<float>> shiftImpactLine(oversample*nfft);
    for (size_t col=0; col<shiftImpactLine.size(); ++col){
        double phase = -1.0*shift*2.0*M_PI*rangeFrequencies[col];
        shiftImpactLine[col] = std::complex<float> (std::cos(phase),
                                                    std::sin(phase));
    }

    // The imapct is the same for each range line. Therefore copying the line for the block
    for (size_t line = 0; line < blockRows; ++line){
            shiftImpact[std::slice(line*nfft*oversample, nfft*oversample, 1)] = shiftImpactLine;
    }
}

/*
forced instantiation
   */

template __global__
void interferogram_g<float>(gpuComplex<float> *ifgram, 
        gpuComplex<float> *refSlcUp, 
        gpuComplex<float> *secSlcUp, 
        int n_rows, 
        int n_cols, 
        int n_fft, 
        int oversample_i,
        float oversample_f);

