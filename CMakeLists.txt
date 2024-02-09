cmake_minimum_required(VERSION 3.27)
project(coursework)

set(CMAKE_CXX_STANDARD 17)

add_executable(coursework calculator.cpp
        pass_manager.cpp)

include_directories(/opt/homebrew/Cellar/openssl@3/3.2.1/include/)
target_link_libraries(openss_hmac /opt/homebrew/Cellar/openssl@1.1/1.1.1d/lib/libcrypto.a)