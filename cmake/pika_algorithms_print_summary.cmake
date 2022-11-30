# Copyright (c) 2017-2019 Hartmut Kaiser
#
# SPDX-License-Identifier: BSL-1.0
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

function(pika_algorithms_create_configuration_summary message module_name)

  set(pika_algorithms_config_information)
  set(upper_cfg_name "PIKA_ALGORITHMS")
  set(upper_option_suffix "")

  string(TOUPPER ${module_name} module_name_uc)
  if(NOT "${module_name_uc}x" STREQUAL "PIKA_ALGORITHMSx")
    set(upper_cfg_name "PIKA_ALGORITHMS_${module_name_uc}")
    set(upper_option_suffix "_${module_name_uc}")
  endif()

  get_property(
    DEFINITIONS_VARS GLOBAL
    PROPERTY PIKA_ALGORITHMS_CONFIG_DEFINITIONS${upper_option_suffix}
  )
  if(DEFINED DEFINITIONS_VARS)
    list(SORT DEFINITIONS_VARS)
    list(REMOVE_DUPLICATES DEFINITIONS_VARS)
  endif()

  get_property(
    _variableNames GLOBAL
    PROPERTY PIKA_ALGORITHMS_MODULE_CONFIG_${module_name_uc}
  )
  list(SORT _variableNames)

  # Only print the module configuration if options specified
  list(LENGTH _variableNames _length)
  if(${_length} GREATER_EQUAL 1)
    pika_algorithms_info("")
    pika_algorithms_info(${message})

    foreach(_variableName ${_variableNames})
      if(${_variableName}Category)

        # handle only options which start with PIKA_ALGORITHMS_WITH_
        string(FIND ${_variableName} "${upper_cfg_name}_WITH_" __pos)

        if(${__pos} EQUAL 0)
          get_property(
            _value
            CACHE "${_variableName}"
            PROPERTY VALUE
          )
          pika_algorithms_info("    ${_variableName}=${_value}")
        endif()

        string(REPLACE "_WITH_" "_HAVE_" __variableName ${_variableName})
        list(FIND DEFINITIONS_VARS ${__variableName} __pos)
        if(NOT ${__pos} EQUAL -1)
          set(pika_algorithms_config_information
              "${pika_algorithms_config_information}"
              "\n        \"${_variableName}=${_value}\","
          )
        elseif(NOT ${_variableName}Category STREQUAL "Generic"
               AND NOT ${_variableName}Category STREQUAL "Build Targets"
        )
          get_property(
            _type
            CACHE "${_variableName}"
            PROPERTY TYPE
          )
          if(_type STREQUAL "BOOL")
            set(pika_algorithms_config_information
                "${pika_algorithms_config_information}"
                "\n        \"${_variableName}=OFF\","
            )
          endif()
        endif()
      endif()
    endforeach()
  endif()

  if(pika_algorithms_config_information)
    string(REPLACE ";" "" pika_algorithms_config_information
                   ${pika_algorithms_config_information}
    )
  endif()

  set(_base_dir_local "pika/config")
  set(_base_dir "pika/config")
  set(_template "config_defines_strings.hpp.in")

  if("${module_name_uc}" STREQUAL "PIKA")
    configure_file(
      "${PIKA_ALGORITHMS_SOURCE_DIR}/cmake/templates/${_template}"
      "${PIKA_ALGORITHMS_BINARY_DIR}/${_base_dir_local}/config_strings.hpp"
      @ONLY
    )
    configure_file(
      "${PIKA_ALGORITHMS_SOURCE_DIR}/cmake/templates/${_template}"
      "${PIKA_ALGORITHMS_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${_base_dir}/config_strings.hpp"
      @ONLY
    )
  endif()
endfunction()
