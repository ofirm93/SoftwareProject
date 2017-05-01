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
#set(MAIN_AUX_TESTER_SOURCE_FILES unit_tests/unit_test_util.h SPKDArray.c SPImageProc.cpp SPLogger.c SPConfig.c
#        KNearestSearch.c SPPoint.c SPKDTree.c unit_tests/sp_main_aux_unit_test.c main_aux.cpp SPBPriorityQueue.c)
#unit_tests/unit_test_util.h  SPLogger.c SPConfig.c main_aux.cpp  SPPoint.c SPKDTree.c
set(MAIN_AUX_TESTER_SOURCE_FILES unit_tests/sp_main_aux_unit_test.cpp
unit_tests/../main_aux.h unit_tests/../SPKDTree.h
unit_tests/../SPKDArray.h unit_tests/../SPPoint.h
unit_tests/../SPConfig.h unit_tests/../SPLogger.h
unit_tests/../SPBPriorityQueue.h unit_tests/unit_test_util.h
unit_tests/../SPLogger.h)

#        SPConfig.h SPLogger.h main_aux.h SPKDTree.h SPKDArray.h SPPoint.h SPConfig.h SPBPriorityQueue.h unit_tests/unit_test_util.h unit_tests/sp_main_aux_unit_test.cpp)
set(SPCONFIG_TESTER_SOURCE_FILES unit_tests/sp_config_unit_test.c
unit_tests/../SPConfig.c unit_tests/../SPLogger.c unit_tests/../SPPoint.c
unit_tests/unit_test_util.h )
add_executable(SoftwareProject ${SOURCE_FILES})
add_executable(SPConfigTester ${SPCONFIG_TESTER_SOURCE_FILES})

add_executable(KDArrayTester ${SPKDARRAY_TESTER_SOURCE_FILES})
add_executable(MainAuxTester ${MAIN_AUX_TESTER_SOURCE_FILES})

target_link_libraries( SoftwareProject ${OpenCV_LIBS} )
target_link_libraries( KDArrayTester ${OpenCV_LIBS} )
target_link_libraries( MainAuxTester ${OpenCV_LIBS} )
