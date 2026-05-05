#include <iostream>
#include <cstring>
#include <signal.h>
#include <unistd.h>
#include <nlohmann/json.hpp>
#include "led-matrix.h"
#include "graphics.h"
#include "webrequest.hpp"

using namespace rgb_matrix;

static volatile bool running = true;

void DrawInfinity(Canvas *canvas, int x_offset, int y_offset, const Color &color) {
    const char* pattern[] = {
        "0011110000111100",
        "0110011001100110",
        "1100001111000011",
        "1100000110000011",
        "1100001111000011",
        "0110011001100110",
        "0011110000111100",
    };

    for (int y = 0; y < 7; y++) {
        for (int x = 0; x < 16; x++) {
            if (pattern[y][x] == '1') {
                int px = x + x_offset;
                int py = y + y_offset;

                if (px >= 0 && px < canvas->width() &&
                    py >= 0 && py < canvas->height()) {
                    canvas->SetPixel(px, py, color.r, color.g, color.b);
                }
            }
        }
    }
}

void SignalHandler(int) {
    running = false;
}

int main(int argc, char **argv) {
    // Matrix options and runtime options
    RGBMatrix::Options matrix_options;
    RuntimeOptions runtime_options;

    // Parse command-line flags automatically
    if (!ParseOptionsFromFlags(&argc, &argv, &matrix_options, &runtime_options)) {
        std::cerr << "Failed to parse options\n";
        return 1;
    }

    // Create matrix
    RGBMatrix *matrix = RGBMatrix::CreateFromOptions(matrix_options, runtime_options);
    if (!matrix) {
        std::cerr << "Failed to create matrix\n";
        return 1;
    }

    Canvas *canvas = matrix;
    Color orange(255, 128, 0);

    // Load font
    Font font;
    if (!font.LoadFont("../rpi-rgb-led-matrix/fonts/7x13B.bdf")) {
        std::cerr << "Failed to load font\n";
        return 1;
    }

    nlohmann::json departures = getDepartures();
    auto strings = parseDepartures(departures);

    std::string movingText = "";
    for (auto e: strings) {
        movingText += e[0] + " " + e[1] + "         ";
    }

    signal(SIGINT, SignalHandler);
    signal(SIGTERM, SignalHandler);

    const char *nextDestination = strings[0][0].c_str();
    const char *nextTime = strings[0][1].c_str();
    const char *rolling = movingText.c_str();
    int text_width = font.CharacterWidth('H') * strlen(rolling); // Approximate width
    int x = canvas->width(); // Start off-screen right

    while (running) {
        canvas->Clear();
        DrawText(canvas, font, 2, 13, orange, nextDestination);
	if (strings[0][1] == "OO") DrawInfinity(canvas, canvas->width() - 16 - 5, 3, orange);
        else DrawText(canvas, font, canvas->width() - font.CharacterWidth('H') * strlen(nextTime) - 1, 13, orange, nextTime);
        DrawText(canvas, font, x, 28, orange, rolling);

        x--; // Move text left
        if (x + text_width < 0) { // If completely off-screen left
            x = canvas->width();  // Reset to start from right
            departures = getDepartures();
            strings = parseDepartures(departures);
            movingText = "";
            int i = 0;
            for (auto e: strings) {
                if (i == 0) {
                    ++i;
                    continue;
                }
                movingText += e[0] + " " + e[1] + "         ";
            }
            nextDestination = strings[0][0].c_str();
            nextTime = strings[0][1].c_str();
            rolling = movingText.c_str();
            text_width = font.CharacterWidth('H') * strlen(rolling); // Approximate width
        }

        usleep(20000); // Adjust speed: lower = faster
    }

    matrix->Clear();
    delete matrix;
    return 0;
}
