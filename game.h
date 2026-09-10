#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <array>
#include <ctime>


enum class side { Left, Right, None };
constexpr const int NUM_BRANCHES = 6;
void setText(sf::Text& message, std::string text, float x, float y);
void setPlayerAndAxe(sf::Sprite& player, sf::Sprite& axe, side side);
const float PLAYER_POSITION_RIGHT = 1250.f;
const float PLAYER_POSITION_LEFT = 630.f;
const float AXE_POSITION_RIGHT = 1075.f;
const float AXE_POSITION_LEFT = 700.f;
const float LOG_SPEED_X = 2000.f;
const float LOG_SPEED_Y = 500.f;
const float MAX_TIME = 10.f;
constexpr float timeBarStartWidth = 400.f;
constexpr float timeBarHeight = 80.f;

class Game {
public:
	Game();
	void run();

	void update(sf::Time elapsedTime);
	void render();
	void processEvent();
	void reset();
	void updateStatistics(sf::Time elapsedTime);
	void updateBranches();

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

	sf::Text scoreText;
	sf::Text message;
	sf::Text statisticText;

	sf::Sound chopSound;
	sf::Sound deathSound;
	sf::Sound ootSound;

	int statisticNumFrame;
	int score;

	sf::Time statisticUpdateTime;

	float cloudSpeed;
	float timeRemaining;

	float timeBarWidthPerSecond;

	bool cloudActive;
	bool logActive;
	bool gamePaused;

	side playerSide;
	std::array <side, NUM_BRANCHES> branchPosition;

	sf::RectangleShape timeBar;

};
