//
// Created by dbundgaard on 2022-07-10.
//

#include <random>
#include <functional>
#include "Particle.h"
template<typename T>
static int between(T min, T max) {
    std::random_device rnd;
    std::mt19937 mt_rand(rnd());

    auto distribution = std::uniform_int_distribution<>(min, max);
    return distribution(mt_rand);
}

void Particle::Update(std::vector<std::vector<PixelData>>* pData)
{
    position1 = std::floor(y);
    position2 = std::floor(x);
    auto movement = (2.5 - speed) + velocity;
    speed = pData->at(position1).at(position2).brightness;
    angle += speed/40;
    size = speed*  1.5;

    y += (int)movement;
    x -= (int)(movement + std::sin(angle) * 2.0);

    if(y > height) {
        y = 0;
        x = between(0, width);
    }
    if(x <0)
    {
        y = between(0,height);
        x = width;
    }

}

void Particle::Draw()
{

}

Particle::Particle(int width, int height)
{
    x = between(0, width);
    y = between(0, height);
    speed = 1;
    velocity = between(0.0, 0.5);
    size = between(0.2, 2.5);
    position1 = std::floor(y);
    position2 = std::floor(x);
    angle = 0;

    this->height = height;
    this->width = width;
}
