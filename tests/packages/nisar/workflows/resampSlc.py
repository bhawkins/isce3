#!/usr/bin/env python3
#
# Author: Liang Yu
# Copyright 2019-

import numpy as np
from nisar.workflows import resampSlc

class resamp_opts:
    '''
    class to emulate argparse terminal input
    member values set to test basic functionality
    values can be adjusted to meet test requirements
    '''
    product = '../../../lib/isce/data/envisat.h5'
    frequency = 'A'
    polarization = 'HH'
    offsetdir = '../../../lib/isce/data/offsets'
    outdir = ''


def test_resamp():
    '''
    run resample SLC without flattening and compare output against golden data
    '''
    # init inputs
    opts = resamp_opts()

    # run resamp
    resampSlc.main(opts)

    # load generated coregistered SLC
    test_slc = np.fromfile('coreg_{}.slc'.format(opts.polarization), dtype=np.complex64)

    # load reference SLC
    ref_slc = np.fromfile('../../../lib/isce/data/warped_envisat.slc', dtype=np.complex64)

    # get magnitude of mean difference
    mean_diff = np.abs(np.mean(test_slc - ref_slc))

    # check if mean difference within bounds
    print(mean_diff)
    assert(mean_diff < 1e-6)


if __name__ == '__main__':
    test_resamp()

# end of file
