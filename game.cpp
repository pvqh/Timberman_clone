#include "game.h"
#include <sstream>
#include <algorithm>

void setText(sf::Text& message, std::string_view text, float x, float y);
void setPlayerPosition(sf::Sprite& player, side side);
void setAxePosition(sf::Sprite& axe, side side);


Game::Game()
	: window(sf::VideoMode::getDesktopMode(), "Timberman", sf::State::Fullscreen)
	, textureBackground("resources/graphics/background.png")
	, textureCloud("resources/graphics/cloud.png")
	, textureTree("resources/graphics/tree2.png")
	, textureBranch("resources/graphics/branch.png")
	, texturePlayer("resources/graphics/player.png")
	, textureAxe("resources/graphics/axe.png")
	, textureLog("resources/graphics/log.png")
	, textureGrave("resources/graphics/rip.png")
	, spriteBackground(textureBackground)
	, spriteCloud(textureCloud)
	, spriteTree(textureTree)
	, spritePlayer(texturePlayer)
	, spriteAxe(textureAxe)
	, spriteLog(textureLog)
	, spriteGrave(textureGrave)
	, branches{
		sf::Sprite(textureBranch)
		,sf::Sprite(textureBranch)
		,sf::Sprite(textureBranch)
		,sf::Sprite(textureBranch)
		,sf::Sprite(textureBranch)
		,sf::Sprite(textureBranch)
	}
	, font("resources/fonts/KOMIKAP_.ttf")
	, chopBuffer("resources/sound/chop.wav")
	, deathBuffer("resources/sound/death.wav")
	, ootBuffer("resources/sound/out_of_time.wav")
	, chopSound(chopBuffer)
	, deathSound(deathBuffer)
	, ootSound(ootBuffer)
	, scoreText(font, std::string("Score = 0"), 60)
	, message(font)
	, statisticText(font)
	, statisticNumFrame(0)
	, score(0)
	, statisticUpdateTime(sf::Time::Zero)
	, cloudSpeed(0.0f)
	, timeRemaining(MAX_TIME)
	, cloudActive(false)
	, logActive(false)
	, gamePaused(true)
	, playerSide(side::Left)
	, branchPosition{
		side::None
		, side::None
		, side::None
		, side::None
		, side::None
		, side::None
	}
	, timeBar({0.0f, 0.0f})
	, gen(std::random_device{}())
	, distribBranch(0, 5)
	, distribCloudSpeed(50,100)
	, distribCloudHeight(50,100)
	

{
	sf::View view(sf::FloatRect({ 0.f,0.f }, { WINDOW_WIDTH, WINDOW_HEIGHT }));
	window.setView(view);
	window.setFramerateLimit(60);
	reset();
}

void Game::run()
{
	while (window.isOpen())
	{
		sf::Time dt = clock.restart();

		processEvent();
		update(dt);
		render();
		updateStatistics(dt);

	}
}


void Game::update(sf::Time dt)
{
	if (!gamePaused)
	{
		timeRemaining -= dt.asSeconds();
		timeBar.setSize({ timeBarWidthPerSecond * timeRemaining, timeBarHeight });
		if (timeRemaining <= 0.f)
		{
			gamePaused = true;
			setText(message, "Out Of Time", (WINDOW_WIDTH / 2.f), (WINDOW_HEIGHT/ 2.f));
			ootSound.play();
		}

		if (!cloudActive)
		{
			
			cloudSpeed = distribCloudSpeed(gen);
			float cloudHeight = distribCloudHeight(gen);

			spriteCloud.setPosition(sf::Vector2f( 2000, cloudHeight ));
			cloudActive = true;

		}
		else {

			spriteCloud.setPosition({ spriteCloud.getPosition().x - (cloudSpeed * dt.asSeconds()), spriteCloud.getPosition().y });

			if (spriteCloud.getPosition().x < -100.f)
			{
				cloudActive = false;
			}
		}

		if (branchPosition[NUM_BRANCHES - 1] == playerSide)
		{
			gamePaused = true;
			if (playerSide == side::Left)
			{
				spriteGrave.setPosition(sf::Vector2f(600, 750));
			}
			else
			{
				spriteGrave.setPosition(sf::Vector2f(1200, 750));
			}
			setText(message, "SQUISHED!!!", (WINDOW_WIDTH / 2.f), (WINDOW_HEIGHT / 2.f));
			deathSound.play();
			logActive = false;
			cloudActive = false;
		}

		if (logActive)
		{
			spriteLog.setPosition(sf::Vector2f(spriteLog.getPosition().x - (LOG_SPEED_X * dt.asSeconds()), spriteLog.getPosition().y - (LOG_SPEED_Y * dt.asSeconds())));
		}
		if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000)
		{
			logActive = false;
			spriteLog.setPosition(sf::Vector2f(LOG_POSITION_X, LOG_POSITION_Y));
		}
	}
}

void Game::processEvent()
{
	while (const std::optional<sf::Event> event = window.pollEvent())
	{
		if (event->getIf<sf::Event::KeyReleased>() && !gamePaused)
		{
			spriteAxe.setPosition(sf::Vector2f(2100, AXE_POSITION_Y));
		}
		if (event->is<sf::Event::Closed>()) {
			window.close();
		}
		if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
			if (key->code == sf::Keyboard::Key::Escape)
			{
				window.close();
			}
			if (gamePaused)
			{
				if (key->code == sf::Keyboard::Key::Enter)
				{
					gamePaused = false;
					reset();
				}
			}
			else
			{
				if (key->code == sf::Keyboard::Key::Left)
				{
					chop(side::Left);
				}
				if (key->code == sf::Keyboard::Key::Right)
				{
					chop(side::Right);
				}
			}
		}
	}
}

