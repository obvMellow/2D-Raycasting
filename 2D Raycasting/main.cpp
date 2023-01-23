#include <SDL.h>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <vector>
#include "Point.h"

const int screenWidth = 1280;
const int screenHeight = 720;

// Higher the value, less intense the rays
float intensity = 2.0f;

Point line_intersection(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4) {
    Point intersection;
    double denominator = ((x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4));
    if (denominator == 0) {
        intersection.set_x(NAN);
        intersection.set_y(NAN);
    }
    else {
        double x = (((x1 * y2 - y1 * x2) * (x3 - x4)) - ((x1 - x2) * (x3 * y4 - y3 * x4))) / denominator;
        double y = (((x1 * y2 - y1 * x2) * (y3 - y4)) - ((y1 - y2) * (x3 * y4 - y3 * x4))) / denominator;
        intersection.set_x(x);
        intersection.set_y(y);
        if (x < fmin(x1, x2) || x > fmax(x1, x2) || y < fmin(y1, y2) || y > fmax(y1, y2) || x < fmin(x3, x4) || x > fmax(x3, x4) || y < fmin(y3, y4) || y > fmax(y3, y4)) {
            intersection.set_x(NAN);
            intersection.set_y(NAN);
        }
    }
    return intersection;
}

void draw_line(SDL_Renderer* renderer, std::pair<Point, Point> line) {
    SDL_RenderDrawLine(renderer, line.first.get_x(), line.first.get_y(), line.second.get_x(), line.second.get_y());
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Raycasting Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    std::vector<std::pair<Point, Point>> lines;
    lines.push_back(std::make_pair(Point(370, 140), Point(140, 280)));
    lines.push_back(std::make_pair(Point(620, 200), Point(730, 240)));
    lines.push_back(std::make_pair(Point(730, 240), Point(680, 120)));
    lines.push_back(std::make_pair(Point(680, 120), Point(600, 130)));
    lines.push_back(std::make_pair(Point(600, 130), Point(620, 200)));

    SDL_Event e;
    bool quit = false;
    int mouseX{}, mouseY{};

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_MOUSEMOTION:
                    mouseX = e.motion.x;
                    mouseY = e.motion.y;
                    break;
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_w) {
                        intensity -= 0.1f;
                        std::cout << intensity << "\n";
                    }
                    else if (e.key.keysym.sym == SDLK_s) {
                        intensity += 0.1f;
                        std::cout << intensity << "\n";
                    }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

        for (int i = 0; i < 360; i++) {
            float angle = (float)i * (M_PI / 180.0f) * intensity;
            int x2 = mouseX + cos(angle) * screenWidth * 2;
            int y2 = mouseY + sin(angle) * screenHeight * 2;

            for (auto& line : lines) {
                draw_line(renderer, line);

                Point intersection = line_intersection(mouseX, mouseY, x2, y2, line.first.get_x(), line.first.get_y(), line.second.get_x(), line.second.get_y());
                if (!std::isnan(intersection.get_x()) && !std::isnan(intersection.get_y())) {
                    x2 = intersection.get_x();
                    y2 = intersection.get_y();
                }
            }

            SDL_RenderDrawLine(renderer, mouseX, mouseY, x2, y2);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}