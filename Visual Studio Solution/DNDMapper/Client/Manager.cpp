#include "Manager.h"

//Construct the Manager with the specific window settings.
Manager::Manager(sf::ContextSettings settings): window(sf::VideoMode(WINDOWX, WINDOWY), "Dungeons and Dragons!", sf::Style::Default, settings){

	menuStack.push_back(std::unique_ptr<Game>(new Game(&window)));


}

Manager::~Manager(){

}

//The main event loop of the Manager object.
void Manager::mainLoop() {

	while (window.isOpen()) {
		sf::Event pollingEvent;
		while (window.pollEvent(pollingEvent)) {
			menuStack.back()->interpretEvent(pollingEvent);
		}

		menuStack.back()->update();

		window.clear();
		window.setView(menuStack.back()->getCamera());
		window.draw(*menuStack.back());
		window.display();
	}
}
