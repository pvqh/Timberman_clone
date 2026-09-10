#include "game.h"
#include <sstream>


Game::Game()
	: window(sf::VideoMode::getDesktopMode(), "Timberman", sf::State::Fullscreen)
	, font("resources/fonts/KOMIKAP_.ttf")
	, textureBackground("resources/graphics/background.png")
	, textureCloud("resources/graphics/cloud.png")
	, textureTree("resources/graphics/tree2.png")
	, texturePlayer("resources/graphics/player.png")
	, textureAxe("resources/graphics/axe.png")
	, textureLog("resources/graphics/log.png")
	, textureGrave("resources/graphics/rip.png")
	, textureBranch("resources/graphics/branch.png")
	, spriteBackground(textureBackground)
	, spriteCloud(textureCloud)
	, spriteTree(textureTree)
	, spritePlayer(texturePlayer)
	, spriteAxe(textureAxe)
	, spriteLog(textureLog)
	, spriteGrave(textureGrave)
	, chopBuffer("resources/sound/chop.wav")
	, deathBuffer("resources/sound/death.wav")
	, ootBuffer("resources/sound/out_of_time.wav")
	, chopSound(chopBuffer)
	, deathSound(deathBuffer)
	, ootSound(ootBuffer)
	, statisticText(font)
	, scoreText(font, std::string("Score = 0"), 60)
	, message(font)
	, branches{
		sf::Sprite(textureBranch)
		,sf::Sprite(textureBranch)
		,sf::Sprite(textureBranch)
		,sf::Sprite(textureBranch)
		,sf::Sprite(textureBranch)
		,sf::Sprite(textureBranch)
	}
	, statisticUpdateTime(sf::Time::Zero)
	, statisticNumFrame(0)
	, gamePaused(true)
	, timeBarWidthPerSecond(timeBarStartWidth / timeRemaining)

{
	sf::View view(sf::FloatRect({ 0.f,0.f }, { 1920.f, 1080.f }));
	window.setView(view);
	window.setFramerateLimit(60);
	reset();
}

void Game::run()
{
	srand((int)(time(0)));

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
			setText(message, "Out Of Time", (1920 / 2.f), (1080 / 2.f));
			ootSound.play();
		}

		if (!cloudActive)
		{
			cloudSpeed = (rand() % 50) + 50.f;
			float cloudHeight = (rand() % 100) + 50.f;

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
			setText(message, "SQUISHED!!!", (1920 / 2.f), (1080 / 2.f));
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
			spriteLog.setPosition(sf::Vector2f(800, 760));
		}
	}
}

void Game::processEvent()
{
	while (const std::optional event = window.pollEvent())
	{
		if (event->is<sf::Event::KeyReleased>() && !gamePaused)
		{
			spriteAxe.setPosition(sf::Vector2f(2100, 830));
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
					playerSide = side::Left;
					score = score + 10;
					if (timeRemaining > MAX_TIME)
					{
						timeRemaining = MAX_TIME;
					}
					else {
						timeRemaining += (2.f / score) + 0.15f;
					}
					setPlayerAndAxe(spritePlayer, spriteAxe, side::Left);
					updateBranches();
					spriteLog.setPosition(sf::Vector2f(800, 760));
					logActive = true;
					chopSound.play();
				}
				if (key->code == sf::Keyboard::Key::Right)
				{
					playerSide = side::Right;
					score = score + 10;
					if (timeRemaining > MAX_TIME)
					{
						timeRemaining = MAX_TIME;
					}
					else {
						timeRemaining += (2.f / score) + 0.15f;
					}
					setPlayerAndAxe(spritePlayer, spriteAxe, side::Right);
					updateBranches();
					spriteLog.setPosition(sf::Vector2f(800, 760));
					logActive = true;
					chopSound.play();

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
	for (int i = 0; i < NUM_BRANCHES; i++) {
		window.draw(branches[i]);
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
	setPlayerAndAxe(spritePlayer, spriteAxe, side::Left);
	spriteTree.setPosition(sf::Vector2f(1600 / 2.f, 0));
	spriteGrave.setPosition(sf::Vector2f(2100, -2000));
	spriteLog.setPosition(sf::Vector2f(2100, -2000));


	// Setup timebar
	timeBar.setSize({ timeBarStartWidth, timeBarHeight });
	timeBar.setFillColor(sf::Color::Red);
	timeBar.setPosition(sf::Vector2f(1920 / 2.f - timeBarStartWidth / 2.f, 980));

	// Setup texts
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition({ 200, 200 });

	setText(message, "Press Enter to start", (1920 / 2.f), (1080 / 2.f));

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


	int r = (rand() % 5);
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



void setText(sf::Text& message, std::string text, float x, float y)
{
	message.setString(text);
	sf::FloatRect textRect = message.getLocalBounds();
	message.setOrigin({ textRect.getCenter() });
	message.setPosition({ x, y });
	message.setFillColor(sf::Color::White);

}

void setPlayerAndAxe(sf::Sprite& player, sf::Sprite& axe, side side)
{
	if (side == side::Left)
	{
		player.setScale(sf::Vector2f(-1.f, 1.f));
		player.setPosition(sf::Vector2f(PLAYER_POSITION_LEFT, 700));
		axe.setPosition(sf::Vector2f(AXE_POSITION_LEFT, 830));
	}
	if (side == side::Right)
	{
		player.setScale(sf::Vector2f(1.f, 1.f));
		player.setPosition(sf::Vector2f(PLAYER_POSITION_RIGHT, 700));
		axe.setPosition(sf::Vector2f(AXE_POSITION_RIGHT, 830));

	}
}