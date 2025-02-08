#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>

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
        "SDL2 Test",                  // Заголовок окна
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

    // Загрузка изображения
    SDL_Surface* imageSurface = IMG_Load("assets/image.png");
    if (imageSurface == nullptr) {
        std::cerr << "Failed to load image! IMG_Error: " << IMG_GetError() << std::endl;
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }
    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);
    SDL_FreeSurface(imageSurface);

    // Загрузка звука
    Mix_Chunk* sound = Mix_LoadWAV("assets/sound.wav");
    if (sound == nullptr) {
        std::cerr << "Failed to load sound! Mix_Error: " << Mix_GetError() << std::endl;
        SDL_DestroyTexture(imageTexture);
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Основной цикл
    bool running = true;
    SDL_Event event;
    while (running) {
        // Обработка событий
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                // Воспроизведение звука при нажатии клавиши
                Mix_PlayChannel(-1, sound, 0);
            }
        }

        // Очистка экрана
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Чёрный цвет
        SDL_RenderClear(renderer);

        // Отрисовка изображения
        SDL_RenderCopy(renderer, imageTexture, nullptr, nullptr);

        // Отрисовка текста
        SDL_Color textColor = {255, 255, 255, 255}; // Белый цвет
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Hello, SDL2!", textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_Rect textRect = {10, 10, textSurface->w, textSurface->h};
        SDL_FreeSurface(textSurface);
        SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
        SDL_DestroyTexture(textTexture);

        // Обновление экрана
        SDL_RenderPresent(renderer);
    }

    // Очистка
    Mix_FreeChunk(sound);
    SDL_DestroyTexture(imageTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}