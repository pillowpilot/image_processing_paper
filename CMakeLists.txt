cmake_minimum_required(VERSION 2.8)
project(code_paper)

#set(CMAKE_BUILD_TYPE relwithdebinfo)
set(CMAKE_BUILD_TYPE debug)

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

add_library(RNG  ${CMAKE_CURRENT_SOURCE_DIR}/src/random_number_generator)
set_property(TARGET RNG PROPERTY CXX_STANDARD 11)

add_library(Optimizable  ${CMAKE_CURRENT_SOURCE_DIR}/src/optimizable)

add_library(Rectangulation  ${CMAKE_CURRENT_SOURCE_DIR}/src/rectangulation)
target_link_libraries(Rectangulation ${OpenCV_LIBS} Optimizable)

add_library(CircleRectangulation  ${CMAKE_CURRENT_SOURCE_DIR}/src/circle_rectangulation)
target_link_libraries(CircleRectangulation ${OpenCV_LIBS} Rectangulation RNG)
set_property(TARGET CircleRectangulation PROPERTY CXX_STANDARD 11)

add_library(TreeRectangulation  ${CMAKE_CURRENT_SOURCE_DIR}/src/tree_rectangulation)
target_link_libraries(TreeRectangulation ${OpenCV_LIBS} Rectangulation RNG)
set_property(TARGET TreeRectangulation PROPERTY CXX_STANDARD 11)

add_library(TreeRectangulationZero  ${CMAKE_CURRENT_SOURCE_DIR}/src/tree_rectangulation_zero)
target_link_libraries(TreeRectangulationZero ${OpenCV_LIBS} Rectangulation RNG)
set_property(TARGET TreeRectangulationZero PROPERTY CXX_STANDARD 11)

add_library(Transformation  ${CMAKE_CURRENT_SOURCE_DIR}/src/transformation)

add_library(SimpleTransformation  ${CMAKE_CURRENT_SOURCE_DIR}/src/simple_transformation)
target_link_libraries(SimpleTransformation ${OpenCV_LIBS} Transformation)
set_property(TARGET SimpleTransformation PROPERTY CXX_STANDARD 11)

add_library(CLAHETransformation  ${CMAKE_CURRENT_SOURCE_DIR}/src/clahe_transformation)
target_link_libraries(CLAHETransformation ${OpenCV_LIBS} Transformation Rectangulation)
set_property(TARGET CLAHETransformation PROPERTY CXX_STANDARD 11)

add_library(AHETransformation  ${CMAKE_CURRENT_SOURCE_DIR}/src/ahe_transformation)
target_link_libraries(AHETransformation ${OpenCV_LIBS} CLAHETransformation)
set_property(TARGET AHETransformation PROPERTY CXX_STANDARD 11)

add_library(InterpolationScheme  ${CMAKE_CURRENT_SOURCE_DIR}/src/interpolation_scheme)
target_link_libraries(InterpolationScheme ${OpenCV_LIBS})

add_library(SimpleInterpolator  ${CMAKE_CURRENT_SOURCE_DIR}/src/simple_interpolator)
target_link_libraries(SimpleInterpolator ${OpenCV_LIBS} InterpolationScheme Transformation)
set_property(TARGET SimpleInterpolator PROPERTY CXX_STANDARD 11)

add_library(CircleInterpolation  ${CMAKE_CURRENT_SOURCE_DIR}/src/circle_interpolation)
target_link_libraries(CircleInterpolation ${OpenCV_LIBS} InterpolationScheme Transformation)
set_property(TARGET CircleInterpolation PROPERTY CXX_STANDARD 11)

add_library(Metric  ${CMAKE_CURRENT_SOURCE_DIR}/src/metric)
target_link_libraries(Metric ${OpenCV_LIBS})

add_library(MeanSquaredError  ${CMAKE_CURRENT_SOURCE_DIR}/src/mean_squared_error)
target_link_libraries(MeanSquaredError ${OpenCV_LIBS} Metric)

