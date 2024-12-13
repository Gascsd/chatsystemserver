# 1. 添加cmake版本说明
cmake_minimum_required(VERSION 3.1.3)
# 2. 声明功能名称
project(odb-test)
# 3. 检测并生成odb框架代码
# 3.1 添加所需的odb映射代码名称
set(odb_path ${CMAKE_CURRENT_SOURCE_DIR}/entity)
set(odb_files student.hxx)
# 3.2 检测框架代码是否已经生成
set(odb_hxx "")
set(odb_cxx "")
set(odb_srcs "")
foreach(odb_file ${odb_files})
# 3.3 如果没有生成，则预定义生成指令 -- 用于在构建项目之前先生成框架代码
    string(REPLACE ".hxx" "-odb.hxx" odb_hxx ${odb_file})
    string(REPLACE ".hxx" "-odb.cxx" odb_cxx ${odb_file})
    if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/${odb_cxx})
        add_custom_command(
            PRE_BUILD
            COMMAND odb
            ARGS -d mysql --std c++11 --generate-query --generate-schema --profile boost/date-time ${odb_path}/${odb_file}
            DEPENDS ${odb_path}/${odb_file}
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${odb_cxx}
            COMMENT "生成ODB框架代码文件:" ${CMAKE_CURRENT_BINARY_DIR}/${odb_cxx}
        )
    endif()
# 3.4 将所有生成的框架代码文件名称保存起来
    message(STATUS "odb cxx file: " ${odb_cxx})
    list(APPEND odb_srcs ${CMAKE_CURRENT_BINARY_DIR}/${odb_cxx}) # stduent-odb.cxx classes-odb.cxx
endforeach(odb_file odb_files)
# 4. 获取源码目录下的所有源码文件
set(src_files "")
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/source src_files)
message(STATUS "odb src file: " ${odb_srcs})
# 5. 声明目标及依赖
add_executable(main ${src_files} ${odb_srcs})
# 6. 设置头文件默认搜索路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/entity)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
# 7. 设置需要链接的库
target_link_libraries(main -lgflags -lodb-mysql -lodb -lodb-boost)