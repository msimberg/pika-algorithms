# Copyright (c) 2020-2022 ETH Zurich
#
# SPDX-License-Identifier: BSL-1.0
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cxx_std="17"
gcc_version="10.3.0"
apex_version="2.4.1"
pika_version="0.11.0"
spack_compiler="gcc@${gcc_version}"
spack_arch="cray-cnl7-haswell"

# apex +openmp does not currently build so we disable openmp
spack_spec="pika-algorithms@main arch=${spack_arch} %${spack_compiler} cxxstd=${cxx_std} ^pika@${pika_version} +apex malloc=system ^apex@${apex_version} ~openmp ~papi"

configure_extra_options+=" -DPIKA_ALGORITHMS_WITH_CXX_STANDARD=${cxx_std}"

build_extra_options+=" -j10"
