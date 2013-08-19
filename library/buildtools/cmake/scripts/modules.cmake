
function(add_module MODULE_NAME)
  string(TOUPPER ${MODULE_NAME} CURRENT_MODULE_NAME)
  string(TOLOWER ${MODULE_NAME} LOWER_MODULE_NAME)

  setprop(CURRENT_MODULE_NAME ${CURRENT_MODULE_NAME})
  setprop(${CURRENT_MODULE_NAME}_PATH ${CMAKE_CURRENT_SOURCE_DIR}/)
  setprop(${CURRENT_MODULE_NAME}_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/cmakelists.txt)

  appendprop(MODULES_LIST ${CURRENT_MODULE_NAME})
  
  set(MODULE ${LOWER_MODULE_NAME} PARENT_SCOPE)
endfunction(add_module)

function(module_add_includes PATH)
  getprop(MODULE_NAME CURRENT_MODULE_NAME)
  getprop(MODULE_PATH ${MODULE_NAME}_PATH)
  
  appendprop(${MODULE_NAME}_INCLUDE ${MODULE_PATH}${PATH}/)
endfunction(module_add_includes)

function(module_add_sources PATH)
  getprop(MODULE_NAME CURRENT_MODULE_NAME)
  getprop(MODULE_PATH ${MODULE_NAME}_PATH)
  
  appendprop(${MODULE_NAME}_SOURCE ${MODULE_PATH}${PATH})
endfunction(module_add_sources)

function(module_add_sources_recursive DIRNAME FILTER)
  getprop(MODULE_NAME CURRENT_MODULE_NAME)
  getprop(MODULE_PATH ${MODULE_NAME}_PATH)

  set(PATH "${MODULE_PATH}${DIRNAME}")
  file(GLOB CHILDREN RELATIVE ${PATH} ${PATH}/*)

  module_add_sources("${DIRNAME}/${FILTER}")

  foreach(CHILD ${CHILDREN})
    if(IS_DIRECTORY ${PATH}/${CHILD})
		set(CHILD_DIR "${DIRNAME}/${CHILD}")
        module_add_sources_recursive(${CHILD_DIR} ${FILTER})
    endif()
  endforeach()
endfunction()

function(module_add_libraries PATH)
  getprop(MODULE_NAME CURRENT_MODULE_NAME)
  getprop(MODULE_PATH ${MODULE_NAME}_PATH)
  
  appendprop(${MODULE_NAME}_LIBRARY ${MODULE_PATH}${PATH})
endfunction(module_add_libraries)

function(module_add_binarys PATH)
  getprop(MODULE_NAME CURRENT_MODULE_NAME)
  getprop(MODULE_PATH ${MODULE_NAME}_PATH)
  
  appendprop(${MODULE_NAME}_BINARY ${MODULE_PATH}${PATH})
endfunction(module_add_binarys)

function(module_add_linklib NAME)
  getprop(MODULE_NAME CURRENT_MODULE_NAME)
  appendprop(${MODULE_NAME}_LINKLIB ${NAME})
endfunction(module_add_linklib)

function(module_add_binlib NAME)
  getprop(MODULE_NAME CURRENT_MODULE_NAME)
  appendprop(${MODULE_NAME}_BINLIB ${NAME})
endfunction(module_add_binlib)

function(module_import MODULE_NAME)
  string(TOUPPER ${MODULE_NAME} NAME)

  getprop(MODULE_NAME CURRENT_MODULE_NAME)
  appendprop(${MODULE_NAME}_DEPENDENCIES ${NAME})
endfunction(module_import)

function(module_set_c)
  getprop(MODULE_NAME CURRENT_MODULE_NAME)
  setprop(${MODULE_NAME}_C TRUE)
endfunction(module_set_c)

function(module_output_debugdata MODULE_NAME)
  string(TOUPPER ${MODULE_NAME} NAME)

  getprop(SOURCE ${NAME}_SOURCE)
  getprop(INCLUDE ${NAME}_INCLUDE)
  getprop(DEPENDENCIES ${NAME}_DEPENDENCIES)

  message(Modulename: ${NAME})

  message(Includes:)
  foreach(VAR ${INCLUDE})
    message(${VAR})
  endforeach(VAR)  

  message(Sources:)
  foreach(VAR ${SOURCE})
    message(${VAR})
  endforeach(VAR)  

  message(Dependencies:)
  foreach(VAR ${DEPENDENCIES})
    message(${VAR})
  endforeach(VAR)  
endfunction(module_output_debugdata)
