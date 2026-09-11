#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <array>
#include <random>


enum class side { Left, Right, None };
constexpr int NUM_BRANCHES = 6;
constexpr float PLAYER_POSITION_RIGHT = 1250.f;
constexpr float PLAYER_POSITION_LEFT = 630.f;
constexpr float PLAYER_POSITION_Y = 700.f;
constexpr float AXE_POSITION_RIGHT = 1075.f;
constexpr float AXE_POSITION_LEFT = 700.f;
constexpr float AXE_POSITION_Y = 830.f;
constexpr float LOG_SPEED_X = 2000.f;
constexpr float LOG_SPEED_Y = 500.f;
constexpr float LOG_POSITION_X = 800.f;
constexpr float LOG_POSITION_Y = 760.f;
constexpr float WINDOW_WIDTH = 1920.f;
constexpr float WINDOW_HEIGHT = 1080.f;
constexpr float MAX_TIME = 10.0f;
constexpr float timeBarStartWidth = 400.f;
constexpr float timeBarHeight = 80.f;
constexpr float timeBarWidthPerSecond = timeBarStartWidth / MAX_TIME;

class Game {
public:
	Game();
	void run();
private:
	void update(sf::Time elapsedTime);
	void render();
	void processEvent();
	void reset();
	void updateStatistics(sf::Time elapsedTime);
	void updateBranches();
	
	


private:
	void chop(side chopSide);
private:
	sf::RenderWindow window;
	sf::Clock clock;

	sf::Texture textureBackground;
	sf::Texture textureCloud;
	sf::Texture textureTree;
	sf::Texture textureBranch;
	sf::Texture texturePlayer;
	sf::Texture textureAxe;
	sf::Texture textureLog;
	sf::Texture textureGrave;

	sf::Sprite spriteBackground;
	sf::Sprite spriteCloud;
	sf::Sprite spriteTree;
	sf::Sprite spritePlayer;
	sf::Sprite spriteAxe;
	sf::Sprite spriteLog;
	sf::Sprite spriteGrave;
	std::array<sf::Sprite, NUM_BRANCHES> branches;

	sf::Font font;
	sf::SoundBuffer chopBuffer;
	sf::SoundBuffer deathBuffer;
	sf::SoundBuffer ootBuffer;
	sf::Sound chopSound;
	sf::Sound deathSound;
	sf::Sound ootSound;

	sf::Text scoreText;
	sf::Text message;
	sf::Text statisticText;

	
	int statisticNumFrame;
	int score;

	sf::Time statisticUpdateTime;

	float cloudSpeed;
	float timeRemaining;


	bool cloudActive;
	bool logActive;
	bool gamePaused;

	side playerSide;
	std::array <side, NUM_BRANCHES> branchPosition;

	sf::RectangleShape timeBar;

	std::mt19937 gen;
	std::uniform_int_distribution<int> distribBranch;
	std::uniform_real_distribution<float> distribCloudSpeed;
	std::uniform_real_distribution<float> distribCloudHeight;
};
