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
        main.cpp SPConfig.c SPKDArray.c main_aux.cpp)
#set(TESTER_FILES unit_tests/sp_config_unit_test.c unit_tests/sp_kd_array_unit_test.c unit_tests/sp_main_aux_unit_test.c
#        unit_tests/sp_logger_unit_test.c )
#set(ALL_FILES $(SOURCE_FILES) $(TESTER_FILES))
set(SPKDARRAY_TESTER_SOURCE_FILES SPLogger.c SPBPriorityQueue.c SPKDArray.c unit_tests/unit_test_util.h unit_tests/sp_kd_array_unit_test.c)
add_executable(SoftwareProject ${SOURCE_FILES})
add_executable(KDArrayTester ${SPKDARRAY_TESTER_SOURCE_FILES})

target_link_libraries( SoftwareProject ${OpenCV_LIBS} )
target_link_libraries( KDArrayTester ${OpenCV_LIBS} )