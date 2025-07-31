cmake_minimum_required(VERSION 3.10)

project(avionics)

# C 표준 지정
set(CMAKE_C_STANDARD 99)

# 컴파일러 강제 지정 (gcc)
set(CMAKE_C_COMPILER gcc)

# 최적화 및 아키텍처 옵션 설정
add_compile_options(-O3 -march=native)

# GSLV1B_Common 디렉토리 내부 소스 파일 모으기
file(GLOB COMMON_SRC
    "${CMAKE_SOURCE_DIR}/src/hal/*.c"
    "${CMAKE_SOURCE_DIR}/src/driver/*.c"
    "${CMAKE_SOURCE_DIR}/src/common/*.c"
    "${CMAKE_SOURCE_DIR}/src/app/*.c"
)

# 메인 소스
set(MAIN_SRC src/main.c)

# 실행 파일 생성
add_executable(${PROJECT_NAME} ${MAIN_SRC} ${COMMON_SRC})

target_link_libraries(${PROJECT_NAME} m)

# include 디렉토리 추가
target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    "${CMAKE_SOURCE_DIR}/src/hal/"
    "${CMAKE_SOURCE_DIR}/src/driver/"
    "${CMAKE_SOURCE_DIR}/src/common/"
    "${CMAKE_SOURCE_DIR}/src/app/"
)