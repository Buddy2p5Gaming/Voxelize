#include "Voxelize.hpp"

int main() {
    // run game
    Voxelize::Game game = Voxelize::Game();
    game.Run();

    // display error
    std::cout << game.error.json << std::endl;

    return 0;
}

