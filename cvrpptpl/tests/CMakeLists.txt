
file(GLOB TEST_SOURCES *.cpp)

add_executable(cvrpptpltest ${TEST_SOURCES})
target_link_libraries(cvrpptpltest PUBLIC
	GTest::gtest_main
	cvrpptpl_lib
)

add_test(
	NAME cvrpptpltest
	COMMAND cvrpptpltest
)