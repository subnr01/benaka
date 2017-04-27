/*

To have a readable and intuitive way of creating objects of a class
To impose certain constraints while creating objects of a class

*/


class Game
{
  public:
    static Game createSinglePlayerGame() { return Game(0); } // named constructor
    static Game createMultiPlayerGame() { return Game(1); }  // named constructor
  protected:
    Game (int game_type);
};
int main(void)
{
   Game g1 = Game::createSinglePlayerGame(); // Using named constructor
   Game g2 = Game(1); // multiplayer game; without named constructor (does not compile)
}
