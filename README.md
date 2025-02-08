Полная документация: от установки до готового проекта
1. Установка MSYS2 и необходимых библиотек
a) Установка MSYS2
Скачайте MSYS2 с официального сайта: https://www.msys2.org/.

Установите MSYS2, следуя инструкциям на сайте.

b) Обновление пакетов MSYS2
Откройте терминал MSYS2 и выполните:

bash
Copy
pacman -Syu
Если система предложит перезапустить терминал, закройте его и откройте снова.

c) Установка необходимых библиотек
Установите SDL2 и её компоненты:

bash
Copy
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_ttf mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_mixer
2. Создание структуры проекта
Создайте следующую структуру проекта:

Copy
C:\PRJ\test1
├── CMakeLists.txt
├── main.cpp
├── assets
│   ├── font.ttf       # Пример шрифта
│   ├── image.png      # Пример изображения
│   └── sound.wav      # Пример звука
└── build
3. Содержимое CMakeLists.txt
Создайте файл CMakeLists.txt со следующим содержимым:

cmake
Copy
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
4. Содержимое main.cpp
Создайте файл main.cpp со следующим содержимым:

cpp
Copy
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>

// Класс для кнопки
class Button {
public:
    SDL_Rect rect;          // Позиция и размер кнопки
    SDL_Color color;        // Цвет кнопки
    std::string text;       // Текст на кнопке
    SDL_Color textColor;    // Цвет текста

    Button(int x, int y, int w, int h, SDL_Color btnColor, const std::string& btnText, SDL_Color txtColor)
        : rect{ x, y, w, h }, color(btnColor), text(btnText), textColor(txtColor) {}
};

// Класс для пользовательского интерфейса
class UI {
public:
    UI(SDL_Renderer* renderer, TTF_Font* font)
        : renderer(renderer), font(font) {}

    // Добавление кнопки
    void addButton(int x, int y, int w, int h, SDL_Color btnColor, const std::string& text, SDL_Color textColor) {
        buttons.emplace_back(x, y, w, h, btnColor, text, textColor);
    }

    // Отрисовка всех кнопок
    void render() {
        for (const auto& button : buttons) {
            // Отрисовка прямоугольника кнопки
            SDL_SetRenderDrawColor(renderer, button.color.r, button.color.g, button.color.b, button.color.a);
            SDL_RenderFillRect(renderer, &button.rect);

            // Отрисовка текста на кнопке
            if (font) {
                SDL_Surface* textSurface = TTF_RenderText_Solid(font, button.text.c_str(), button.textColor);
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_Rect textRect = {
                    button.rect.x + (button.rect.w - textSurface->w) / 2, // Центрирование текста
                    button.rect.y + (button.rect.h - textSurface->h) / 2,
                    textSurface->w,
                    textSurface->h
                };
                SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);
            }
        }
    }

private:
    SDL_Renderer* renderer; // Рендерер
    TTF_Font* font;         // Шрифт
    std::vector<Button> buttons; // Список кнопок
};

int main(int argc, char* argv[]) {
    // Инициализация SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Инициализация SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "TTF could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Инициализация SDL_image
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Инициализация SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! Mix_Error: " << Mix_GetError() << std::endl;
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Создание окна
    SDL_Window* window = SDL_CreateWindow(
        "UI Example",                  // Заголовок окна
        SDL_WINDOWPOS_CENTERED,       // Позиция окна по X
        SDL_WINDOWPOS_CENTERED,       // Позиция окна по Y
        800,                          // Ширина окна
        600,                          // Высота окна
        SDL_WINDOW_SHOWN              // Флаги окна
    );

    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        Mix_CloseAudio();
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Создание рендерера
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Загрузка шрифта
    TTF_Font* font = TTF_OpenFont("assets/font.ttf", 24);
    if (font == nullptr) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Создание пользовательского интерфейса
    UI ui(renderer, font);

    // Добавление кнопок
    ui.addButton(100, 100, 200, 50, { 255, 0, 0, 255 }, "Button 1", { 255, 255, 255, 255 }); // Красная кнопка
    ui.addButton(100, 200, 200, 50, { 0, 255, 0, 255 }, "Button 2", { 0, 0, 0, 255 });       // Зелёная кнопка
    ui.addButton(100, 300, 200, 50, { 0, 0, 255, 255 }, "Button 3", { 255, 255, 255, 255 }); // Синяя кнопка

    // Основной цикл
    bool running = true;
    SDL_Event event;
    while (running) {
        // Обработка событий
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Очистка экрана
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Белый цвет
        SDL_RenderClear(renderer);

        // Отрисовка интерфейса
        ui.render();

        // Обновление экрана
        SDL_RenderPresent(renderer);
    }

    // Очистка
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}
5. Сборка и запуск проекта
Перейдите в папку build:

bash
Copy
cd C:\PRJ\test1\build
Запустите CMake:

bash
Copy
cmake -G "Ninja" ..
Запустите сборку:

bash
Copy
cmake --build .
Запустите программу:

bash
Copy
.\a.exe
6. Что должно получиться
Окно с белым фоном.

Три кнопки: красная, зелёная и синяя, с текстом "Button 1", "Button 2" и "Button 3".

7. Если что-то не работает
Убедитесь, что файлы font.ttf, image.png и sound.wav находятся в папке assets.

Проверьте, что пути к файлам указаны правильно.

Если программа завершается с ошибкой, проверьте вывод в терминале и убедитесь, что все зависимости доступны.
