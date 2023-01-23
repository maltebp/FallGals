#include <iostream>

#include <Core/GameController.hpp>
#include <Game/ChooseMapScene.hpp>

using namespace std;
using namespace fg::core;
using namespace fg::game;


int main(int arg, const char** argv) {

	GameController gameController;
	
	shared_ptr<ChooseMapScene> scene = make_shared<ChooseMapScene>(gameController);

	gameController.run(scene);
	
	return 0; 
}