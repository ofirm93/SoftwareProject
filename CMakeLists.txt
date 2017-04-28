cmake_minimum_required(VERSION 3.6)
project(SoftwareProject)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-build-debug)

find_package( OpenCV REQUIRED )
#target_link_libraries( DisplayImage ${OpenCV_LIBS} )

#find_package(OpenCV REQUIRED)

#if( OpenCV_FOUND )
#    message( "Found OpenCV" )
#    message( "Includes: " ${OpenCV_INCLUDE_DIRS} )
#else()
#    message( "OpenCV not found, woot" )
#endif( OpenCV_FOUND )

set(SOURCE_FILES SPPoint.c SPLogger.c SPKDTree.c SPImageProc.cpp SPBPriorityQueue.c KNearestSearch.c ExtModeForMain.cpp
        main.cpp SPConfig.c SPKDArray.c SPKDArray.h main_aux.cpp main_aux.h  ExtModeForMain.cpp unit_tests/sp_kdtree_unit_test.c
        unit_tests/sp_config_unit_test.c unit_tests/sp_kd_array_unit_test.c unit_tests/sp_main_aux_unit_test.c
        unit_tests/sp_logger_unit_test.c )
#set(TESTER_FILES unit_tests/sp_config_unit_test.c unit_tests/sp_kd_array_unit_test.c unit_tests/sp_main_aux_unit_test.c
#        unit_tests/sp_logger_unit_test.c )
#set(ALL_FILES $(SOURCE_FILES) $(TESTER_FILES))
add_executable(SoftwareProject ${SOURCE_FILES})
target_link_libraries( SoftwareProject ${OpenCV_LIBS} )
