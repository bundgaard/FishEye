#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <array>
#include <cassert>

constexpr int StreamingWidth = 64;
constexpr int StreamingHeight = 64;

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main()
{

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);


    auto window = SDL_CreateWindow("FishEye", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512,
            /*SDL_WINDOW_FULLSCREEN_DESKTOP*/0);
    auto renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

    const auto home = std::getenv("HOME");
    std::string filepath(home);
    filepath += "/Pictures/20201111-assassins-creed-valhalla.png";
    std::cout << "Filepath" << filepath << std::endl;
    const auto backgroundSurface = IMG_Load(filepath.c_str());
    const auto backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);


    // Example on writing out the pixel data
    auto *pixels = (Uint32 *) backgroundSurface->pixels;
    std::array<uint32_t, 64 * 64> framebuf = {0};
    SDL_Texture *streamingTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING,
                                                      StreamingWidth, StreamingHeight);


    SDL_Rect streamingRect = {.x = 32, .y = 32, .w = StreamingWidth, .h = StreamingHeight};

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


        long ypos = map_up_y(255);

        long xpos = map_up_x(255);

        /*for (long y = ypos; y < ypos + StreamingHeight; y++)
        {
            for (long x = xpos; x < xpos + StreamingWidth; x++)
            {
                auto pixel = pixels[x + y * backgroundSurface->w];

                Uint8 red, green, blue, alpha;
                SDL_GetRGBA(pixel, backgroundSurface->format, &red, &green, &blue, &alpha);
                // std::printf("scale=%d,%d, pixel=%x, color=%d,%d,%d\n", x, y,pixel, red, green, blue);
                // AA RR GG BB

                long index = map_down_x(x) + map_down_y(y) * 64;
                long center_index = 32 + 32 * 64;
                long new_y = center_index + (64.0 * std::sin(20 * 3.14576 * 2 * 50 + y));
                long new_x = center_index + (64.0 * std::cos(20 * 3.14576 * 2 * 50 + x));
                new_y = map_down_y(new_y);
                new_x = map_down_x(new_x);
            //    std::printf("index %d, new %d,%d\n", index, new_x, new_y);
                // framebuf[(x - xpos)+ (new_y - ypos) * 64] = 0xffff1111;//alpha << 24 | red << 16 | green << 8 | blue;
                // framebuf[new_x + new_y * 64] = 0xffff1111;//alpha << 24 | red << 16 | green << 8 | blue;

            }
        }*/

        auto pixel = [pixels, backgroundSurface](long x, long y)
        {
            auto color = pixels[x + y * backgroundSurface->w];
            SDL_Color color_s;
            SDL_GetRGBA(color, backgroundSurface->format, &color_s.r, &color_s.g, &color_s.b, &color_s.a);
            return (Uint32) (color_s.a << 24 | color_s.r << 16 | color_s.g << 8 | color_s.b);
        };

        /*   for (int i = 0; i < 32; i++)
           {
               auto left = pixel(map_up_x(streamingRect.x - i), map_up_y(streamingRect.y));
               framebuf[(32 - i) + 32 * 64] = left;
           }

           for (int i = 0; i < 32; i++)
           {
               auto left = pixel(map_up_x(streamingRect.x + i), map_up_y(streamingRect.y));
               framebuf[(32 + i) + 32 * 64] = left;
           }
   */
        // NORTHWEST
        for (int j = 0; j < 32; j++) // move x from 32 to 0
        {
            for (int i = 0; i < 32; i++) // move y from 32 to zero
            {
                auto left = pixel(map_up_x((streamingRect.x + 32) - j), map_up_y((streamingRect.y + 32) - i));
                framebuf[(32 - j) + (32 - i) * 64] = left;
            }

        }
        // NORTHEAST
        for (int j = 0; j < 32; j++) // move x from 32 to 0
        {
            for (int i = 0; i < 32; i++) // move y from 32 to zero
            {
                auto x_pos = map_up_x((streamingRect.x + 32) + j);
                auto y_pos = map_up_y((streamingRect.y + 32) - i);
                auto left = pixel(x_pos, y_pos);


                framebuf[(32 + j) + (32 - i) * 64] = left;
            }

        }
// SOUTHWEST
        for (int j = 0; j < 32; j++) // move x from 32 to 0
        {
            for (int i = 0; i < 32; i++) // move y from 32 to zero
            {
                auto left = pixel(map_up_x((streamingRect.x + 32) - j), map_up_y((streamingRect.y + 32) + i));

                framebuf[(32 - j) + (32 + i) * 64] = left;
            }

        }


        // SOUTHEAST
        for (int j = 0; j < 32; j++) // move x from 32 to 0
        {
            for (int i = 0; i < 32; i++) // move y from 32 to zero
            {
                auto left = pixel(map_up_x((streamingRect.x + 32) + j), map_up_y((streamingRect.y + 32) + i));
                framebuf[(32 + j) + (32 + i) * 64] = left;
            }

        }

        for (auto i = 0.0; i < M_PI * 2; i += 0.01)
        {
            int mid_x = 32;
            int mid_y = 32;
            int x = mid_x + (31 * std::cos(i));
            int y = mid_y + (31 * std::sin(i));

            auto foo = atan2(y, x);
            //    std::printf("foo %f\n", foo);

            assert((x + y * 64 < 64 * 64));
            framebuf[x + y * 64] = 0xffff0000;
        }

        framebuf[32 + 32 * 64] = 0xffffffff; // center dot

        SDL_UpdateTexture(streamingTexture, nullptr, framebuf.data(), StreamingWidth * 4);

        streamingRect.x += dx;
        streamingRect.y += dy;


        // std::printf("rect(%d,%d)\n", streamingRect.x, streamingRect.y);
        if (streamingRect.x < 0 || streamingRect.x + streamingRect.w > 512)
        {
            dx *= -1;
        }
        if (streamingRect.y < 0 || streamingRect.y + streamingRect.h > 512)
        {
            dy *= -1;
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);


        //  SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);

        SDL_RenderCopy(renderer, streamingTexture, nullptr, &streamingRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }



    /*
    std::printf("map x = %ld\n", map_up_x(250));
    auto y_pos = map_up_y(streamingRect.y);
    auto x_pos = map_up_x(streamingRect.x);
    for (long y = y_pos; y < y_pos + StreamingHeight; y++)
    {
        for (long x = x_pos; x < x_pos + StreamingWidth; x++)
        {
            auto pixel = pixels[x + y * backgroundSurface->w];

            Uint8 red, green, blue, alpha;
            SDL_GetRGBA(pixel, backgroundSurface->format, &red, &green, &blue, &alpha);
           // std::printf("scale=%d,%d, pixel=%x, color=%d,%d,%d\n", x, y,pixel, red, green, blue);
            // AA RR GG BB
            framebuf[(x - x_pos) + (y - y_pos) * 64] = alpha << 24 | red << 16 | green << 8 | blue;

        }
    }
    SDL_UpdateTexture(streamingTexture, nullptr, framebuf.data(), StreamingWidth * 4);

    SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
    SDL_RenderCopy(renderer, streamingTexture, nullptr, &streamingRect);
    SDL_RenderPresent(renderer);
      SDL_Delay(3000);
*/



    SDL_FreeSurface(backgroundSurface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
    return 0;


}
