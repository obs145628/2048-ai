
add_custom_target(gen_tests
	COMMAND
	PYL_BUILD_DIR=${CMAKE_BINARY_DIR}
	PYL_ROOT_DIR=${CMAKE_SOURCE_DIR}
	python ${CMAKE_SOURCE_DIR}/tests/gen_tests.py
)

add_custom_target(check
	COMMAND
	PYTHONPATH=${PROJECT_SOURCE_DIR}
	LD_LIBRARY_PATH=${PROJECT_SOURCE_DIR}
	PYL_BUILD_DIR=${CMAKE_BINARY_DIR}
	PYL_ROOT_DIR=${CMAKE_SOURCE_DIR}
	python ${CMAKE_SOURCE_DIR}/tests/run-test-suite.py
	${CMAKE_BINARY_DIR}/tests.json
	DEPENDS 2048-ai gen_tests env2048
)
