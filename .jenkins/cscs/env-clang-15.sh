# Copyright (c) 2020 ETH Zurich
#
# SPDX-License-Identifier: BSL-1.0
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cxx_std="20"
clang_version="15.0.0"
boost_version="1.80.0"
hwloc_version="2.8.0"
pika_version="0.11.0"
spack_compiler="clang@${clang_version}"
spack_arch="cray-cnl7-broadwell"

spack_spec="pika-algorithms@main arch=${spack_arch} %${spack_compiler} cxxstd=${cxx_std} ^pika@${pika_version} malloc=system ^boost@${boost_version} ^hwloc@${hwloc_version}"

configure_extra_options+=" -DPIKA_ALGORITHMS_WITH_CXX_STANDARD=${cxx_std}"
