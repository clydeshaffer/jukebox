# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\JukeboxCreator_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\JukeboxCreator_autogen.dir\\ParseCache.txt"
  "JukeboxCreator_autogen"
  )
endif()
