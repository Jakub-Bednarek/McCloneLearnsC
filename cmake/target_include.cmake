function(target_include_engine_directory dir)
    target_include_directories(${ENGINE_LIBRARY_NAME} PUBLIC ${dir})
endfunction()