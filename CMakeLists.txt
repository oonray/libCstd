cmake_minimum_required(VERSION 3.10)
 
project(libCstd VERSION 1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
enable_testing()

set(CPACK_PACKAGE_VENDOR "oonray")
set(CPACK_PACKAGE_CONTACT "alexanderbjornsrud@gmail.com")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "oonray")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
set(CPACK_DEB_COMPONENT_INSTALL YES)
include(CPack)

set(CPACK_OUTPUT_FILE_PREFIX "${CMAKE_SOURCE_DIR}/_packages")
set(CPACK_PACKAGE_NAME ${PROJECT_NAME} CACHE STRING "libCstd")
set(CPACK_VERBATIM_VARIABLES YES)
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})


#HFILES
set(DEST "include")

FILE(GLOB lib lib/*)
FILE(GLOB src src/*)
FILE(GLOB src_util src/util/*)

FILE(GLOB src_h ${src}/*.h)
FILE(GLOB lib_h ${lib}/*.h)
FILE(GLOB util_h ${inc_src_util}/*.h)

install(FILES ${src_h} DESTINATION ${DEST})
install(FILES ${lib_h} DESTINATION ${DEST})
install(FILES ${util_h} DESTINATION ${DEST}/munit)

include_directories(${lib})
include_directories(${src})
include_directories(${util})

#munit
FILE(GLOB munit_s lib/munit/*.c)
add_library(munit_o OBJECT ${munit_s})
add_library(munit $<TARGET_OBJECTS:munit_o>)

if (DEFINED NO_CA_ERRORS)
message("Using C errors")
else()
message("Using CA_ERRORS")
add_compile_definitions(CA_ERRORS=1)
endif()

#bstrlib
#@author websnarf
#@git https://github.com/websnarf/bstrlib
FILE(GLOB bstr_s lib/bstring/*.c)
add_library(bstrlib_o OBJECT ${bstr_s})
add_library(bstrlib $<TARGET_OBJECTS:bstrlib_o>)

#error
add_library(error_obj OBJECT src/error/ca_error.c)
add_library(ca_error $<TARGET_OBJECTS:error_obj>)
install(TARGETS ca_error)

add_executable(error_tests tests/error_tests.c)
target_link_libraries(error_tests PRIVATE ca_error munit bstrlib)
add_test(NAME error_test
         COMMAND error_tests)

# Vector
add_library(vector_obj OBJECT src/vector/ca_vector.c)
add_library(ca_vector $<TARGET_OBJECTS:vector_obj>)
install(TARGETS ca_vector)

add_executable(vector_tests tests/vector_tests.c)
target_link_libraries(vector_tests PRIVATE ca_vector munit)
add_test(NAME vector_test
         COMMAND vector_tests)

# Map
add_library(map_obj OBJECT src/map/ca_map.c)
add_library(ca_map $<TARGET_OBJECTS:map_obj>)
target_link_libraries(ca_map PRIVATE ca_vector)
add_dependencies(ca_map ca_vector)
install(TARGETS ca_map)

add_executable(map_tests tests/map_tests.c)
target_link_libraries(map_tests PRIVATE ca_map ca_vector bstrlib munit)
add_test(NAME map_test
         COMMAND map_tests)

# Stack
add_library(stack_obj OBJECT src/stack/ca_stack.c)
add_library(ca_stack $<TARGET_OBJECTS:stack_obj>)
target_link_libraries(ca_stack PRIVATE ca_vector)
install(TARGETS ca_stack)
add_dependencies(ca_stack ca_vector)

add_executable(stack_tests tests/stack_tests.c)
target_link_libraries(stack_tests PRIVATE ca_stack ca_vector munit)
add_test(NAME stack_test
         COMMAND stack_tests)

#queue
add_library(queue_obj OBJECT src/queue/ca_queue.c)
add_library(ca_queue $<TARGET_OBJECTS:queue_obj>)
target_link_libraries(ca_queue PRIVATE ca_vector)
install(TARGETS ca_queue)
add_dependencies(ca_queue ca_vector)

add_executable(queue_tests tests/queue_tests.c)
target_link_libraries(queue_tests PRIVATE ca_queue munit)
add_test(NAME queue_test
         COMMAND queue_tests)

#buffer
add_library(buffer_obj OBJECT src/buffer/ca_buffer.c)
add_library(ca_buffer $<TARGET_OBJECTS:buffer_obj>)
target_link_libraries(ca_buffer PRIVATE ca_error)
install(TARGETS ca_buffer)


add_library(caa_buffer_obj OBJECT src/buffer/ca_buffer.cc)
add_library(caa_buffer $<TARGET_OBJECTS:caa_buffer_obj>)
target_link_libraries(caa_buffer PRIVATE ca_error)
install(TARGETS caa_buffer)

add_executable(buffer_tests tests/buffer_tests.c)
target_link_libraries(buffer_tests PRIVATE  ca_buffer ca_error munit bstrlib )
add_test(NAME buffer_test
         COMMAND buffer_tests)

#ringbuffer
add_library(ringbuffer_obj OBJECT src/ringbuffer/ca_ringbuffer.c)
add_library(ca_ringbuffer $<TARGET_OBJECTS:ringbuffer_obj>)
target_link_libraries(ca_ringbuffer PRIVATE ca_buffer ca_error)
install(TARGETS ca_ringbuffer)
add_dependencies(ca_ringbuffer ca_vector bstrlib)

add_executable(ringbuffer_tests tests/ringbuffer_tests.c)
target_link_libraries(ringbuffer_tests PRIVATE ca_buffer ca_error ca_ringbuffer munit bstrlib)
add_test(NAME ringbuffer_test
         COMMAND ringbuffer_tests)

#titree
add_library(tritree_obj OBJECT src/tritree/ca_tritree.c)
add_library(ca_tritree $<TARGET_OBJECTS:tritree_obj> )
target_link_libraries(ca_tritree PRIVATE ca_vector bstrlib)
install(TARGETS ca_tritree)
add_dependencies(ca_tritree ca_vector)

add_executable(tritree_tests tests/tritree_tests.c)
target_link_libraries(tritree_tests ca_tritree munit)
add_test(NAME tritree_test
         COMMAND tritree_tests)

#bintree
add_library(bintree_obj OBJECT src/bintree/ca_bintree.c)
add_library(ca_bintree $<TARGET_OBJECTS:bintree_obj>)
target_link_libraries(ca_bintree PRIVATE bstrlib)
install(TARGETS ca_bintree)

add_executable(bintree_tests tests/bintree_tests.c)
target_link_libraries(bintree_tests PRIVATE ca_bintree munit)
add_test(NAME bintree_test
         COMMAND bintree_tests)

#TODO
#fileio
#add_library(fileio_obj OBJECT src/io/ca_fileio.c)
#add_library(ca_fileio $<TARGET_OBJECTS:fileio_obj>)
#target_link_libraries(ca_fileio PRIVATE bstrlib ca_ringbuffer ca_vector)

#install(TARGETS ca_fileio)
#add_dependencies(ca_fileio ca_ringbuffer)

#add_executable(io_tests tests/fileio_test.c)
#target_link_libraries(io_tests PRIVATE ca_fileio bstrlib munit)
#add_test(NAME io_tests
#         COMMAND io_tests)

#io SSL
#if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
#   set(OSX TRUE)
#   set(OPENSSL_ROOT_DIR "/opt/homebrew/opt/openssl@1.1")
#endif()

#openssl
find_package(OpenSSL)
if(${OPENSSL_FOUND})
message("openssl: ${OPENSSL_FOUND}")
message("openssl-include: ${OPENSSL_INCLUDE_DIR}")

#add_library(fileio_ssl_obj OBJECT io/src/ca_fileio.c io/ca_fileio_ssl.c)
#target_include_directories(fileio_ssl_obj PRIVATE ${OPENSSL_INCLUDE_DIR})
#target_compile_definitions(fileio_ssl_obj PRIVATE _CA_WITH_OPEN_SSL)

#add_library(ca_fileio_ssl $<TARGET_OBJECTS:fileio_ssl_obj>)
#target_link_libraries(ca_fileio_ssl PRIVATE ca_ringbuffer bstrlib OpenSSL::SSL)

#install(TARGETS ca_fileio_ssl)
#add_dependencies(ca_fileio_ssl ca_ringbuffer)
endif()

##argparse
#use optargs
# man 3 optargs

#sockets
#add_library(soc_obj OBJECT src/sockets/soc.c)
#add_library(soc $<TARGET_OBJECTS:soc_obj>)
#target_link_libraries(soc PRIVATE fileio_ssl)
#install(TARGETS soc)
#add_dependencies(soc fileio)
#add_dependencies(soc ringbuffer)

#add_executable(soc_tests tests/socket_tests.c tests/munit.c)
#target_link_libraries(soc_tests PRIVATE soc fileio_ssl bstrlib)
#add_test(NAME soc_tests
#         COMMAND soc_tests)

#  #request
# use libcurl
