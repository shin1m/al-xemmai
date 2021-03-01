add_library(al MODULE
	error.cc
	device.cc
	context.cc
	source.cc
	buffer.cc
	al.cc
	)
target_include_directories(al PRIVATE ${OPENAL_INCLUDE_DIR})
target_compile_options(al PRIVATE ${ALUT_CFLAGS})
target_link_libraries(al xemmai ${OPENAL_LIBRARY} ${ALUT_LIBRARIES})
set_property(TARGET al PROPERTY PREFIX "")
