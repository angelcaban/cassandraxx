
add_executable(clienttest clientTest.cpp)
target_link_libraries(clienttest CassandraCxx gtest gtest_main pthread)
add_test(NAME client_test COMMAND clienttest)

file(COPY ${libraries} DESTINATION ${PROJECT_BINARY_DIR}/test)