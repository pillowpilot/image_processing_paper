cmake_minimum_required(VERSION 2.8)
project(code_paper)

set(CMAKE_BUILD_TYPE relwithdebinfo)

set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Including BOOST
find_package(Boost COMPONENTS filesystem system REQUIRED)
IF(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIR})
endif()

add_library(RNG SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/random_number_generator)

add_library(Optimizable SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/optimizable)

add_library(Rectangulation SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/rectangulation)
target_link_libraries(Rectangulation Optimizable)

add_library(CircleRectangulation SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/circle_rectangulation)
target_link_libraries(CircleRectangulation ${OpenCV_LIBS} Rectangulation RNG)

add_library(Transformation SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/transformation)

add_library(SimpleTransformation SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/simple_transformation)
target_link_libraries(SimpleTransformation ${OpenCV_LIBS} Transformation)

add_library(AHETransformation SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/ahe_transformation)
target_link_libraries(AHETransformation ${OpenCV_LIBS} Transformation)

add_library(CLAHETransformation SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/clahe_transformation)
target_link_libraries(CLAHETransformation ${OpenCV_LIBS} AHETransformation)

add_library(InterpolationScheme SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation_scheme)

add_library(SimpleInterpolator SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/simple_interpolator)
target_link_libraries(SimpleInterpolator ${OpenCV_LIBS} InterpolationScheme)

add_library(CircleInterpolation SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/circle_interpolation)
target_link_libraries(CircleInterpolation ${OpenCV_LIBS} InterpolationScheme Transformation)

add_library(Metric SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/metric)

add_library(MeanSquaredError SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/mean_squared_error)
target_link_libraries(MeanSquaredError ${OpenCV_LIBS} Metric)

add_library(NormalizedMeanSquaredError SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/normalized_mean_squared_error)
target_link_libraries(NormalizedMeanSquaredError ${OpenCV_LIBS} MeanSquaredError)

add_library(HistogramSpread SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/histogram_spread)
target_link_libraries(HistogramSpread ${OpenCV_LIBS} Metric)

add_library(ImageStreamer SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/image_streamer)
target_link_libraries(ImageStreamer ${Boost_LIBRARIES})

add_executable(Main ${CMAKE_CURRENT_SOURCE_DIR}/src/main)
set_property(TARGET Main PROPERTY CXX_STANDARD 11)
set_property(TARGET Main PROPERTY CXX_STANDARD_REQUIRED ON)

add_executable(RadioExperiment ${CMAKE_CURRENT_SOURCE_DIR}/src/radio_experiment)
set_property(TARGET Main PROPERTY CXX_STANDARD 11)
set_property(TARGET Main PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(Main
				${OpenCV_LIBS}
				${Boost_LIBRARIES}
				CircleRectangulation
				SimpleTransformation
				AHETransformation
				CLAHETransformation
				SimpleInterpolator
				CircleInterpolation
				MeanSquaredError
				)

target_link_libraries(RadioExperiment
					${OpenCV_LIBS}
					${Boost_LIBRARIES}
					ImageStreamer
					CircleRectangulation
					CLAHETransformation
					AHETransformation
					CircleInterpolation
					NormalizedMeanSquaredError
					)
