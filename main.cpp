#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <array>
#include <cassert>
#include <vector>
#include "Particle.h"

constexpr int StreamingWidth = 64;
constexpr int StreamingHeight = 64;

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// PixelData will be a vector of a vector so the outer vector is the Y and the inner is the X position.



int main()
{

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG);


    auto window = SDL_CreateWindow("FishEye", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512,
            /*SDL_WINDOW_FULLSCREEN_DESKTOP*/SDL_WINDOW_FULLSCREEN_DESKTOP);
    auto renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

    const auto home = std::getenv("HOME");
    std::string filepath(home);
    // filepath += "/Pictures/20201111-assassins-creed-valhalla.png";
    filepath += "/Pictures/pulp-fiction.jpg";
    std::cout << "Filepath" << filepath << std::endl;
    const auto backgroundSurface = IMG_Load(filepath.c_str());
    const auto backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);


    // Use the format and understand each picture if you want to load PNG or JPG, we need to do bit manipulation for each type.
    auto *pixels = (Uint8 *) backgroundSurface->pixels;
    std::array<uint32_t, 64 * 64> framebuf = {0};

    SDL_Texture *streamingTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                                      StreamingWidth, StreamingHeight);


    SDL_Rect streamingRect = {.x = 32, .y = 32, .w = StreamingWidth, .h = StreamingHeight};
#pragma region "Lambdas"
    auto map_up_x = [backgroundSurface](int value)
    {
        return map(value, 0, 512, 0, backgroundSurface->w);
    };
    auto map_up_y = [backgroundSurface](int value)
    {
        return map(value, 0, 512, 0, backgroundSurface->h);
    };
    auto map_down_x = [backgroundSurface](int value)
    {
        return map(value, 0, backgroundSurface->w, 0, 512);
    };
    auto map_down_y = [backgroundSurface](int value)
    {
        return map(value, 0, backgroundSurface->h, 0, 512);
    };
    auto to_radian = [](int degree)
    {
        return degree * (M_PI / 180.0);
    };
    auto pixel = [pixels, backgroundSurface](long x, long y)
    {
        auto color = pixels[x + y * backgroundSurface->w];
        SDL_Color color_s;
        SDL_GetRGBA(color, backgroundSurface->format, &color_s.r, &color_s.g, &color_s.b, &color_s.a);
        // std::printf("Pixel format %s\n", SDL_GetPixelFormatName(backgroundSurface->format->format));
        if (backgroundSurface->format->format == SDL_PIXELFORMAT_RGB24)
        {
            return SDL_MapRGB(backgroundSurface->format, color_s.r, color_s.g, color_s.b);
        }
        return (Uint32) (color_s.a << 24 | color_s.r << 16 | color_s.g << 8 | color_s.b);
    };
    auto calculateRelativeBrightness = [](SDL_Color &color)
    {
        auto value = std::sqrt(
                (color.r * color.r) * 0.299 +
                (color.g * color.g) * 0.587 +
                (color.b * color.b) * 0.114
        );
        return value;
    };

#pragma endregion

    int dx = -1;
    int dy = 1;
    bool quit = false;
    SDL_Event event;
    SDL_SetTextureBlendMode(streamingTexture, SDL_BLENDMODE_BLEND);
    for (int y = 0; y < streamingRect.h; y++)
    {
        for (int x = 0; x < streamingRect.w; x++)
        {
            auto index = x + y * 64;
            framebuf[index] = 0x00ffffff;
        }
    }

    std::vector<Particle> particles;
    for (auto i = 0; i < 5000; i++)
    {
        particles.emplace_back(512, 512);
    }


    std::printf("Image dimension %d,%d\n", backgroundSurface->w, backgroundSurface->h);
    std::printf("Bytes per pixel %d\n", backgroundSurface->format->BytesPerPixel);
    std::printf("Bits  per pixel %d\n", backgroundSurface->format->BitsPerPixel);
    std::printf("Row size %d %d\n", backgroundSurface->pitch,
                backgroundSurface->pitch / backgroundSurface->format->BytesPerPixel);
    std::printf("Pixels in image %d\n", backgroundSurface->w * backgroundSurface->h);

    std::vector<std::vector<PixelData>> mappedPixels;
    for (auto y = 0; y < backgroundSurface->h; y++)
    {
        std::vector<PixelData> row;
        for (auto x = 0; x < backgroundSurface->w; x++)
        {
            SDL_Color color;
                auto index = (y * backgroundSurface->pitch) + (x * backgroundSurface->format->BytesPerPixel);
            auto red = pixels[index];
            auto green = pixels[index+1];
            auto blue = pixels[index+2];

            Uint32 dot = red << 24 | green << 16 | blue << 8 | 255;
            color.r = red;
            color.g = green;
            color.b = blue;
            PixelData pd = {
                    .brightness = calculateRelativeBrightness(color),
                    .color = color,
                    .dot = dot,
            };

            row.push_back(pd);
        }
        mappedPixels.push_back(row);
    }

    std::printf("Mapped pixels %zu,%zu\n", mappedPixels.size(), mappedPixels[0].size());
    std::printf("mapped pixel brightness %f\n", mappedPixels[0][0].brightness);


    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
                break;
            }
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                quit = true;
                break;
            }
        }


        SDL_SetRenderDrawColor(renderer, 127, 127, 127, 255);
        SDL_RenderClear(renderer);

     //   SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
        for (auto y = 0; y < mappedPixels.size(); y++)
        {
            for (auto x = 0; x < mappedPixels[y].size(); x++)
            {
                PixelData pixel_data = mappedPixels[y][x];
                auto new_x = std::cos(2 * M_PI * 50 + 220) * x + 50;
                auto new_y= 200* std::sin(2 * M_PI * 50 + 220) * y + 50;
                SDL_Rect rect = {.x = (int)new_x, .y = (int)new_y, .w = 1, .h = 1};
                SDL_SetRenderDrawColor(renderer, pixel_data.brightness, pixel_data.brightness, pixel_data.brightness, pixel_data.brightness);
                SDL_RenderFillRect(renderer, &rect);
            }
        }


        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }


    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(streamingTexture);
    SDL_FreeSurface(backgroundSurface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
    return 0;


}
