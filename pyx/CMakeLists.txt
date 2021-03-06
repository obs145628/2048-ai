
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

set(SRC
  ../src/grid.cc
)

add_library(env2048native SHARED EXCLUDE_FROM_ALL ${SRC})

add_custom_target(env2048
  COMMAND python ${PROJECT_SOURCE_DIR}/setup.py build_ext --inplace && rm -rf ${PROJECT_SOURCE_DIR}/pyx/*.cpp ${PROJECT_SOURCE_DIR}/build 
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
add_dependencies(env2048 env2048native)

add_custom_target(env2048_check
   COMMAND PYTHONPATH=${PROJECT_SOURCE_DIR} LD_LIBRARY_PATH=${PROJECT_SOURCE_DIR} python ${PROJECT_SOURCE_DIR}/tests/cython_check.py gridtest world ${PROJECT_SOURCE_DIR}/tests/games/game8_moves
   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
add_dependencies(env2048_check env2048)

add_custom_target(ml_random_ai
  COMMAND
  PYTHONPATH=${PROJECT_SOURCE_DIR}:${PROJECT_SOURCE_DIR}/src_py
  LD_LIBRARY_PATH=${PROJECT_SOURCE_DIR}
  python ${PROJECT_SOURCE_DIR}/ml/sl-ai/random-ai.py
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
add_dependencies(ml_random_ai env2048)

add_custom_target(sl_online_ai
  COMMAND
  PYTHONPATH=${PROJECT_SOURCE_DIR}:${PROJECT_SOURCE_DIR}/src_py
  LD_LIBRARY_PATH=${PROJECT_SOURCE_DIR}
  python ${PROJECT_SOURCE_DIR}/ml/sl-ai/online-ai.py
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
add_dependencies(sl_online_ai env2048)

add_custom_target(sl_batch_ai
  COMMAND
  PYTHONPATH=${PROJECT_SOURCE_DIR}:${PROJECT_SOURCE_DIR}/src_py
  LD_LIBRARY_PATH=${PROJECT_SOURCE_DIR}
  python ${PROJECT_SOURCE_DIR}/ml/sl-ai/batch-ai.py
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
add_dependencies(sl_batch_ai env2048)

add_custom_target(ml_eval_model
  COMMAND
  PYTHONPATH=${PROJECT_SOURCE_DIR}:${PROJECT_SOURCE_DIR}/src_py
  LD_LIBRARY_PATH=${PROJECT_SOURCE_DIR}
  python ${PROJECT_SOURCE_DIR}/ml/sl-ai/eval-model.py
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)
add_dependencies(ml_eval_model env2048)
