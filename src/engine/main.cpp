#include "window.hpp"
#include "version.hpp"

int main(int argc, char* argv[]) {
    Window_Initialize(I_GAME_WIDTH, I_GAME_HEIGHT, I_GAME_TITLE, argc, argv);
    return 0;
}