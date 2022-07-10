//
// Created by dbundgaard on 2022-07-10.
//

#ifndef FISHEYE_PARTICLE_H
#define FISHEYE_PARTICLE_H

#include <SDL2/SDL.h>

struct PixelData
{
    double brightness; // is the average of red, green, blue
    SDL_Color color;
    Uint32 dot;
};

class Particle
{
public:
    int x, y;
    int speed;
    int velocity;
    int size;
    int position1, position2;
    int angle;
    int width, height;
public:
    Particle(int, int);
    void Update(std::vector<std::vector<PixelData>> *);
    void Draw();
};
#endif //FISHEYE_PARTICLE_H
