// -*- C++ -*-
// -*- coding: utf-8 -*-
//
// Source Author: Bryan Riel
// Copyright 2017-2018

#ifndef ISCE_PRODUCT_METADATA_H
#define ISCE_PRODUCT_METADATA_H

// isce::core
#include "isce/core/Orbit.h"

// isce::radar
#include "isce/radar/Radar.h"

// isce::product
#include "isce/product/Identification.h"

// Declarations
namespace isce {
    namespace product {
        class Metadata;
    }
}

// Declare Metadata class
class isce::product::Metadata {

    public:
        /** Default constructor */
        Metadata() {}

        /** Get NOE orbit */
        inline isce::core::Orbit orbitNOE() const;
        /** Set NOE orbit */
        inline void orbitNOE(const isce::core::Orbit &);

        /** Get POE orbit */
        inline isce::core::Orbit orbitPOE() const;
        /** Set POE orbit */
        inline void orbitPOE(const isce::core::Orbit &);

        /** Get attitude */
        inline isce::core::Attitude attitude() const;
        /** Set attitude */
        inline void attitude(const isce::core::Attitude &);

        /** Get radar instrument */
        inline isce::core::Radar instrument() const;
        /** Set instrument */
        inline void instrument(const isce::radar::Radar &);

        /** Get identification */
        inline Identification identification() const;
        /** Set identification */
        inline void identification(Identification &);

    private:
        // Orbits
        isce::core::Orbit _orbitMOE;
        isce::core::Orbit _orbitNOE;
        isce::core::Orbit _orbitPOE;

        // Attitudes
        isce::core::Attitude _attitude;

        // Radar instrument
        isce::radar::Radar _instrument;

        // Identification
        Identification _id;
};

// Get inline implementations for Metadata
#define ISCE_PRODUCT_METADATA_ICC
#include "Metadata.icc"
#undef ISCE_PRODUCT_METADATA_ICC

#endif

// end of file