add_library(NormalizedMeanSquaredError  ${CMAKE_CURRENT_SOURCE_DIR}/src/normalized_mean_squared_error)
target_link_libraries(NormalizedMeanSquaredError ${OpenCV_LIBS} MeanSquaredError)

add_library(HistogramSpread  ${CMAKE_CURRENT_SOURCE_DIR}/src/histogram_spread)
target_link_libraries(HistogramSpread ${OpenCV_LIBS} Metric)
set_property(TARGET HistogramSpread PROPERTY CXX_STANDARD 11)

add_library(ContrastImprovementRatio  ${CMAKE_CURRENT_SOURCE_DIR}/src/contrast_improvement_ratio)
target_link_libraries(ContrastImprovementRatio ${OpenCV_LIBS} Metric)
set_property(TARGET ContrastImprovementRatio PROPERTY CXX_STANDARD 11)

add_library(Optimizator  ${CMAKE_CURRENT_SOURCE_DIR}/src/optimizator)
target_link_libraries(Optimizator ${OpenCV_LIBS} Rectangulation InterpolationScheme Metric Optimizable)
set_property(TARGET Optimizator PROPERTY CXX_STANDARD 11)

add_library(ImageStreamer  ${CMAKE_CURRENT_SOURCE_DIR}/src/image_streamer)
target_link_libraries(ImageStreamer ${Boost_LIBRARIES})
set_property(TARGET ImageStreamer PROPERTY CXX_STANDARD 11)

add_executable(Main ${CMAKE_CURRENT_SOURCE_DIR}/src/main)
set_property(TARGET Main PROPERTY CXX_STANDARD 11)
set_property(TARGET Main PROPERTY CXX_STANDARD_REQUIRED ON)

add_executable(RadioExperiment ${CMAKE_CURRENT_SOURCE_DIR}/src/radio_experiment)
set_property(TARGET RadioExperiment PROPERTY CXX_STANDARD 11)
set_property(TARGET RadioExperiment PROPERTY CXX_STANDARD_REQUIRED ON)

add_executable(OptimizationExperiment ${CMAKE_CURRENT_SOURCE_DIR}/src/optimization_experiment)
set_property(TARGET OptimizationExperiment PROPERTY CXX_STANDARD 11)
set_property(TARGET OptimizationExperiment PROPERTY CXX_STANDARD_REQUIRED ON)

add_executable(TRTest ${CMAKE_CURRENT_SOURCE_DIR}/src/tr_test)
set_property(TARGET TRTest PROPERTY CXX_STANDARD 11)
set_property(TARGET TRTest PROPERTY CXX_STANDARD_REQUIRED ON)

add_executable(TRZTest ${CMAKE_CURRENT_SOURCE_DIR}/src/trz_test)
set_property(TARGET TRZTest PROPERTY CXX_STANDARD 11)
set_property(TARGET TRZTest PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(Main
				${OpenCV_LIBS}
				${Boost_LIBRARIES}
				CircleRectangulation
				TreeRectangulation
				SimpleTransformation
				AHETransformation
				CLAHETransformation
				SimpleInterpolator
				CircleInterpolation
				MeanSquaredError
				HistogramSpread
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

target_link_libraries(OptimizationExperiment
				${OpenCV_LIBS}
				${Boost_LIBRARIES}
				CircleRectangulation
				TreeRectangulation
				AHETransformation
				CLAHETransformation
				CircleInterpolation
				HistogramSpread
				ContrastImprovementRatio
				Optimizator
				)

target_link_libraries(TRTest
				${OpenCV_LIBS}
				${Boost_LIBRARIES}
				CircleRectangulation
				TreeRectangulation
				SimpleTransformation
				AHETransformation
				CLAHETransformation
				SimpleInterpolator
				CircleInterpolation
				MeanSquaredError
				HistogramSpread
				)

target_link_libraries(TRZTest
				${Boost_LIBRARIES}
				${OpenCV_LIBS}
				TreeRectangulationZero
				CLAHETransformation
				CircleInterpolation
				ContrastImprovementRatio
				ImageStreamer
				)