void Game::render()
{
	window.clear();
	window.draw(spriteBackground);
	window.draw(scoreText);
	window.draw(statisticText);
	window.draw(spriteCloud);
	window.draw(spriteTree);
	for (const auto& branch : branches) {
		window.draw(branch);
	}

	if (!gamePaused)
	{
		window.draw(timeBar);
		window.draw(spritePlayer);
		window.draw(spriteAxe);
		window.draw(spriteLog);
	}
	if (gamePaused)
	{
		window.draw(message);
		window.draw(spriteGrave);
	}
	window.display();
}

void Game::reset()
{

	cloudActive = false;
	logActive = false;
	cloudSpeed = 0;
	timeRemaining = MAX_TIME;
	score = 0;

	// Setup player and objects
	playerSide = side::Left;
	setPlayerPosition(spritePlayer, side::Left);
	setAxePosition(spriteAxe, side::Left);
	spriteTree.setPosition(sf::Vector2f(1600 / 2.f, 0));
	spriteGrave.setPosition(sf::Vector2f(2100, -2000));
	spriteLog.setPosition(sf::Vector2f(2100, -2000));


	// Setup timebar
	timeBar.setSize({ timeBarStartWidth, timeBarHeight });
	timeBar.setFillColor(sf::Color::Red);
	timeBar.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.f - timeBarStartWidth / 2.f, 980));

	// Setup texts
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition({ 200, 200 });

	setText(message, "Press Enter to start", (WINDOW_WIDTH / 2.f), (WINDOW_HEIGHT / 2.f));

	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branchPosition[i] = side::None;
		branches[i].setPosition({ -2000, -2000 });
		branches[i].setOrigin(sf::Vector2f(220, 40));

	}
}

void Game::updateBranches()
{
	for (int i = NUM_BRANCHES - 1; i > 0; --i)
	{
		branchPosition[i] = branchPosition[i - 1];
	}

	// branches appear in probability 20/20/60

	int r = distribBranch(gen);
	switch (r)
	{
	case 0:
	{
		branchPosition[0] = side::Left;
		break;
	}
	case 1:
	{
		branchPosition[0] = side::Right;
		break;
	}
	default:
	{
		branchPosition[0] = side::None;
		break;
	}
	}


	for (int i = 0; i < NUM_BRANCHES; i++)
	{

		float height = i * 150.f;
		if (branchPosition[i] == side::Left) {
			branches[i].setPosition(sf::Vector2f(580, height));
			branches[i].setRotation(sf::degrees(180));
		}
		else if (branchPosition[i] == side::Right) {
			branches[i].setPosition(sf::Vector2f(1300, height));
			branches[i].setRotation(sf::Angle::Zero);
		}
		else {
			branches[i].setPosition(sf::Vector2f(3000, height));
		}
	}

}

void Game::updateStatistics(sf::Time elapsedTime)
{
	statisticUpdateTime += elapsedTime;
	statisticNumFrame += 1;

	if (statisticUpdateTime >= sf::seconds(1.0f))
	{
		statisticText.setString("FPS: " + std::to_string(statisticNumFrame));
		statisticNumFrame = 0;
		statisticUpdateTime = sf::Time::Zero;
	}
	scoreText.setString("Score = " + std::to_string(score));
}



void setText(sf::Text& message, std::string_view text, float x, float y)
{
	message.setString(text);
	sf::FloatRect textRect = message.getLocalBounds();
	message.setOrigin({ textRect.getCenter() });
	message.setPosition({ x, y });
	message.setFillColor(sf::Color::White);

}

void setPlayerPosition(sf::Sprite& player, side side)
{
	if (side == side::Left)
	{
		player.setScale(sf::Vector2f(-1.f, 1.f));
		player.setPosition(sf::Vector2f(PLAYER_POSITION_LEFT, PLAYER_POSITION_Y));
	}
	if (side == side::Right)
	{
		player.setScale(sf::Vector2f(1.f, 1.f));
		player.setPosition(sf::Vector2f(PLAYER_POSITION_RIGHT, PLAYER_POSITION_Y));

	}
}

void setAxePosition(sf::Sprite& axe, side side)
{
	if (side == side::Left)
	{
		axe.setPosition(sf::Vector2f(AXE_POSITION_LEFT, AXE_POSITION_Y));
	}
	if (side == side::Right)
	{
		axe.setPosition(sf::Vector2f(AXE_POSITION_RIGHT, AXE_POSITION_Y));
	}
}

void Game::chop(side chopSide)
{
	playerSide = chopSide;
	score = score + 10;
	timeRemaining += (2.f / score) + 0.15f;
	timeRemaining = std::min(MAX_TIME, timeRemaining);
	setPlayerPosition(spritePlayer, chopSide);
	setAxePosition(spriteAxe, chopSide);
	updateBranches();
	spriteLog.setPosition(sf::Vector2f(LOG_POSITION_X, LOG_POSITION_Y));
	logActive = true;
	chopSound.play();
}