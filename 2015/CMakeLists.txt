cmake_minimum_required(VERSION 3.18)

project("aoc-2020")

find_package(boost CONFIG)
find_package(OpenSSL)
#
# Function for each individual project
#
function(aoc_target target_name)

  file(GLOB src_cpp ${target_name}/*.cpp)
  file(GLOB src_h   ${target_name}/*.h)
  file(GLOB_RECURSE inc_h  ../include/*.h ../include/*.ipp)

  message( "trying to add: ${target_name}" )

  add_executable(${target_name}  ${src_cpp} ${src_h} ${common_h})
  set_property(TARGET ${target_name} PROPERTY CXX_STANDARD 20)

  target_include_directories( ${target_name} PRIVATE
    ${target_name}
    ../include
    )

  if(${target_name} STREQUAL "04")
    target_link_libraries(${target_name} OpenSSL::Crypto ) # md5sum
  endif()

  file(GLOB src_input ${target_name}/input.txt)
  add_custom_target( test${target_name}
    COMMAND ${target_name} < ${src_input}
    )

endfunction()

# go ahead and search for 64 bit libs
set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS ON)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wshadow -pedantic" )


# list the directories, select out ONLY the directories matching [0-9]{3}
file(GLOB all_files ${CMAKE_CURRENT_LIST_DIR}/[0-2][0-9])
foreach(i ${all_files} )
  if(IS_DIRECTORY ${i})
    get_filename_component( filename ${i} NAME)
    aoc_target(${filename})
  endif()
endforeach()
