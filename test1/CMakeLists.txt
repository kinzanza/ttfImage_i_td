cmake_minimum_required(VERSION 3.10)
project(a)

# Ищем SDL2 и его компоненты
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

# Добавляем исполняемый файл
add_executable(a main.cpp)

# Подключаем заголовочные файлы
target_include_directories(a PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
)

# Подключаем библиотеки
target_link_libraries(a PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
)

# Для Windows также нужно подключить системные библиотеки
if (WIN32)
    target_link_libraries(a PRIVATE
        -lmingw32
        -lSDL2main
        -lSDL2
        -lSDL2_ttf
        -lSDL2_image
        -lSDL2_mixer
        -lopengl32
        -lwinmm
        -limm32
        -lversion
        -lsetupapi
        -lole32
        -loleaut32
        -lgdi32
        -luser32
        -lkernel32
    )
endif()