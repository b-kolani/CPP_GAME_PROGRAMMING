// Include important libraries here
#include "../../SFML/include/SFML/Graphics.hpp"
// #include "../../SFML/include/SFML/Audio.hpp"
#include <iostream>
#include <sstream>
#include <math.h>
#include <vector>

// Make the code easier to type "using namespace"
using namespace sf;

// Function declaration
void updateBranches(int seed);

const int NUM_BRANCHES = 6;
// Here we need to create a temporary texture
// because Sprite class has no default constructor
// only parameterized constructors. Another
// alterative is to used vector and call
// emplace_back to construct each element
// directly in the container.
Texture tempTex;
Sprite branches[NUM_BRANCHES] = {
	Sprite(tempTex),
	Sprite(tempTex),
	Sprite(tempTex),
	Sprite(tempTex),
	Sprite(tempTex),
	Sprite(tempTex)};
// std::vector<Sprite> branches;

// Where is palyer/branch ?
// LEFT or RIGHT
enum class side
{
	LEFT,
	RIGHT,
	NONE
};
side branchPositions[NUM_BRANCHES];

// This where our game starts from
int main()
{
	// std::cout << static_cast<int>(branchPositions[3]) << std::endl;
	//  Create a video mode object
	VideoMode vm({1920, 1080});

	// Create and open a window for the game
	RenderWindow window(vm, "Timber!!!", Style::Default);

	// create a texture to hold a graphic on the GPU
	Texture textureBackground;

	// Load a graphic into the texture
	if (!textureBackground.loadFromFile("./graphics/background.png"))
		return -1;
	// Create a sprite
	// And attach the texture to to the sprite
	Sprite spriteBackground(textureBackground);

	// Set the spriteBackground to cover the screen
	spriteBackground.setPosition({0, 0});

	// Make a tree sprite
	Texture textureTree;
	if (!textureTree.loadFromFile("graphics/tree.png"))
		return -1;
	Sprite spriteTree(textureTree);
	spriteTree.setPosition({810, 0});

	// Prepare the bee
	Texture textureBee;
	if (!textureBee.loadFromFile("graphics/bee.png"))
		return -1;
	Sprite spriteBee(textureBee);
	spriteBee.setPosition({0, 800});
	// Is the bee currently moving ?
	bool beeActive = false;
	// How fast can the bee fly
	float beeSpeed = 0.0f;

	// make 3 cloud sprites from 1 texture
	Texture textureCloud;
	// Load 1 new texture
	if (!textureCloud.loadFromFile("graphics/cloud.png"))
		return -1;
	// 3 New sprites with the same texture
	Sprite spriteCloud1(textureCloud);
	Sprite spriteCloud2(textureCloud);
	Sprite spriteCloud3(textureCloud);
	// Position the clouds on the left of the screen
	// at different heights
	spriteCloud1.setPosition({0, 0});
	spriteCloud2.setPosition({0, 250});
	spriteCloud3.setPosition({0, 500});
	// Are the clouds currently on screen ?
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;
	// How fast is each cloud?
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	// variables to control time itself
	Clock clock;

	// Track wether the game is running or not
	bool paused = true;
	bool playerDied = false;

	// Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition(Vector2f((1920 / 2) - (timeBarStartWidth / 2), 980));

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// Draw some text
	int score = 0;
	int oldScore = 0;

	// We need to choose a font
	Font font("fonts/KOMIKAP_.ttf");

	// Set the font to our messages
	// And assign the actual message
	// Make it really big
	Text scoreText(font, "Score = 0", 75);
	Text fpsText(font, "FPS = 0", 75);
	Text messageText(font, "Press Enter to start!", 100);

	// Choose a color
	scoreText.setFillColor(Color::White);
	scoreText.setOutlineThickness(1.f);
	messageText.setFillColor(Color::White);
	messageText.setOutlineThickness(1);
	fpsText.setFillColor(Color::White);
	fpsText.setOutlineThickness(1.f);

	// Position the text
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin({textRect.position.x + textRect.size.x / 2.0f,
						   textRect.position.y + textRect.size.y / 2.0f});
	messageText.setPosition({1920 / 2.0f, 1080 / 2.0f});

	FloatRect scoreRect = scoreText.getLocalBounds();
	scoreText.setOrigin({scoreRect.position.x + scoreRect.size.x / 2.0f,
						 scoreRect.position.y + scoreRect.size.y / 2.f});
	RectangleShape scoreRectShape({scoreRect.size.x + 20.0f, scoreRect.size.y + 20.f});
	scoreRectShape.setPosition(Vector2f(10.f, 20.f));
	scoreRectShape.setFillColor(Color(0, 0, 0, 150));
	FloatRect rectShapeBounds = scoreRectShape.getGlobalBounds();
	std::cout << rectShapeBounds.position.x + rectShapeBounds.size.x / 2.f << std::endl;
	scoreText.setPosition({rectShapeBounds.position.x + rectShapeBounds.size.x / 2.f,
						   rectShapeBounds.position.y + rectShapeBounds.size.y / 2.f});

	FloatRect fpsRect = fpsText.getLocalBounds();
	fpsText.setOrigin(Vector2f(fpsRect.position.x + fpsRect.size.x / 2.f,
							   fpsRect.position.y + fpsRect.size.y / 2.0f));
	RectangleShape fpsRectShape({520, fpsRect.size.y + 20.f});
	fpsRectShape.setPosition(Vector2f(1400.f, 20.f));
	fpsRectShape.setFillColor(Color(0, 0, 0, 150));
	FloatRect fpsRectShapeBounds = fpsRectShape.getGlobalBounds();
	fpsText.setPosition({fpsRectShapeBounds.position.x + fpsRectShapeBounds.size.x / 3.0f,
						 fpsRectShapeBounds.position.y + fpsRectShapeBounds.size.y / 2.0f});

	// Prepare 6 branches
	Texture textureBranch;
	if (!textureBranch.loadFromFile("graphics/branch.png"))
		return -1;

	// Sprite	tempBranch(textureBranch);
	// tempBranch.setPosition({1330, 40});
	// Set the texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		// Without the true as second argument the sprite
		// will keep the initial empty texture used
		// previously to construct each element
		// of the array and in thi case the branch
		// sprite won't appear. By setting true
		// the sprite will use reset the size of the former
		// and use the new texture.
		branches[i].setTexture(textureBranch, true);
		// branches.emplace_back(textureBranch);
		branches[i].setPosition({-2000, -2000});

		// Set the sprite's origin to dead centre
		// We can then spin it round without changing its position
		branches[i].setOrigin({220, 20});
	}

	// updateBranches(1);
	// updateBranches(2);
	// updateBranches(3);
	// updateBranches(4);
	// updateBranches(5);

	// Prepare the player
	Texture texturePlayer;
	if (!texturePlayer.loadFromFile("./graphics/player.png"))
		return -1;
	Sprite spritePlayer(texturePlayer);
	spritePlayer.setPosition(Vector2f(580.f, 720.f));

	// The player starts on the left
	side playerSide = side::LEFT;

	// Prepare the gravestone
	Texture textureRIP;
	if (!textureRIP.loadFromFile("graphics/rip.png"))
		return -1;
	Sprite spriteRIP(textureRIP);
	spriteRIP.setPosition({600.f, 860.f});

	// Prepare the axe
	Texture textureAxe;
	if (!textureAxe.loadFromFile("graphics/axe.png"))
		return -1;
	Sprite spriteAxe(textureAxe);
	spriteAxe.setPosition(Vector2f(700.f, 830.f));

	// Line the axe up with the tree
	const float AXE_POSITION_LEFT = 700.f;
	const float AXE_POSITION_RIGHT = 1075.f;

	//  Prepare the flying log
	Texture textureLog;
	if (!textureLog.loadFromFile("graphics/log.png"))
		return -1;
	Sprite spriteLog(textureLog);
	spriteLog.setPosition(Vector2f(810.f, 720.f));

	// Some other useful log related variables
	bool logActive = false;
	float logSpeedX = 1000.f;
	float logSpeedY = -1500.f;

	// Control the player input
	bool acceptInput = false;

	// Prepare the sounds
	// The player chopping sound
	// SoundBuffer chopbuffer;
	// if (!chopbuffer.loadFromFile("sound/chop.wav"))
	// 	return -1;
	// Sound chop(chopbuffer);

	// The player has met his end under a branch
	// SoundBuffer deathBuffer;
	// if (!deathBuffer.loadFromFile("sound/death.wav"))
	// 	return -1;
	// Sound death(deathBuffer);

	// Out of time
	// SoundBuffer ootBuffer;
	// if (!ootBuffer.loadFromFile("sound/out_of_time.wav"))
	// 	return -1;
	// Sound outOfTime(ootBuffer);

	// Measure the FPS
	int fps = 0;
	// The main game loop
	while (window.isOpen())
	{
		fps++;
		// Display the FPS
		if (fps >= 100)
		{
			std::stringstream ss;
			ss << "FPS = " << fps;
			fpsText.setString(ss.str());
		}
		oldScore = score;

		/*
		********************************************
		Handle the players input
		********************************************
		*/
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<Event::KeyReleased>() && !paused)
			{
				// Listen for key presses again
				acceptInput = true;

				// hide the axe
				spriteAxe.setPosition({2000,
									   spriteAxe.getPosition().y});
			}
		}
		// Handle Events to avoid the Not responding message of
		// the window
		// while (const std::optional<Event>event = window.pollEvent()) {
		// 	if (event->is<Event::Closed>()) {
		// 		window.close();
		// 	}
		// }
		// Same thing as the commented code above
		// while (const std::optional event = window.pollEvent())
		// 	;
		if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
		{
			window.close();
		}

		// Start the game
		if (Keyboard::isKeyPressed(Keyboard::Key::Enter))
		{
			paused = false;

			// Reset the score and time
			score = 0;
			timeRemaining = 6;

			// Make all the branches disappear
			// starting in the second position
			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			// Make sure the gravestone is hidden
			spriteRIP.setPosition(Vector2f(675.f, 2000.f));

			// Move the player into position
			spritePlayer.setPosition({580.f, 620.f});

			acceptInput = true;
		}

		// Wrap the palyer controls to
		// Make surr we are accepting input
		if (acceptInput)
		{
			// ...

			// First handle pressing the right cursor key
			if (Keyboard::isKeyPressed(Keyboard::Key::Right))
			{
				// Make sure the palyer is on the right
				playerSide = side::RIGHT;
				score++;

				// Add to the amount of time remaining
				timeRemaining += (2 / score) + .15f;

				spriteAxe.setPosition({AXE_POSITION_RIGHT,
									   spriteAxe.getPosition().y});
				spritePlayer.setPosition({1200, 720});

				// Update the branches
				updateBranches(score);

				// Set the log flying to the left
				spriteLog.setPosition(Vector2f(810, 720));
				logSpeedX = -5000;

				logActive = true;

				acceptInput = false;

				// Play a chop sound
				// chop.play();
			}

			// Handle the left cursor key
			if (Keyboard::isKeyPressed(Keyboard::Key::Left))
			{
				// Make sure the player is on the left
				playerSide = side::LEFT;
				score++;

				// Add the amount of time remaining
				timeRemaining += (2 / score) + .15f;
				spriteAxe.setPosition(Vector2f(AXE_POSITION_LEFT,
											   spriteAxe.getPosition().y));
				spritePlayer.setPosition({580, 720});

				// Update the branches
				updateBranches(score);

				// Set the log flying to the left
				spriteLog.setPosition({810, 720});
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;

				// Play a chop sound
				// chop.play();
			}
		}
		/*
		********************************************
		Update the scene
		********************************************
		*/

		if (!paused)
		{

			// measure time
			Time dt = clock.restart();

			// Substract from the amount of time remaining
			timeRemaining -= dt.asSeconds();

			// Resize up the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond *
										 timeRemaining,
									 timeBarHeight));

			if (timeRemaining <= 0.0f)
			{
				// Pause the game
				paused = true;

				// Change the message shown to the player
				messageText.setString("Out of time!!");
				// Reposition the text based on its new size
				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin(Vector2f(
					textRect.position.x + textRect.size.x / 2.0f,
					textRect.position.y + textRect.size.y / 2.0f));

				messageText.setPosition(Vector2f(1920 / 2.0f, 1080 / 2.0f));

				// Play the out of time sound
				// outOfTime.play();
			}
			// Setup the bee
			if (!beeActive)
			{
				// How fast is the bee
				srand((int)time(0));
				beeSpeed = (rand() % 200) + 200;

				// How high is the bee
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition({2000, height});
				beeActive = true;
			}
			else
			{
				// Move the bee
				spriteBee.setPosition({spriteBee.getPosition().x -
										   (beeSpeed * dt.asSeconds()),
									   spriteBee.getPosition().y});

				// Has the bee reached the left-hand edge of the screen?
				if (spriteBee.getPosition().x < -100)
				{
					// Set it up ready to be a whole new bee the next frame
					beeActive = false;
				}
			}

			// Manage the clouds
			// Cloud 1
			if (!cloud1Active)
			{
				// How fast is the cloud
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				// How high is the cloud
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition({-200, height});
				cloud1Active = true;
			}
			else
			{
				spriteCloud1.setPosition({spriteCloud1.getPosition().x +
											  (cloud1Speed * dt.asSeconds()),
										  spriteCloud1.getPosition().y});
				// Has the cloud reached the right hand edge of the screen?
				if (spriteCloud1.getPosition().x > 1920)
				{
					// Set it up ready to be a whole new cloud the next frame
					cloud1Active = false;
				}
			}

			// Cloud 2
			if (!cloud2Active)
			{
				// How fast is the cloud
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				// How high is the cloud
				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition({-200, height});
				cloud2Active = true;
			}
			else
			{
				spriteCloud2.setPosition({spriteCloud2.getPosition().x +
											  (cloud2Speed * dt.asSeconds()),
										  spriteCloud2.getPosition().y});
				// Has the cloud reached the right hand edge of the screen ?
				if (spriteCloud2.getPosition().x > 1920)
				{
					// Set it up ready to be a whole new cloud the next frame
					cloud2Active = false;
				}
			}

			// Cloud 3
			if (!cloud3Active)
			{
				// How fast is the cloud
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);

				// How high is the cloud
				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition({-200, height});
				cloud3Active = true;
			}
			else
			{
				spriteCloud3.setPosition({spriteCloud3.getPosition().x +
											  (cloud3Speed * dt.asSeconds()),
										  spriteCloud3.getPosition().y});
				// Has the cloud reached the right hand edge of the screen
				if (spriteCloud3.getPosition().x > 1920)
				{
					// Set it up ready to be whole new cloud the next frame
					cloud3Active = false;
				}
			}

			// Update the score text
			// In a loop which runs multiple time for exmaple at 1 000 FPS
			// This code cost a lot to the processor to convert
			// an integer or any other type into text or string
			// so to improve the code we can run it occasionaly. Only
			// execute it when the score changed.
			if (score != oldScore)
			{
				std::stringstream ss;
				ss << "Score = " << score;
				scoreText.setString(ss.str());
			}

			// update the branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;

				if (branchPositions[i] == side::LEFT)
				{
					// Move the sprite to the left side
					branches[i].setPosition({610, height});

					// Flip the sprite round the other way
					Angle a = degrees(180.f);
					branches[i].setRotation(a);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					// Move the sprite to the right side
					branches[i].setPosition({1330, height});

					// Set the sprite rotation to normal
					// By default the branch sprite hangs
					// to the right side but we did a rotation of 0 degree
					// by precaution if previously a rotation
					// was applied to the sprite
					branches[i].setRotation(degrees(0));
				}
				else
				{
					// Hide the branch
					// because the side has the NONE value
					branches[i].setPosition({3000, height});
				}
			}

			// Handle a flying log
			if (logActive)
			{
				spriteLog.setPosition({spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
									   spriteLog.getPosition().y + (logSpeedY * dt.asSeconds())});

				// Has the log reached the right hand edge ?
				if (spriteLog.getPosition().x < -100 ||
					spriteLog.getPosition().x > 2000)
				{
					// Set it up ready to be a whole new log next frame
					logActive = false;
					spriteLog.setPosition({810.f, 720.f});
				}
			}

			// has the player been squished by a branch ?
			if (branchPositions[5] == playerSide)
			{

				// death
				paused = true;
				acceptInput = false;
				playerDied = true;

				// Draw the gravestone (525, 760)
				spriteRIP.setPosition({525,
									   760});

				// Change the text of the message to "SQUISHED!!"
				messageText.setString("SQUISHED !!");

				// Center it on the screen
				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin({textRect.position.x + (textRect.size.x / 2.0f),
									   textRect.position.y + (textRect.size.y / 2.f)});

				messageText.setPosition({1920 / 2.f, 1080 / 2.f});

				// Play the death sound
				// death.play();
			}
		} // End !paused
		/*
		********************************************
		Draw the scene
		********************************************
		*/

		// Clear everything from the last scene
		window.clear();

		// Draw our game scene here
		window.draw(spriteBackground);

		// Draw clouds
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		// Draw branches
		// std::cout << "x:" << branches[0].getPosition().x
		// 		  << ", y:" << branches[0].getPosition().y
		// 	      << std::endl;

		for (int i = 0; i < NUM_BRANCHES; i++)
			window.draw(branches[i]);
		// window.draw(tempBranch);

		// Draw the tree
		window.draw(spriteTree);

		// Draw the player
		if (!playerDied)
			window.draw(spritePlayer);

		// Draw the axe
		window.draw(spriteAxe);

		// Draw the flying log
		window.draw(spriteLog);

		// Draw the gravestone
		if (playerDied)
			window.draw(spriteRIP);

		// Draw the bee
		window.draw(spriteBee);

		// Draw our score text
		// along with its rectangle shape behind
		// as background
		window.draw(scoreRectShape);
		window.draw(scoreText);

		// Draw our FPS text\
		using winow.draw()//
		window.draw(fpsRectShape);
		window.draw(fpsText);

		// Draw the time bar
		window.draw(timeBar);

		if (paused)
		{
			// Draw our message text
			window.draw(messageText);
		}

		// Show everything we just drew
		window.display();
	}

	return 0;
}

// Function definition
void updateBranches(int seed)
{
	// Move all the branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}

	// Spawn a new branch at position 0
	// LEFT, RIGHT or NONE
	srand((unsigned int)time(0) + seed);
	int r = rand() % 5;

	switch (r)
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}
}