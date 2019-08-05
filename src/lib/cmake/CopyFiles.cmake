function(copy_sdk_header_files _OUTPUT_DIRECTORY _INPUT_BASE_DIR)

    set(_HEADERS ${ARGN})   
	
	# list(LENGTH ${ARGN} ARGC)
	# file( GLOB_RECURSE pattern_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/" "patterns/*.vti" )

	message("${BoldGreen}[benni] copy_sdk_header_files( _OUTPUT_DIRECTORY = ${_OUTPUT_DIRECTORY}, _HEADERS = ${ARGC} )${ColourReset}")
	
	foreach(_HEADER ${_HEADERS})
		set(_FROM "${_HEADER}")
		string(REPLACE "${_INPUT_BASE_DIR}/" "" _TO "${_HEADER}") # Remove base dir and build new dir
		set(_TO "${_OUTPUT_DIRECTORY}/${_TO}")
		add_custom_command(	OUTPUT ${_TO} COMMAND cmake -E copy ${_FROM} ${_TO}	DEPENDS ${_FROM})
		### list( APPEND pattern_files_dest "${_HEADER}" )
		message("${Yellow}[benni] ${BoldBlue}_FROM = ${_FROM} )${ColourReset}")	
		message("${Yellow}[benni] ${BoldYellow}_TO = ${_TO}${ColourReset}")
endforeach(_HEADER )

endfunction(copy_sdk_header_files)

function(my_add_elf _target)

    set(_source_list ${ARGN})
    add_executable(${_target}_in ${_source_list})

    set_target_properties(
        ${_target}_in
        PROPERTIES
            POSITION_INDEPENDENT_CODE   0
            SUFFIX                      .elf
    )

    add_custom_command(
        OUTPUT ${_target}_step1.elf
        COMMAND some_conversion_cmd $<TARGET_FILE:${_target}_in> > ${_target}_step1.elf
        DEPENDS ${_target}_in
    )

    add_custom_target(
        ${_target}_step1 
        DEPENDS 
            ${_target}_step1.elf
    )

    add_custom_command(
        OUTPUT ${_target}_out.elf
        COMMAND final_post_process_cmd ${_target}_step1.elf > ${_target}_out.elf
        DEPENDS ${_target}_step1
    )

    add_custom_target(
        ${_target}_out 
        DEPENDS 
            ${_target}_out.elf
    )

    # alias / PHONY target
    add_custom_target(${_target} DEPENDS ${_target}_out)

	message("${BoldGreen}[benni] _target ${_target}${ColourReset}")
endfunction(my_add_elf)

