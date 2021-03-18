# Install script for directory: /home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/xtensor" TYPE FILE FILES
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xaccessible.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xaccumulator.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xadapt.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xarray.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xassign.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xaxis_iterator.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xaxis_slice_iterator.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xbroadcast.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xbuffer_adaptor.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xbuilder.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xchunked_array.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xcomplex.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xcontainer.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xcsv.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xdynamic_view.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xeval.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xexception.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xexpression.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xexpression_holder.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xexpression_traits.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xfixed.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xfunction.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xfunctor_view.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xgenerator.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xhistogram.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xindex_view.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xinfo.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xio.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xiterable.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xiterator.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xjson.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xlayout.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xmanipulation.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xmasked_view.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xmath.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xmime.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xnoalias.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xnorm.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xnpy.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xoffset_view.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xoperation.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xoptional.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xoptional_assembly.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xoptional_assembly_base.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xoptional_assembly_storage.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xpad.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xrandom.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xreducer.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xrepeat.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xscalar.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xsemantic.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xset_operation.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xshape.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xslice.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xsort.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xstorage.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xstrided_view.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xstrided_view_base.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xstrides.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xtensor.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xtensor_config.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xtensor_forward.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xtensor_simd.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xutils.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xvectorize.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xview.hpp"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/include/xtensor/xview_utils.hpp"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/xtensor" TYPE FILE FILES
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/xtensorConfig.cmake"
    "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/xtensorConfigVersion.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/xtensor/xtensorTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/xtensor/xtensorTargets.cmake"
         "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/CMakeFiles/Export/lib/cmake/xtensor/xtensorTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/xtensor/xtensorTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/xtensor/xtensorTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/xtensor" TYPE FILE FILES "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/CMakeFiles/Export/lib/cmake/xtensor/xtensorTargets.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/xtensor.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/xtensor.hpp")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/test/Dropbox/Work/Stanford/Stanford_Units_Y2_T2/CS248/Project/Code/deps/xtensor/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
