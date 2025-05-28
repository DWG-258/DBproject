cmake_minimum_required(VERSION 3.10)
project(dblite)

# 设置 C++20 标准并强制启用
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # <-- 推荐添加，防止降级
set(CMAKE_CXX_EXTENSIONS OFF)        # <-- 可选：禁止 GNU 扩展（如 -std=gnu++20）

# 设置 Debug 模式
set(CMAKE_BUILD_TYPE Debug)

# 头文件路径
include_directories(${PROJECT_SOURCE_DIR}/include)

# 收集源文件
file(GLOB SRC_FILES src/*.cpp)
# 设置可执行文件输出路径（例如：项目根目录下的 bin 目录）
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


# 可执行文件
add_executable(dblite ${SRC_FILES})

# 编译选项：开启调试符号和警告信息
target_compile_options(dblite PRIVATE -g -Wall)
target_include_directories(dblite PRIVATE ${GLPK_INCLUDE_DIR})
target_link_libraries(dblite PRIVATE ${GLPK_LIBRARY})
