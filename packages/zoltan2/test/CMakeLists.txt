# For Zoltan2_TestHelpers.hpp
TRIBITS_INCLUDE_DIRECTORIES(REQUIRED_DURING_INSTALLATION_TESTING ${CMAKE_CURRENT_SOURCE_DIR}/core/helpers)

ADD_SUBDIRECTORIES(core)

IF(Trilinos_ENABLE_Zoltan2Sphynx AND Zoltan2_ENABLE_TESTS)
 ADD_SUBDIRECTORIES(sphynx)
ENDIF()
