
function(getprop VAR_NAME PROP_NAME)
  get_property(TEMP GLOBAL PROPERTY ${PROP_NAME})
  set(${VAR_NAME} ${TEMP} PARENT_SCOPE)
endfunction(getprop)

function(setprop PROP_NAME PROP_VALUE)
  set_property(GLOBAL PROPERTY ${PROP_NAME} ${PROP_VALUE})
endfunction(setprop)

function(appendprop PROP_NAME PROP_VALUE)
  get_property(TEMP GLOBAL PROPERTY ${PROP_NAME})
  set_property(GLOBAL PROPERTY ${PROP_NAME} ${TEMP} ${PROP_VALUE})
endfunction(appendprop)