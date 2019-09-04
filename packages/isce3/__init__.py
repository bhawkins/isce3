# -*- Python -*-
# -*- coding: utf-8 -*-
#
# eric m. gurrola <eric.gurrola@jpl.nasa.gov>
# california institute of technology, jet propulsion laboratory
# (c) 2010
#
#-*- coding: utf-8 -*-


# import and publish pyre symbols
from pyre import (
    # protocols, components, traits, and their infrastructure
    schemata, constraints, properties, protocol, component, foundry,
    # component interface decorators
    export, provides,
    # the manager of the pyre runtime
    executive,
    # multidimensional containers
    grid,
    # concurrency
    nexus,
    # workflows
    flow,
    # miscellaneous
    tracking, units, weaver, primitives,
    )


# bootstrap
package = executive.registerPackage(name='isce3', file=__file__)
# publish the package layout
home, prefix, defaults = package.layout()


# publish my extension modules
from .extensions import (
    libisce,     # the pure CPU implementation of isce
)

# publish the local modules
from . import (
    meta,                # package meta-data
    shells,              # application support
    cli,                 # command panels that client packages may extend
    # abstractions
    protocols,           # abstract specifications of isce3 entities
    # support
    core,                # access to low level core objects from libisce3
    # concrete components
    products,            # data
    factories,           # data processors
    readers,             # parsers of the standard formats for encoding isce3 products
    workflows,           # built in workflows
)


# singletons
wgs84 = core.newEllipsoid(name="wgs84")


# administrative
def copyright():
    """
    Print the isce3 copyright note
    """
    return print(meta.header)


def license():
    """
    Print the isce3 license
    """
    # print it
    return print(meta.license)


def version():
    """
    Return the isce3 version
    """
    return meta.version


def credits():
    """
    Print the acknowledgments
    """
    # print it
    return print(meta.acknowledgments)


# end of file
