cmake_minimum_required(VERSION 2.8)
cmake_policy(VERSION 2.8)


project(LearnOpenGL)

IF(NOT CAMKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

