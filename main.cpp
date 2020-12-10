#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>
#include "Player.h"
#include "Enemy.h"
#include "Game.h"
#include "Menu.h"
#include "block.h"
#include "Animetion.h"


#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<SFML/System.hpp>
#include<SFML/Window.hpp>

using namespace std;

int main()
{
	sf::RenderWindow window(sf::VideoMode(1560, 780), "JusGame", sf::Style::Close| sf::Style::Resize);
	sf::Event ev;

	int stage = 3,ename=0,dead=0;

	Menu menu(window.getSize().x, window.getSize().y);

	while (true)
	{
		//Menu
		if (stage == 0)
		{
			while (true)
			{
				while (window.pollEvent(ev))
				{
					switch (ev.type)
					{
					case sf::Event::KeyReleased:
						switch (ev.key.code)
						{
						case sf::Keyboard::W:
							menu.moveUp();
							break;
						case sf::Keyboard::S:
							menu.moveDown();
							break;
						case sf::Keyboard::Return:
							switch (menu.getPressItem())
							{
							case 0:
								ename = 1;
								break;
							case 1:
								std::cout << "option !!";
								break;
							case 2:
								window.close();
								break;
							}
						}
					}
				}
				window.clear(sf::Color::Black);
				menu.toDraw(window);
				window.display();
				if (ename==1)
				{
					break;
				}
			}
		}
		//Entername
		if (ename == 1)
		{
			ename =0;
			char name[15];
			while (true)
			{
				while (window.pollEvent(ev))
				{
					switch (ev.type)
					{
					case sf::Event::KeyReleased:
						switch (ev.key.code)
						{
						case sf::Keyboard::Enter:
							stage = 1;
							break;
						}
					}
				}
				window.clear(sf::Color::Green);
				window.display();
				if (stage == 1)
				{
					break;
				}
			}
		}
		//Stage
		if (stage == 1)//-----------------------------------------Stage I-----------------------------
		{
			window.setFramerateLimit(60);
			float dt;  //(deltatime)

			//viewSet
			sf::View view,defaultView;
			view.reset(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
			view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
			sf::Vector2f position(window.getSize().x, window.getSize().y);

			//background
			sf::Texture bg;
			bg.loadFromFile("map1r.png");
			sf::RectangleShape bground;
			bground.setSize(sf::Vector2f(12000, 780));
			bground.setTexture(&bg);

			//main music
			sf::Music music;
			music.openFromFile("mainSong.ogg");
			music.setVolume(5.0);
			music.play();

			//skill music
			sf::SoundBuffer soundBang, soundJump;
			soundBang.loadFromFile("bang.wav");
			soundJump.loadFromFile("musJump.wav");
			sf::Sound sBang, sJump;
			sBang.setBuffer(soundBang);
			sJump.setBuffer(soundJump);
			sBang.setVolume(30.0);
			sJump.setVolume(30.0);

			//upperText
			sf::Font font;
			font.loadFromFile("BAUHS93.TTF");

			ostringstream showStage;
			showStage << "Stage 1";
			sf::Text lblStage;
			lblStage.setCharacterSize(30);
			lblStage.setPosition({ 1420,10 });
			lblStage.setFont(font);
			lblStage.setString(showStage.str());

			int f2sec = 0, f1min = 0, f2min = 0;
			sf::Clock clock;
			sf::Time elapsed = clock.getElapsedTime();
			ostringstream showTime;

			sf::Text lblTime;
			lblTime.setCharacterSize(30);
			lblTime.setPosition({ 1420,50 });
			lblTime.setFont(font);

			//Mana+Hp
			ostringstream pickmana;
			ostringstream pickheart;

			sf::Texture h6, m4;
			h6.loadFromFile("h6.png");
			sf::Sprite heart;
			heart.setTexture(h6);
			heart.setPosition(0, 0);

			m4.loadFromFile("m4.png");
			sf::Sprite mana;
			mana.setTexture(m4);
			mana.setPosition(0, 50);

			//BackGroundAnimation
			sf::Texture heli1, heli2, texrope, tr;
			heli1.loadFromFile("helicopterRanimation.png");
			heli2.loadFromFile("helicopterLanimation.png");
			texrope.loadFromFile("rope.png");
			tr.loadFromFile("tr.png");
			sf::Sprite heliR, heliL[2], rope, truck,truck2;
			heliR.setTexture(heli1);
			heliL[0].setTexture(heli2);
			heliL[0].setPosition(7472.0, 15.0);
			heliL[1].setTexture(heli2);
			heliL[1].setPosition(8778, 15.0);
			heliR.setPosition(11567.0, 44.0);
			Animetion helicopR(&heli1, sf::Vector2u(1, 4), 0.05f);
			Animetion helicopL(&heli2, sf::Vector2u(1, 4), 0.05f);
			rope.setTexture(texrope);
			rope.setPosition(11666.0, 162.0);
			truck.setTexture(tr);
			truck2.setTexture(tr);
			truck.setPosition(5368, 484);
			truck2.setPosition(9850,484);



			//Clock_use
			sf::Clock co1; //Lswitch
			sf::Clock co2; //Jswitch
			sf::Clock co3; //Ulti
			sf::Clock co4; //jumpBlock
			sf::Clock dtclock; //deltaTime
			sf::Clock cmanaClock; //manaGain







			//PlayerObject
			Player player({ 84.0,112.0 });
			player.setOrigin();
			player.setPos({ 60.0,664.0 });
			sf::Vector2f playerHalf = player.getSize() / 2.0f;

			//item
			srand(time(0));
			int randomItem=rand()%2;
			Item item;
			if (randomItem == 1)
			{
				item.setPOs({ 1300.0,380.0 });
			}
			else if (randomItem == 2)
			{
				item.setPOs({1300.0,100});
			}
			else 
			{
				item.setPOs({1300.0,600.0});
			}

			//skillPlayer
			Skill skilli[12], skillf[12], skillu[12];
			for (int i = 1; i <= 11; i++)
			{
				skilli[i].setColor(sf::Color::Blue);
				skillf[i].setColor(sf::Color::Red);
				skillu[i].setColor(sf::Color(128, 0, 128));
			}

			//enemy
			Enemy enemy[12];
			//White
			enemy[0].set0();
			enemy[0].setPos({ 2452,670 });
			enemy[1].set0();
			enemy[1].setPos({ 9350,670 });
			//Blue
			enemy[2].set1();
			enemy[2].setPos({ 4360,670 });
			enemy[3].set1();
			enemy[3].setPos({ 4680,670 });
			enemy[4].set1();
			enemy[4].setPos({ 8590, 285 });
			//Red
			enemy[5].set2();
			enemy[5].setPos({ 3115,670 });
			enemy[6].set2();
			enemy[6].setPos({ 4515,670 });
			enemy[7].set2();
			enemy[7].setPos({ 9120,670 });
			enemy[8].set2();
			enemy[8].setPos({ 11100,670 });
			//Violet
			enemy[9].set3();
			enemy[9].setPos({ 5693,590 });
			enemy[10].set3();
			enemy[10].setPos({ 9560,670 });
			enemy[11].set3();
			enemy[11].setPos({ 10240,590 });


			Block b1;
			vector<Block> blocks;

			sf::Vector2f pos[26], siz[26];
			pos[0] = { 5373 ,511 };			//truck
			pos[1] = { 5531 ,482 };
			pos[2] = { 5373 + 4478,511 };
			pos[3] = { 5531 + 4478,482 };
			pos[4] = { 1543.0,640.5 };		//singCar
			pos[5] = { 1636,605 };
			pos[6] = { 1767.5,652.5 };
			pos[7] = { 1770.5,664.5 };
			pos[8] = { 1543.0 + 3546,637 };
			pos[9] = { 5181.5,600.5 };
			pos[10] = { 3407,559 };			//doubCar
			pos[11] = { 3506,521 };
			pos[12] = { 3638.5,567.5 };
			pos[13] = { 3642,584 };
			pos[14] = { 3417 + 3749,559 };
			pos[15] = { 3506 + 3749,521 };
			pos[16] = { 7391,569 };
			pos[17] = { 7393,590 };
			pos[18] = { 7639.5,330 };		//box
			pos[19] = { 6777,627 };
			pos[20] = { 9745,613 };
			pos[21] = { 7540,127 };			//helicopter
			pos[22] = { 7717,27 };
			pos[23] = { 8860,24 };
			pos[24] = { 9037,93 };
			pos[25] = { 11666.0,620 };		//finish

			siz[0] = { 158,216 };			//truck
			siz[1] = { 428,250 };
			siz[2] = { 158,216 };
			siz[3] = { 428,250 };
			siz[4] = { 234.5,93.0 };		//singCar
			siz[5] = { 16,41 };
			siz[6] = { 22,23 };
			siz[7] = { 38,69 };
			siz[8] = { 284,93 };
			siz[9] = { 16,41 };
			siz[10] = { 243,175 };			//doubCar
			siz[11] = { 16,41 };
			siz[12] = { 22,23 };
			siz[13] = { 35,150 };
			siz[14] = { 244,175 };
			siz[15] = { 16,41 };
			siz[16] = { 25,22 };
			siz[17] = { 38,139 };
			siz[18] = { 1433.0,31.0 };		//box
			siz[19] = { 103.0,103.0 };
			siz[20] = { 103.0,103.0 };
			siz[21] = { 190,25 };			//helicopter
			siz[22] = { 170,104 };
			siz[23] = { 190,124 };
			siz[24] = { 145,32 };
			siz[25] = { 168,23 };			//finish

			sf::RectangleShape woodh;
			sf::Texture woodd;
			woodd.loadFromFile("woodd.png");
			woodh.setTexture(&woodd);
			woodh.setSize(sf::Vector2f(1442.0f, 38.0f));
			woodh.setPosition(7640, 330);


			int size = 0;

			//variableSwitch
			int isMove = 0, isJump = 0, ulti = 0, faceRight = 1;

			//variableSkill
			int change = 0, isSkilli[12], isSkillf[12], isSkillu[12], scount = 0, sdir[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
			int cmana = 4, manax = 4;

			//variablePlayerDoing
			float speed = 600.0f, Yspeed, retard = 25.0f, fallspeed = 0, Rspeed = 600.0f, Lspeed = 600.0f;
			int  onGround = 1, groundHeigh = 662, boxx = 0;

			//Heart
			int cheart = 6, hert = 0, hertspeed = 0;

			//block
			float dx, dy;
			float intersectX, intersectY;
			int finish=0;

			//enemy
			int loopEnemy = 0, ebus2 = 0, dashgo1 = 0, dashgo2 = 0,jumppoint1=0,jumppoint2=0,downpoint1=0,downpoint2=0;


			//------------------------------------------------------IN Game stage I------------------------
			while (window.isOpen())
			{
				dt = dtclock.restart().asSeconds();
				while (window.pollEvent(ev))
				{
					switch (ev.type)
					{
					case sf::Event::Closed:
						window.close();
						break;
					case sf::Event::KeyPressed:
						if (ev.key.code == sf::Keyboard::Escape)
							window.close();
						break;
					}
				}
				//RunTime
				elapsed = clock.getElapsedTime();
				showTime << f2min << f1min << ":" << f2sec << (int)elapsed.asSeconds() << ":" << elapsed.asMilliseconds() % 1000 / 10;
				if (elapsed.asSeconds() >= 10)
				{
					clock.restart();
					f2sec += 1;
				}
				if (f2sec >= 6)
				{
					f1min += 1;
					f2sec = 0;
				}
				if (f2min >= 10)
				{
					f1min += 1;
					f2min = 0;
				}
				lblTime.setString(showTime.str());
				showTime.str("");

				//Cooldown
				sf::Time Lcool = co1.getElapsedTime();
				sf::Time Jcool = co2.getElapsedTime();
				sf::Time Utime = co3.getElapsedTime();
				sf::Time JumpCool = co4.getElapsedTime();
				sf::Time gainMana = cmanaClock.getElapsedTime();



				//controlKey
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				{
					if (onGround == 1)
					{
						sJump.play();
						isJump = 1;
						onGround = 0;
						Yspeed = -750.0f;
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
				{
					Lspeed = 600.0f;
					faceRight = -1;
					isMove = -1;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
				{
					Rspeed = 600.0f;
					faceRight = 1;
					isMove = 1;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L))
				{
					if (change == 0 && (Lcool.asSeconds() >= 0.2))
					{
						change = 1;
						co1.restart();
					}
					else if (change == 1 && (Lcool.asSeconds() >= 0.2))
					{
						change = 0;
						co1.restart();
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J) && (Jcool.asSeconds() >= 0.2))
				{
					if (cmana > 0)
					{
						cmana -= 1;
						pickmana << "m" << cmana << ".png";
						m4.loadFromFile(pickmana.str());
						pickmana.str("");
						mana.setTexture(m4);

						co2.restart();
						sBang.play();

						//ReDirect+Posi Skill
						scount += 1;
						if (scount == 12)
						{
							for (int i = 1; i <= 11; i++)
							{
								sdir[i] = 0;
								skilli[i].setPos({ -50,-50 });
								skillf[i].setPos({ -50,-50 });
								skillu[i].setPos({ -50,-50 });
							}
							scount = 1;
						}

						if (ulti == 1)
						{
							skillu[scount].setPos({ player.getX() , player.getY() - 5 });
							isSkillu[scount] = 1;
						}
						else if (change == 0)
						{
							skilli[scount].setPos({ player.getX() , player.getY() - 5 });
							isSkilli[scount] = 1;
						}
						else
						{
							skillf[scount].setPos({ player.getX() , player.getY() - 5 });
							isSkillf[scount] = 1;
						}
					}
				}

				//Cant move LimitMap
				if (player.getX() - player.getSize().x / 2.0f <= 0)
				{
					Lspeed = 0;
				}
				if (player.getX() + player.getSize().x / 2.0f >= 11999)
				{
					Rspeed = 0;
				}

				//Mana_Charge & Use
				if (cmana == manax)
				{
					cmanaClock.restart();
				}
				if (gainMana.asSeconds() >= 1)
				{
					if (cmana < manax)
					{
						cmana += 1;
						pickmana << "m" << cmana << ".png";
						m4.loadFromFile(pickmana.str());
						pickmana.str("");
						mana.setTexture(m4);
						cmanaClock.restart();
					}
				}




				//BlockSet;
				if (size < 26)
				{
					b1.set({ pos[size] }, { siz[size] });
					blocks.push_back(b1);
					size++;
				}



				//EnemyCollide_Player
				if (enemy[loopEnemy].getGlobalBounds().intersects(player.getGlobalBounds()))
				{
					if (loopEnemy <= 8)
					{
						cheart -= 1;
						pickheart << "h" << cheart << ".png";
						h6.loadFromFile(pickheart.str());
						pickheart.str("");
					}
					else
					{
						cheart -= 2;
						pickheart << "h" << cheart << ".png";
						h6.loadFromFile(pickheart.str());
						pickheart.str("");
					}
					if (player.getX() - enemy[loopEnemy].getPos().x <= 0)
						hert = -1;
					else
						hert = 1;

					hertspeed = 1000.0f;
				}

				//EnemyPush
				loopEnemy += 1;
				if (loopEnemy == 12)
				{
					loopEnemy = 0;
				}
				if (enemy[loopEnemy].getGlobalBounds().intersects(player.getGlobalBounds()))
				{
					if (loopEnemy <= 8)
					{
						cheart -= 1;
						pickheart << "h" << cheart << ".png";
						h6.loadFromFile(pickheart.str());
						pickheart.str("");
					}
					else
					{
						cheart -= 2;
						pickheart << "h" << cheart << ".png";
						h6.loadFromFile(pickheart.str());
						pickheart.str("");
					}
					if (player.getX() - enemy[loopEnemy].getPos().x <= 0)
						hert = -1;
					else
						hert = 1;

					hertspeed = 1000.0f;
				}

				//EnemyPush
				loopEnemy += 1;
				if (loopEnemy == 12)
				{
					loopEnemy = 0;
				}
				if (hert == -1 && hertspeed >= 0)
				{
					player.moveLeft(hertspeed * dt);
					hertspeed -= 35.0f;
					Rspeed = 0;
					Lspeed = 0;
				}
				if (hert == 1 && hertspeed >= 0)
				{
					player.moveRight(hertspeed * dt);
					hertspeed -= 35.0f;
					Lspeed = 0;
					Rspeed = 0;
				}



				//CheckBlock_Collide
				for (size_t i = 0; i < blocks.size(); i++)
				{
					if (player.getGlobalBounds().intersects(blocks[i].getGlobalBounds()))
					{
						dx = player.getX() - blocks[i].getPos().x;
						dy = player.getY() - blocks[i].getPos().y;
						float intersectX = abs(dx) - (playerHalf.x + blocks[i].getSize().x / 2.0f);
						float intersectY = abs(dy) - (playerHalf.y + blocks[i].getSize().y / 2.0f);
						if (intersectX > intersectY)
						{
							if (dx > 0.0f)
							{
								Lspeed = 0;
								hertspeed = -1;
							}
							else
							{
								Rspeed = 0;
								hertspeed = -1;
							}
						}
						else
						{
							if (dy < 0.0f)
							{
								isJump = 0;
								onGround = 1;
								Yspeed = -25.0f;
								if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
								{
									if (onGround == 1)
									{
										sJump.play();
										isJump = 1;
										onGround = 0;
										Yspeed = -700.0f;
									}
								}
								if (i == 25)
								{
									finish = 1;
								}
							}
							else
							{
								Yspeed = 50.0f;
								isJump = 0;
							}
						}
					}
					for (int j = 1; j <= scount; j++)
					{
						if (skillf[j].getGlobalBounds().intersects(blocks[i].getGlobalBounds()))
						{
							skillf[j].setPos({ -50,-50 });
						}
						if (skilli[j].getGlobalBounds().intersects(blocks[i].getGlobalBounds()))
						{
							skilli[j].setPos({ -50,-50 });
						}
						if (skillu[j].getGlobalBounds().intersects(blocks[i].getGlobalBounds()))
						{
							skillu[j].setPos({ -50,-50 });
						}
					}
				}



				//checkSkill_vsEnemys
				for (int j = 1; j <= scount; j++)
				{
					//white
					for (int i = 0; i <= 1; i++)
					{
						if (skillf[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skillf[j].setPos({ -50,-50 });
						}
						if (skilli[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skilli[j].setPos({ -50,-50 });
						}
						if (skillu[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skillu[j].setPos({ -50,-50 });
						}
					}
					//blue
					for (int i = 2; i <= 4; i++)
					{
						if (skilli[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skilli[j].setPos({ -50,-50 });
						}
						if (skillu[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skillu[j].setPos({ -50,-50 });
						}
					}
					//red
					for (int i = 5; i <= 8; i++)
					{
						if (skillf[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skillf[j].setPos({ -50,-50 });
						}
						if (skillu[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skillu[j].setPos({ -50,-50 });
						}
					}
					//violet
					for (int i = 9; i <= 11; i++)
					{
						if (skillu[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skillu[j].setPos({ -50,-50 });
						}
					}
					for (int i = 1; i <= 11; i++)
					{
						if (skillf[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							skillf[j].setPos({ -50,-50 });
						}
						if (skilli[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							skilli[j].setPos({ -50,-50 });
						}
					}
				}

				enemy[0].Pattern0(dt);

				enemy[4].Pattern1(dt, 8390, 8790);
				enemy[5].Pattern1(dt, 2915, 3355);
				enemy[8].Pattern1(dt, 10900, 11300);

				//Buss
				enemy[2].Pattern1(dt, 3760, 4660);
				enemy[6].Pattern1(dt, 3915, 4815);
				enemy[3].Pattern1(dt, 4080, 4980);


				if (player.getX() >= 7650)	ebus2 = 1;
				if (ebus2 == 1)
				{
					enemy[7].Pattern1(dt, 7720, 9170);
					enemy[1].Pattern1(dt, 7950, 9400);
					enemy[10].Pattern1(dt, 8160, 9610);
				}

				//Dash
				if (player.getX() >= 6300)
				{
					dashgo1 = 1;
				}
				if (enemy[9].getPos().x >= 6600)
				{
					jumppoint1 = 1;
				}
				if (enemy[9].getPos().x >= 6050)
				{
					downpoint1 = 1;
				}
				enemy[9].Pattern2(dt, dashgo1, jumppoint1, downpoint1);
					
				/*
				if (player.getX() >= 6500)
				{
					dashgo2 = 1;
				}
				if (enemy[11].getPos().x >= 6600)
				{
					jumppoint2 = 1;
				}
				if (enemy[11].getPos().x >= 6050)
				{
					downpoint2 = 1;
				}
				enemy[11].Pattern2(dt, dashgo2, jumppoint2, downpoint2);*/




				//FinishStage
				if (finish==1)
				{
					heliR.move(100.0f * dt, 0.0f);
					isMove = 0;
					isJump = 0;
					Yspeed = 0;
					Yspeed -= 25.0f;
					player.moveRight(100.0f * dt);
					rope.move(100.0f * dt, 0.0f);
				}
				if (player.getX() >= 12200)
				{
					stage = 2;
					break;
				}
				
				//Dead
				if (cheart<=0)
				{
					defaultView.reset(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
					window.setView(defaultView);
					dead=1;
					break;
				}
			





				//playerDoing

				if (isMove == 1)
				{
					player.moveRight(Rspeed * dt);
					isMove = 0;
				}
				if (isMove == -1)
				{
					player.moveLeft(Lspeed * dt);
					isMove = 0;
				}
				if (isJump == 1 && onGround == 0)
				{
					player.moveY(Yspeed * dt);
					Yspeed += retard;

					if (player.getY() >= groundHeigh)
					{
						onGround = 1;
						isJump = 0;
					}
				}
				if (isJump == 0 && player.getY() < groundHeigh)
				{
					Yspeed += 25.0f;
					player.moveY(Yspeed * dt);
					if (player.getY() >= groundHeigh - 1)
					{
						onGround = 1;
					}
				}




				//skillDirect

				for (int i = 1; i <= scount; i++)
				{
					if (isSkillu[i] == 1)
					{
						if (sdir[i] == 0)
						{
							sdir[i] = faceRight;
						}
						if (sdir[i] == 1)
						{
							skillu[i].move((speed + 500) * dt);
						}
						if (sdir[i] == -1)
						{
							skillu[i].move(-(speed + 500) * dt);
						}
					}
					if (isSkilli[i] == 1)
					{
						if (sdir[i] == 0)
						{
							sdir[i] = faceRight;
						}
						if (sdir[i] == 1)
						{
							skilli[i].move((speed + 500) * dt);
						}
						if (sdir[i] == -1)
						{
							skilli[i].move(-(speed + 500) * dt);
						}

					}
					if (isSkillf[i] == 1)
					{
						if (sdir[i] == 0)
						{
							sdir[i] = faceRight;
						}
						if (sdir[i] == 1)
						{
							skillf[i].move((speed + 500) * dt);
						}
						if (sdir[i] == -1)
						{
							skillf[i].move(-(speed + 500) * dt);
						}

					}
				}

				//ItemCollect
				if (player.getGlobalBounds().intersects(item.getGlobalBounds()))
				{
					co3.restart();
					item.setPOs({ -50.0f,-50.0f });
					ulti = 1;
				}
				//Ultimate
				if (Utime.asSeconds() >= 10)
				{
					ulti = 0;
					co3.restart();
				}











				//viewUpdate

				position.x = player.getX() + 300 - (window.getSize().x / 2.0f);
				position.y = player.getY()  - (window.getSize().y / 1.0f);
				if (position.x < 0)
				{
					position.x = 0;
				}
				if (position.x >= 10439)
				{
					position.x = 10440;
				}
				if (position.y < 0)
				{
					position.y = 0;
				}
				view.reset(sf::FloatRect(position.x, position.y, window.getSize().x, window.getSize().y));
				lblStage.setPosition({ position.x + 1420.0f,10.0f });
				lblTime.setPosition({ position.x + 1420.0f,50.0f });
				heart.setPosition(10 + position.x, 10);
				mana.setPosition(19 + position.x, 57);

				//ToDraw
				window.clear();
				window.setView(view);
				window.draw(bground);


				window.draw(rope);
				helicopR.Update(0, dt);
				heliR.setTextureRect(helicopR.uvRect);
				window.draw(heliR);
				helicopL.Update(0, dt);
				heliL[0].setTextureRect(helicopL.uvRect);
				heliL[1].setTextureRect(helicopL.uvRect);
				window.draw(heliL[0]);
				window.draw(heliL[1]);
				

				//PlatformShow
				/*for (size_t i = 0; i < blocks.size(); i++)
				{
					window.draw(blocks[i].body);
				}*/
				window.draw(woodh);


				for (int i = 1; i <= scount; i++)
				{
					skilli[i].toDraw(window);
					skillf[i].toDraw(window);
					skillu[i].toDraw(window);
				}
				for (int i = 0; i < 12; i++)
				{
					enemy[i].toDraw(window);
				}
				window.draw(truck);
				window.draw(truck2);

				window.draw(heart);
				window.draw(mana);
				window.draw(lblStage);
				window.draw(lblTime);




				item.toDraw(window);

				if (faceRight == 1)
				{
					player.toDrawR(window);
				}
				else
				{
					player.toDrawL(window);
				}
				//Draw
				window.display();

			}
		}
		if (stage == 2)
		{
			window.setFramerateLimit(60);
			float dt;  //(deltatime)

			//viewSet
			sf::View view, defaultView;
			view.reset(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
			view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
			sf::Vector2f position(window.getSize().x, window.getSize().y);

			//background
			sf::Texture bg;
			bg.loadFromFile("map2r.png");
			sf::RectangleShape bground;
			bground.setSize(sf::Vector2f(15000, 780));
			bground.setTexture(&bg);

			//main music
			sf::Music music;
			music.openFromFile("mainSong.ogg");
			music.setVolume(5.0);
			music.play();

			//skill music
			sf::SoundBuffer soundBang, soundJump;
			soundBang.loadFromFile("bang.wav");
			soundJump.loadFromFile("musJump.wav");
			sf::Sound sBang, sJump;
			sBang.setBuffer(soundBang);
			sJump.setBuffer(soundJump);
			sBang.setVolume(30.0);
			sJump.setVolume(30.0);

			//upperText
			sf::Font font;
			font.loadFromFile("BAUHS93.TTF");

			ostringstream showStage;
			showStage << "Stage 2";
			sf::Text lblStage;
			lblStage.setCharacterSize(30);
			lblStage.setPosition({ 1420,10 });
			lblStage.setFont(font);
			lblStage.setString(showStage.str());

			int f2sec = 0, f1min = 0, f2min = 0;
			sf::Clock clock;
			sf::Time elapsed = clock.getElapsedTime();
			ostringstream showTime;

			sf::Text lblTime;
			lblTime.setCharacterSize(30);
			lblTime.setPosition({ 1420,50 });
			lblTime.setFont(font);

			//Mana+Hp
			ostringstream pickmana;
			ostringstream pickheart;

			sf::Texture h6, m4;
			h6.loadFromFile("h6.png");
			sf::Sprite heart;
			heart.setTexture(h6);
			heart.setPosition(0, 0);

			m4.loadFromFile("m4.png");
			sf::Sprite mana;
			mana.setTexture(m4);
			mana.setPosition(0, 50);

			//BackGroundAnimation
			sf::Texture heli1, heli2, texrope, tr, tr2;
			heli1.loadFromFile("helicopterRanimation.png");
			heli2.loadFromFile("helicopterLanimation.png");
			texrope.loadFromFile("ropes.png");
			tr2.loadFromFile("tr2.png");
			tr.loadFromFile("tr.png");
			sf::Sprite heliR[2], heliL[2], rope, truck, truck2;
			heliL[0].setTexture(heli2);
			heliL[1].setTexture(heli2);
			heliL[0].setPosition(3145.0, 15.0);
			heliL[1].setPosition(4199, 15.0);
			heliR[0].setTexture(heli1);
			heliR[0].setPosition(2434.0, 65.0);
			heliR[1].setTexture(heli1);
			heliR[1].setPosition(14511.0, 3.0);

			Animetion helicopR(&heli1, sf::Vector2u(1, 4), 0.05f);
			Animetion helicopL(&heli2, sf::Vector2u(1, 4), 0.05f);
			rope.setTexture(texrope);
			rope.setPosition(14615.0, 130.0);
			truck.setTexture(tr);
			truck2.setTexture(tr2);
			truck.setPosition(1248, 484);
			truck2.setPosition(13605, 484);

			sf::RectangleShape hrope1, hrope2, hrope3;
			hrope1.setSize({ 3,240 });
			hrope1.setFillColor(sf::Color::White);
			hrope1.setPosition(3376, 140);
			hrope2.setSize({ 3,240 });
			hrope2.setFillColor(sf::Color::White);
			hrope2.setPosition(4423, 140);
			hrope3.setSize({ 3,300 });
			hrope3.setFillColor(sf::Color::White);
			hrope3.setPosition(2623, 180);



			//Clock_use
			sf::Clock co1; //Lswitch
			sf::Clock co2; //Jswitch
			sf::Clock co3; //Ulti
			sf::Clock co4; //jumpBlock
			sf::Clock dtclock; //deltaTime
			sf::Clock cmanaClock; //manaGain







			//PlayerObject
			Player player({ 84.0,112.0 });
			player.setOrigin();
			player.setPos({ 100.0,664.0 });
			sf::Vector2f playerHalf = player.getSize() / 2.0f;

			//item
			srand(time(0));
			int randomItem = rand() % 3;
			Item item;
			if (randomItem == 1)
			{
				item.setPOs({ 1300.0,380.0 });
			}
			else if (randomItem == 2)
			{
				item.setPOs({ 1300.0,100 });
			}
			else
			{
				item.setPOs({ 1300.0,600.0 });
			}

			//skillPlayer
			Skill skilli[12], skillf[12], skillu[12];
			for (int i = 1; i <= 11; i++)
			{
				skilli[i].setColor(sf::Color::Blue);
				skillf[i].setColor(sf::Color::Red);
				skillu[i].setColor(sf::Color(128, 0, 128));
			}

			//enemy
			Enemy enemy[21];
			//White
			enemy[0].set0();
			enemy[0].setPos({ 6256,380 });
			enemy[1].set0();
			enemy[1].setPos({ 8025,670 });
			enemy[2].set0();
			enemy[2].setPos({10200,670 });
			enemy[3].set0();
			enemy[3].setPos({ 11094,510 });
			//Blue
			enemy[4].set1();
			enemy[4].setPos({ 3898,670 });
			enemy[5].set1();
			enemy[5].setPos({ 4070,670 });
			enemy[6].set1();
			enemy[6].setPos({ 5727, 440 });
			enemy[7].set1();
			enemy[7].setPos({ 8798,670 });
			enemy[8].set1();
			enemy[8].setPos({ 11288,510 });
			enemy[9].set1();
			enemy[9].setPos({ 12037, 670 });
			//Red
			enemy[10].set2();
			enemy[10].setPos({ 2391,420 });
			enemy[11].set2();
			enemy[11].setPos({ 3726,670 });
			enemy[12].set2();
			enemy[12].setPos({ 3910,328 });
			enemy[13].set2();
			enemy[13].setPos({ 6775,280 });
			enemy[14].set2();
			enemy[14].setPos({ 9469,670 });
			enemy[15].set2();
			enemy[15].setPos({ 10900,510 });
			enemy[16].set2();
			enemy[16].setPos({ 11475,400 });
			enemy[17].set2();
			enemy[17].setPos({ 11865,670 });
			//Violet
			enemy[18].set3();
			enemy[18].setPos({ 1542,590 });
			enemy[19].set3();
			enemy[19].setPos({ 13691,590 });
			enemy[20].set3();
			enemy[20].setPos({ 16000,670 });


			Block b1;
			vector<Block> blocks;

			sf::Vector2f pos[50], siz[50];
			pos[0] = { 1251.5 ,498.5 };			//truck
			pos[1] = { 1409.5,477.5 };
			pos[2] = { 5072.5,506.5 };
			pos[3] = { 5230.5,477.5 };
			pos[4] = { 6103.5,506.5 };
			pos[5] = { 5663.5,500.5 };
			pos[6] = { 14038.5,506.5 };
			pos[7] = { 13610.5,481.5 };

			pos[8] = { 967,637 };		//singCar
			pos[9] = { 1063.5,599.5 };
			pos[10] = { 6743 ,637.5 };
			pos[11] = { 6838 ,602 };
			pos[12] = { 6972 ,645 };
			pos[13] = { 6976 ,659 };
			pos[14] = { 8413,637 };		
			pos[15] = { 8508,602 };
			pos[16] = { 8642,645 };
			pos[17] = { 8646,659 }; 
			pos[18] = { 8403+729,637 };
			pos[19] = { 8498+729,602 };
			pos[20] = { 8632+729,645 };
			pos[21] = { 8636 + 729,659 };
			pos[22] = { 8403 + 1441,637 };
			pos[23] = { 8498 + 1441,602 };
			pos[24] = { 8632 + 1441,645 };
			pos[25] = { 8636 + 1441,659 };
			pos[26] = { 8403 + 2153,637 };
			pos[27] = { 8498 + 2153,602 };
			pos[28] = { 8632 + 2153,645 };
			pos[29] = { 8636 + 2153,659 };
			pos[30] = { 7633,544 };			//doubCar
			pos[31] = { 7728,502 };
			pos[32] = { 7875,569 };
			pos[33] = { 4968,617 };		//box
			pos[34] = { 12316,629 };
			pos[35] = { 12410,629 };
			pos[36] = { 12410,539 };
			pos[37] = { 13495,614 };
			pos[38] = { 14214,640 };
			pos[39] = { 1741,460 };
			pos[40] = { 3210,370 };
			pos[41] = { 2434,144 };			//helicopter
			pos[42] = { 2592,86 };
			pos[43] = { 3232,136 };
			pos[44] = { 3390,44 };
			pos[45] = { 4275,45 };
			pos[46] = { 4452,100 };
			pos[47] = { 14614.0,385 };		//finish



			siz[0] = { 158,216 };			//truck
			siz[1] = { 428,250 };
			siz[2] = { 158,216 };
			siz[3] = { 876,250 };
			siz[4] = { 158,216 };			
			siz[5] = { 1,1 };
			siz[6] = { 158,216 };
			siz[7] = { 428,250 };
			siz[8] = { 284,93 };			//singCar
			siz[9] = { 16,41 };
			siz[10] = { 234.5,93.0 };		
			siz[11] = { 16,41 };
			siz[12] = { 22,23 };
			siz[13] = { 38,69 };
			siz[14] = { 234.5,93.0 };
			siz[15] = { 16,41 };
			siz[16] = { 22,23 };
			siz[17] = { 38,69 };
			siz[18] = { 234.5,93.0 };
			siz[19] = { 16,41 };
			siz[20] = { 22,23 };
			siz[21] = { 38,69 };
			siz[22] = { 234.5,93.0 };
			siz[23] = { 16,41 };
			siz[24] = { 22,23 };
			siz[25] = { 38,69 };
			siz[26] = { 234.5,93.0 };
			siz[27] = { 16,41 };
			siz[28] = { 22,23 };
			siz[29] = { 38,69 };
			siz[30] = { 243,175 };			//doubCar
			siz[31] = { 16,41 };
			siz[32] = { 35,150 };
			siz[33] = { 103.0,103.0 };		//box
			siz[34] = { 103.0,103.0 };
			siz[35] = { 103.0,103.0 };
			siz[36] = { 103.0,103.0 };
			siz[37] = { 103.0,103.0 };
			siz[38] = { 78.0,78.0 };
			siz[39] = { 1107.0,25.0 };
			siz[40] = { 1330.0,25.0 };
			siz[41] = { 190,25 };			//helicopter
			siz[42] = { 190,124 };
			siz[43] = { 190,25 };
			siz[44] = { 170,104 };
			siz[45] = { 190,124 };
			siz[46] = { 145,32 };
			siz[47] = { 168,23 };			//finish

			sf::RectangleShape woodh,woodh2;
			sf::Texture woodd;
			woodd.loadFromFile("woodd.png");
			woodh.setTexture(&woodd);
			woodh2.setTexture(&woodd);
			woodh.setSize(sf::Vector2f(1108.0f, 25.0f));
			woodh.setPosition(1741, 463);
			woodh2.setSize(sf::Vector2f(1333.0f, 25.0f));
			woodh2.setPosition(3210, 370);


			int size = 0;

			//variableSwitch
			int isMove = 0, isJump = 0, ulti = 0, faceRight = 1;

			//variableSkill
			int change = 0, isSkilli[12], isSkillf[12], isSkillu[12], scount = 0, sdir[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
			int cmana = 4, manax = 4;

			//variablePlayerDoing
			float speed = 600.0f, Yspeed, retard = 25.0f, fallspeed = 0, Rspeed = 600.0f, Lspeed = 600.0f;
			int  onGround = 1, groundHeigh = 662, boxx = 0;

			//Heart
			int cheart = 6, hert = 0, hertspeed = 0;

			//block
			float dx, dy;
			float intersectX, intersectY;
			int finish = 0;

			//enemy
			int loopEnemy = 0, ebus2 = 0, dashgo1 = 0, dashgo2 = 0, jumppoint1 = 0, jumppoint2 = 0, downpoint1 = 0, downpoint2 = 0;


			//------------------------------------------------------IN Game stage II------------------------
			while (window.isOpen())
			{
				dt = dtclock.restart().asSeconds();
				while (window.pollEvent(ev))
				{
					switch (ev.type)
					{
					case sf::Event::Closed:
						window.close();
						break;
					case sf::Event::KeyPressed:
						if (ev.key.code == sf::Keyboard::Escape)
							window.close();
						break;
					}
				}
				//RunTime
				elapsed = clock.getElapsedTime();
				showTime << f2min << f1min << ":" << f2sec << (int)elapsed.asSeconds() << ":" << elapsed.asMilliseconds() % 1000 / 10;
				if (elapsed.asSeconds() >= 10)
				{
					clock.restart();
					f2sec += 1;
				}
				if (f2sec >= 6)
				{
					f1min += 1;
					f2sec = 0;
				}
				if (f2min >= 10)
				{
					f1min += 1;
					f2min = 0;
				}
				lblTime.setString(showTime.str());
				showTime.str("");

				//Cooldown
				sf::Time Lcool = co1.getElapsedTime();
				sf::Time Jcool = co2.getElapsedTime();
				sf::Time Utime = co3.getElapsedTime();
				sf::Time JumpCool = co4.getElapsedTime();
				sf::Time gainMana = cmanaClock.getElapsedTime();



				//controlKey
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				{
					if (onGround == 1)
					{
						sJump.play();
						isJump = 1;
						onGround = 0;
						Yspeed = -700.0f;
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
				{
					Lspeed = 600.0f;
					faceRight = -1;
					isMove = -1;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
				{
					Rspeed = 600.0f;
					faceRight = 1;
					isMove = 1;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L))
				{
					if (change == 0 && (Lcool.asSeconds() >= 0.2))
					{
						change = 1;
						co1.restart();
					}
					else if (change == 1 && (Lcool.asSeconds() >= 0.2))
					{
						change = 0;
						co1.restart();
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J) && (Jcool.asSeconds() >= 0.2))
				{
					if (cmana > 0)
					{
						cmana -= 1;
						pickmana << "m" << cmana << ".png";
						m4.loadFromFile(pickmana.str());
						pickmana.str("");
						mana.setTexture(m4);

						co2.restart();
						sBang.play();

						//ReDirect+Posi Skill
						scount += 1;
						if (scount == 12)
						{
							for (int i = 1; i <= 11; i++)
							{
								sdir[i] = 0;
								skilli[i].setPos({ -50,-50 });
								skillf[i].setPos({ -50,-50 });
								skillu[i].setPos({ -50,-50 });
							}
							scount = 1;
						}

						if (ulti == 1)
						{
							skillu[scount].setPos({ player.getX() , player.getY() - 5 });
							isSkillu[scount] = 1;
						}
						else if (change == 0)
						{
							skilli[scount].setPos({ player.getX() , player.getY() - 5 });
							isSkilli[scount] = 1;
						}
						else
						{
							skillf[scount].setPos({ player.getX() , player.getY() - 5 });
							isSkillf[scount] = 1;
						}
					}
				}

				//Cant move LimitMap
				if (player.getX() - player.getSize().x / 2.0f <= 0)
				{
					Lspeed = 0;
				}
				if (player.getX() + player.getSize().x / 2.0f >= 14999)
				{
					Rspeed = 0;
				}

				//Mana_Charge & Use
				if (cmana == manax)
				{
					cmanaClock.restart();
				}
				if (gainMana.asSeconds() >= 1)
				{
					if (cmana < manax)
					{
						cmana += 1;
						pickmana << "m" << cmana << ".png";
						m4.loadFromFile(pickmana.str());
						pickmana.str("");
						mana.setTexture(m4);
						cmanaClock.restart();
					}
				}




				//BlockSet;
				if (size < 48)
				{
					b1.set({ pos[size] }, { siz[size] });
					blocks.push_back(b1);
					size++;
				}



				//EnemyCollide_Player
				if (enemy[loopEnemy].getGlobalBounds().intersects(player.getGlobalBounds()))
				{
					if (loopEnemy <= 17)
					{
						cheart -= 1;
						pickheart << "h" << cheart << ".png";
						h6.loadFromFile(pickheart.str());
						pickheart.str("");
					}
					else
					{
						cheart -= 2;
						pickheart << "h" << cheart << ".png";
						h6.loadFromFile(pickheart.str());
						pickheart.str("");
					}
					if (player.getX() - enemy[loopEnemy].getPos().x <= 0)
						hert = -1;
					else
						hert = 1;

					hertspeed = 1000.0f;
				}
				loopEnemy += 1;
				if (loopEnemy == 21)
				{
					loopEnemy = 0;
				}
				if (enemy[loopEnemy].getGlobalBounds().intersects(player.getGlobalBounds()))
				{
					if (loopEnemy <= 17)
					{
						cheart -= 1;
						pickheart << "h" << cheart << ".png";
						h6.loadFromFile(pickheart.str());
						pickheart.str("");
					}
					else
					{
						cheart -= 2;
						pickheart << "h" << cheart << ".png";
						h6.loadFromFile(pickheart.str());
						pickheart.str("");
					}
					if (player.getX() - enemy[loopEnemy].getPos().x <= 0)
						hert = -1;
					else
						hert = 1;

					hertspeed = 1000.0f;
				}
				loopEnemy += 1;
				if (loopEnemy == 21)
				{
					loopEnemy = 0;
				}

				//Enemypush
				if (hert == -1 && hertspeed >= 0)
				{
					player.moveLeft(hertspeed * dt);
					hertspeed -= 35.0f;
					Rspeed = 0;
					Lspeed = 0;
				}
				if (hert == 1 && hertspeed >= 0)
				{
					player.moveRight(hertspeed * dt);
					hertspeed -= 35.0f;
					Lspeed = 0;
					Rspeed = 0;
				}



				//CheckBlock_Collide
				for (size_t i = 0; i < blocks.size(); i++)
				{
					if (player.getGlobalBounds().intersects(blocks[i].getGlobalBounds()))
					{
						dx = player.getX() - blocks[i].getPos().x;
						dy = player.getY() - blocks[i].getPos().y;
						float intersectX = abs(dx) - (playerHalf.x + blocks[i].getSize().x / 2.0f);
						float intersectY = abs(dy) - (playerHalf.y + blocks[i].getSize().y / 2.0f);
						if (intersectX > intersectY)
						{
							if (dx > 0.0f)
							{
								Lspeed = 0;
								hertspeed = -1;
							}
							else
							{
								Rspeed = 0;
								hertspeed = -1;
							}
						}
						else
						{
							if (dy < 0.0f)
							{
								isJump = 0;
								onGround = 1;
								Yspeed = -25.0f;
								if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
								{
									if (onGround == 1)
									{
										sJump.play();
										isJump = 1;
										onGround = 0;
										Yspeed = -700.0f;
									}
								}
								if (i == 47)
								{
									finish = 1;
								}
							}
							else
							{
								Yspeed = 50.0f;
								isJump = 0;
							}
						}
					}
					for (int j = 1; j <= scount; j++)
					{
						if (skillf[j].getGlobalBounds().intersects(blocks[i].getGlobalBounds()))
						{
							skillf[j].setPos({ -50,-50 });
						}
						if (skilli[j].getGlobalBounds().intersects(blocks[i].getGlobalBounds()))
						{
							skilli[j].setPos({ -50,-50 });
						}
						if (skillu[j].getGlobalBounds().intersects(blocks[i].getGlobalBounds()))
						{
							skillu[j].setPos({ -50,-50 });
						}
					}
				}



				//checkSkill_vsEnemys
				for (int j = 1; j <= scount; j++)
				{
					//white
					for (int i = 0; i <= 3; i++)
					{
						if (skillf[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skillf[j].setPos({ -50,-50 });
						}
						if (skilli[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skilli[j].setPos({ -50,-50 });
						}
						if (skillu[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skillu[j].setPos({ -50,-50 });
						}
					}
					//blue
					for (int i = 4; i <= 9; i++)
					{
						if (skilli[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skilli[j].setPos({ -50,-50 });
						}
						if (skillu[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skillu[j].setPos({ -50,-50 });
						}
					}
					//red
					for (int i = 10; i <= 17; i++)
					{
						if (skillf[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skillf[j].setPos({ -50,-50 });
						}
						if (skillu[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skillu[j].setPos({ -50,-50 });
						}
					}
					//violet
					for (int i = 18; i <= 20; i++)
					{
						if (skillu[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skillu[j].setPos({ -50,-50 });
						}
					}
					for (int i = 1; i <= 20; i++)
					{
						if (skillf[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							skillf[j].setPos({ -50,-50 });
						}
						if (skilli[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							skilli[j].setPos({ -50,-50 });
						}
					}
				}

				//std::cout << player.getX() << endl;

				//EnemyAttack

				enemy[10].Pattern1(dt,1830,2800);
				enemy[12].Pattern1(dt,3400,4300);
				enemy[6].Pattern1(dt, 5300, 6000);
				enemy[1].Pattern1(dt, 8000, 8300);
				enemy[7].Pattern1(dt, 8800, 9030);
				enemy[14].Pattern1(dt, 9470, 9750);
				enemy[2].Pattern1(dt, 10200, 10450);



				//bus
				enemy[11].Pattern1(dt, 2000,4410 );
				enemy[4].Pattern1(dt,2170 ,4580 );
				enemy[5].Pattern1(dt, 2340, 4750);

				enemy[17].Pattern1(dt, 11000,11930 );
				enemy[9].Pattern1(dt, 11170, 12100);

				//




				//FinishStage
				if (finish == 1)
				{
					heliR[1].move(100.0f * dt, 0.0f);
					isMove = 0;
					isJump = 0;
					Yspeed = 0;
					Yspeed -= 25.0f;
					player.moveRight(100.0f * dt);
					rope.move(100.0f * dt, 0.0f);
				}
				if (player.getX() >= 15100)
				{
					stage = 3;
					break;
				}

				//Dead
				if (cheart <= 0)
				{
					defaultView.reset(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
					window.setView(defaultView);
					dead = 1;
					break;
				}






				//playerDoing

				if (isMove == 1)
				{
					player.moveRight(Rspeed * dt);
					isMove = 0;
				}
				if (isMove == -1)
				{
					player.moveLeft(Lspeed * dt);
					isMove = 0;
				}
				if (isJump == 1 && onGround == 0)
				{
					player.moveY(Yspeed * dt);
					Yspeed += retard;

					if (player.getY() >= groundHeigh)
					{
						onGround = 1;
						isJump = 0;
					}
				}
				if (isJump == 0 && player.getY() < groundHeigh)
				{
					Yspeed += 25.0f;
					player.moveY(Yspeed * dt);
					if (player.getY() >= groundHeigh - 1)
					{
						onGround = 1;
					}
				}




				//skillDirect

				for (int i = 1; i <= scount; i++)
				{
					if (isSkillu[i] == 1)
					{
						if (sdir[i] == 0)
						{
							sdir[i] = faceRight;
						}
						if (sdir[i] == 1)
						{
							skillu[i].move((speed + 500) * dt);
						}
						if (sdir[i] == -1)
						{
							skillu[i].move(-(speed + 500) * dt);
						}
					}
					if (isSkilli[i] == 1)
					{
						if (sdir[i] == 0)
						{
							sdir[i] = faceRight;
						}
						if (sdir[i] == 1)
						{
							skilli[i].move((speed + 500) * dt);
						}
						if (sdir[i] == -1)
						{
							skilli[i].move(-(speed + 500) * dt);
						}

					}
					if (isSkillf[i] == 1)
					{
						if (sdir[i] == 0)
						{
							sdir[i] = faceRight;
						}
						if (sdir[i] == 1)
						{
							skillf[i].move((speed + 500) * dt);
						}
						if (sdir[i] == -1)
						{
							skillf[i].move(-(speed + 500) * dt);
						}

					}
				}

				//ItemCollect
				if (player.getGlobalBounds().intersects(item.getGlobalBounds()))
				{
					co3.restart();
					item.setPOs({ -50.0f,-50.0f });
					ulti = 1;
				}
				//Ultimate
				if (Utime.asSeconds() >= 10)
				{
					ulti = 0;
					co3.restart();
				}











				//viewUpdate

				position.x = player.getX() + 300 - (window.getSize().x / 2.0f);
				position.y = player.getY() - (window.getSize().y / 1.0f);
				if (position.x < 0)
				{
					position.x = 0;
				}
				if (position.x >= 13439)
				{
					position.x = 13440;
				}
				if (position.y < 0)
				{
					position.y = 0;
				}
				view.reset(sf::FloatRect(position.x, position.y, window.getSize().x, window.getSize().y));
				lblStage.setPosition({ position.x + 1420.0f,10.0f });
				lblTime.setPosition({ position.x + 1420.0f,50.0f });
				heart.setPosition(10 + position.x, 10);
				mana.setPosition(19 + position.x, 57);





				//==========================================================DRAW=======================
				//ToDraw
				window.clear();
				window.setView(view);
				window.draw(bground);


				window.draw(rope);
				window.draw(hrope1);
				window.draw(hrope2);
				window.draw(hrope3);
				helicopR.Update(0, dt);
				heliR[0].setTextureRect(helicopR.uvRect);
				heliR[1].setTextureRect(helicopR.uvRect);
				window.draw(heliR[0]);
				window.draw(heliR[1]);
				helicopL.Update(0, dt);
				heliL[0].setTextureRect(helicopL.uvRect);
				heliL[1].setTextureRect(helicopL.uvRect);
				window.draw(heliL[0]);
				window.draw(heliL[1]);


				//PlatformShow
				/*
				for (size_t i = 0; i < blocks.size(); i++)
				{
					window.draw(blocks[i].body);
				}*/
				window.draw(woodh);
				window.draw(woodh2);


				for (int i = 1; i <= scount; i++)
				{
					skilli[i].toDraw(window);
					skillf[i].toDraw(window);
					skillu[i].toDraw(window);
				}
				for (int i = 0; i < 21; i++)
				{
					enemy[i].toDraw(window);
				}
				//window.draw(truck);
				//window.draw(truck2);

				window.draw(heart);
				window.draw(mana);
				window.draw(lblStage);
				window.draw(lblTime);




				item.toDraw(window);

				if (faceRight == 1)
				{
					player.toDrawR(window);
				}
				else
				{
					player.toDrawL(window);
				}
				//Draw
				window.display();

			}
		}
		if (stage == 3)
		{
			window.setFramerateLimit(60);
			float dt;  //(deltatime)

			//viewSet
			sf::View view, defaultView;
			view.reset(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
			view.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
			sf::Vector2f position(window.getSize().x, window.getSize().y);

			//background
			sf::Texture bg;
			bg.loadFromFile("map3r.png");
			sf::RectangleShape bground;
			bground.setSize(sf::Vector2f(15000, 780));
			bground.setTexture(&bg);

			//main music
			sf::Music music;
			music.openFromFile("mainSong.ogg");
			music.setVolume(5.0);
			music.play();

			//skill music
			sf::SoundBuffer soundBang, soundJump;
			soundBang.loadFromFile("bang.wav");
			soundJump.loadFromFile("musJump.wav");
			sf::Sound sBang, sJump;
			sBang.setBuffer(soundBang);
			sJump.setBuffer(soundJump);
			sBang.setVolume(30.0);
			sJump.setVolume(30.0);

			//upperText
			sf::Font font;
			font.loadFromFile("BAUHS93.TTF");

			ostringstream showStage;
			showStage << "Stage 3";
			sf::Text lblStage;
			lblStage.setCharacterSize(30);
			lblStage.setPosition({ 1420,10 });
			lblStage.setFont(font);
			lblStage.setString(showStage.str());

			int f2sec = 0, f1min = 0, f2min = 0;
			sf::Clock clock;
			sf::Time elapsed = clock.getElapsedTime();
			ostringstream showTime;

			sf::Text lblTime;
			lblTime.setCharacterSize(30);
			lblTime.setPosition({ 1420,50 });
			lblTime.setFont(font);

			//Mana+Hp
			ostringstream pickmana;
			ostringstream pickheart;

			sf::Texture h6, m4;
			h6.loadFromFile("h6.png");
			sf::Sprite heart;
			heart.setTexture(h6);
			heart.setPosition(0, 0);

			m4.loadFromFile("m4.png");
			sf::Sprite mana;
			mana.setTexture(m4);
			mana.setPosition(0, 50);

			//BackGroundAnimation
			sf::Texture heli1, heli2, texrope, tr, tr2;
			heli1.loadFromFile("helicopterRanimation.png");
			heli2.loadFromFile("helicopterLanimation.png");
			texrope.loadFromFile("ropes.png");
			tr2.loadFromFile("tr2.png");
			tr.loadFromFile("tr.png");
			sf::Sprite heliR, rope ;
			heliR.setTexture(heli1);
			heliR.setPosition(2434.0, 65.0);

			Animetion helicopR(&heli1, sf::Vector2u(1, 4), 0.05f);
			
			rope.setTexture(texrope);
			rope.setPosition(9000.0, 20.0);

			//Clock_use
			sf::Clock co1; //Lswitch
			sf::Clock co2; //Jswitch
			sf::Clock co3; //Ulti
			sf::Clock co4; //jumpBlock
			sf::Clock dtclock; //deltaTime
			sf::Clock cmanaClock; //manaGain


			//PlayerObject
			Player player({ 84.0,112.0 });
			player.setOrigin();
			player.setPos({ 12000.0,664.0 });
			sf::Vector2f playerHalf = player.getSize() / 2.0f;

			//item
			srand(time(0));
			int randomItem = rand() % 3;
			Item item;
			if (randomItem == 1)
			{
				item.setPOs({ 1300.0,380.0 });
			}
			else if (randomItem == 2)
			{
				item.setPOs({ 1300.0,100 });
			}
			else
			{
				item.setPOs({ 1300.0,600.0 });
			}

			//skillPlayer
			Skill skilli[12], skillf[12], skillu[12];
			for (int i = 1; i <= 11; i++)
			{
				skilli[i].setColor(sf::Color::Blue);
				skillf[i].setColor(sf::Color::Red);
				skillu[i].setColor(sf::Color(128, 0, 128));
			}

			//enemy
			Enemy enemy[21];
			//White
			/*enemy[0].set0();
			enemy[0].setPos({ 2452,670 });
			enemy[1].set0();
			enemy[1].setPos({ 9350,670 });
			enemy[0].set0();
			enemy[0].setPos({ 2452,670 });
			enemy[1].set0();
			enemy[1].setPos({ 9350,670 });
			//Blue
			enemy[2].set1();
			enemy[2].setPos({ 4360,670 });
			enemy[3].set1();
			enemy[3].setPos({ 4680,670 });
			enemy[4].set1();
			enemy[4].setPos({ 8590, 285 });
			enemy[2].set1();
			enemy[2].setPos({ 4360,670 });
			enemy[3].set1();
			enemy[3].setPos({ 4680,670 });
			enemy[4].set1();
			enemy[4].setPos({ 8590, 285 });
			//Red
			enemy[5].set2();
			enemy[5].setPos({ 3115,670 });
			enemy[6].set2();
			enemy[6].setPos({ 4515,670 });
			enemy[7].set2();
			enemy[7].setPos({ 9120,670 });
			enemy[8].set2();
			enemy[8].setPos({ 11100,670 });
			//Violet
			enemy[9].set3();
			enemy[9].setPos({ 5693,590 });
			enemy[10].set3();
			enemy[10].setPos({ 9560,670 });
			enemy[11].set3();
			enemy[11].setPos({ 10240,590 });*/


			Block b1;
			vector<Block> blocks;

			sf::Vector2f pos[27], siz[27];
			

			pos[0] = { 8413,30 };		//helicop
			pos[1] = { 4062,622 };		//box
			pos[2] = { 11995,626 };
			pos[3] = { 12093,626 };
			pos[4] = { 12093,534 };
			pos[5] = { 12743,622 };
			pos[6] = { 12743,530 };
			pos[7] = { 12841,622 };
			pos[8] = { 2134,144 };		//platform	
			pos[9] = { 2592,86 };
			pos[10] = { 3232,136 };
			pos[11] = { 3390,44 };
			pos[12] = { 4275,45 };
			pos[13] = { 4452,100 };
			pos[14] = { 3232,136 };
			pos[15] = { 3390,44 };
			pos[16] = { 4275,45 };
			pos[17] = { 4452,100 };
			pos[18] = { 4452,100 };
			pos[19] = { 4452,100 };
			pos[20] = { 4452,100 };
			pos[21] = { 4452,100 };
			pos[22] = { 4452,100 };
			pos[23] = { 4452,100 };
			pos[24] = { 4452,100 };
			pos[25] = { 14614.0,385 };		//finish


			siz[0] = { 397,128 };		//helicop
			siz[1] = { 98,98 };			//box
			siz[2] = { 98,98 };
			siz[3] = { 98,98 };
			siz[4] = { 98,98 };
			siz[5] = { 98,98 };
			siz[6] = { 98,98 };
			siz[7] = { 98,98 };
			siz[8] = { 190,25 };		//platform	
			siz[9] = { 190,124 };
			siz[10] = { 190,25 };
			siz[11] = { 170,104 };
			siz[12] = { 190,124 };
			siz[13] = { 145,32 };
			siz[14] = { 190,25 };
			siz[15] = { 170,104 };
			siz[16] = { 190,124 };
			siz[17] = { 145,32 };
			siz[18] = { 145,32 };
			siz[19] = { 145,32 };
			siz[20] = { 145,32 };
			siz[21] = { 145,32 };
			siz[22] = { 145,32 };
			siz[23] = { 145,32 };
			siz[24] = { 145,32 };
			siz[25] = { 168,23 };		//finish


			int size = 0;

			//variableSwitch
			int isMove = 0, isJump = 0, ulti = 0, faceRight = 1;

			//variableSkill
			int change = 0, isSkilli[12], isSkillf[12], isSkillu[12], scount = 0, sdir[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
			int cmana = 4, manax = 4;

			//variablePlayerDoing
			float speed = 600.0f, Yspeed, retard = 25.0f, fallspeed = 0, Rspeed = 600.0f, Lspeed = 600.0f;
			int  onGround = 1, groundHeigh = 662, boxx = 0;

			//Heart
			int cheart = 6, hert = 0, hertspeed = 0;

			//block
			float dx, dy;
			float intersectX, intersectY;
			int finish = 0;

			//enemy
			int loopEnemy = 0, ebus2 = 0, dashgo1 = 0, dashgo2 = 0, jumppoint1 = 0, jumppoint2 = 0, downpoint1 = 0, downpoint2 = 0;


			//------------------------------------------------------IN Game stage II------------------------
			while (window.isOpen())
			{
				dt = dtclock.restart().asSeconds();
				while (window.pollEvent(ev))
				{
					switch (ev.type)
					{
					case sf::Event::Closed:
						window.close();
						break;
					case sf::Event::KeyPressed:
						if (ev.key.code == sf::Keyboard::Escape)
							window.close();
						break;
					}
				}
				//RunTime
				elapsed = clock.getElapsedTime();
				showTime << f2min << f1min << ":" << f2sec << (int)elapsed.asSeconds() << ":" << elapsed.asMilliseconds() % 1000 / 10;
				if (elapsed.asSeconds() >= 10)
				{
					clock.restart();
					f2sec += 1;
				}
				if (f2sec >= 6)
				{
					f1min += 1;
					f2sec = 0;
				}
				if (f2min >= 10)
				{
					f1min += 1;
					f2min = 0;
				}
				lblTime.setString(showTime.str());
				showTime.str("");

				//Cooldown
				sf::Time Lcool = co1.getElapsedTime();
				sf::Time Jcool = co2.getElapsedTime();
				sf::Time Utime = co3.getElapsedTime();
				sf::Time JumpCool = co4.getElapsedTime();
				sf::Time gainMana = cmanaClock.getElapsedTime();



				//controlKey
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				{
					if (onGround == 1)
					{
						sJump.play();
						isJump = 1;
						onGround = 0;
						Yspeed = -700.0f;
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
				{
					Lspeed = 600.0f;
					faceRight = -1;
					isMove = -1;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
				{
					Rspeed = 600.0f;
					faceRight = 1;
					isMove = 1;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L))
				{
					if (change == 0 && (Lcool.asSeconds() >= 0.5))
					{
						change = 1;
						co1.restart();
					}
					else if (change == 1 && (Lcool.asSeconds() >= 0.5))
					{
						change = 0;
						co1.restart();
					}
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J) && (Jcool.asSeconds() >= 0.2))
				{
					if (cmana > 0)
					{
						cmana -= 1;
						pickmana << "m" << cmana << ".png";
						m4.loadFromFile(pickmana.str());
						pickmana.str("");
						mana.setTexture(m4);

						co2.restart();
						sBang.play();

						//ReDirect+Posi Skill
						scount += 1;
						if (scount == 12)
						{
							for (int i = 1; i <= 11; i++)
							{
								sdir[i] = 0;
								skilli[i].setPos({ -50,-50 });
								skillf[i].setPos({ -50,-50 });
								skillu[i].setPos({ -50,-50 });
							}
							scount = 1;
						}

						if (ulti == 1)
						{
							skillu[scount].setPos({ player.getX() , player.getY() - 5 });
							isSkillu[scount] = 1;
						}
						else if (change == 0)
						{
							skilli[scount].setPos({ player.getX() , player.getY() - 5 });
							isSkilli[scount] = 1;
						}
						else
						{
							skillf[scount].setPos({ player.getX() , player.getY() - 5 });
							isSkillf[scount] = 1;
						}
					}
				}

				//Cant move LimitMap
				if (player.getX() - player.getSize().x / 2.0f <= 0)
				{
					Lspeed = 0;
				}
				if (player.getX() + player.getSize().x / 2.0f >= 14999)
				{
					Rspeed = 0;
				}

				//Mana_Charge & Use
				if (cmana == manax)
				{
					cmanaClock.restart();
				}
				if (gainMana.asSeconds() >= 1)
				{
					if (cmana < manax)
					{
						cmana += 1;
						pickmana << "m" << cmana << ".png";
						m4.loadFromFile(pickmana.str());
						pickmana.str("");
						mana.setTexture(m4);
						cmanaClock.restart();
					}
				}




				//BlockSet;
				if (size < 28)
				{
					b1.set({ pos[size] }, { siz[size] });
					blocks.push_back(b1);
					size++;
				}



				//EnemyCollide_Player
				if (enemy[loopEnemy].getGlobalBounds().intersects(player.getGlobalBounds()))
				{
					if (loopEnemy <= 8)
					{
						cheart -= 1;
						pickheart << "h" << cheart << ".png";
						h6.loadFromFile(pickheart.str());
						pickheart.str("");
					}
					else
					{
						cheart -= 2;
						pickheart << "h" << cheart << ".png";
						h6.loadFromFile(pickheart.str());
						pickheart.str("");
					}
					if (player.getX() - enemy[loopEnemy].getPos().x <= 0)
						hert = -1;
					else
						hert = 1;

					hertspeed = 1000.0f;
				}
				loopEnemy += 1;
				if (loopEnemy == 12)
				{
					loopEnemy = 0;
				}
				if (enemy[loopEnemy].getGlobalBounds().intersects(player.getGlobalBounds()))
				{
					if (loopEnemy <= 8)
					{
						cheart -= 1;
						pickheart << "h" << cheart << ".png";
						h6.loadFromFile(pickheart.str());
						pickheart.str("");
					}
					else
					{
						cheart -= 2;
						pickheart << "h" << cheart << ".png";
						h6.loadFromFile(pickheart.str());
						pickheart.str("");
					}
					if (player.getX() - enemy[loopEnemy].getPos().x <= 0)
						hert = -1;
					else
						hert = 1;

					hertspeed = 1000.0f;
				}
				loopEnemy += 1;
				if (loopEnemy == 12)
				{
					loopEnemy = 0;
				}

				//Enemypush
				if (hert == -1 && hertspeed >= 0)
				{
					player.moveLeft(hertspeed * dt);
					hertspeed -= 35.0f;
					Rspeed = 0;
					Lspeed = 0;
				}
				if (hert == 1 && hertspeed >= 0)
				{
					player.moveRight(hertspeed * dt);
					hertspeed -= 35.0f;
					Lspeed = 0;
					Rspeed = 0;
				}



				//CheckBlock_Collide
				for (size_t i = 0; i < blocks.size(); i++)
				{
					if (player.getGlobalBounds().intersects(blocks[i].getGlobalBounds()))
					{
						dx = player.getX() - blocks[i].getPos().x;
						dy = player.getY() - blocks[i].getPos().y;
						float intersectX = abs(dx) - (playerHalf.x + blocks[i].getSize().x / 2.0f);
						float intersectY = abs(dy) - (playerHalf.y + blocks[i].getSize().y / 2.0f);
						if (intersectX > intersectY)
						{
							if (dx > 0.0f)
							{
								Lspeed = 0;
								hertspeed = -1;
							}
							else
							{
								Rspeed = 0;
								hertspeed = -1;
							}
						}
						else
						{
							if (dy < 0.0f)
							{
								isJump = 0;
								onGround = 1;
								Yspeed = -25.0f;
								if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
								{
									if (onGround == 1)
									{
										sJump.play();
										isJump = 1;
										onGround = 0;
										Yspeed = -700.0f;
									}
								}
								if (i == 47)
								{
									finish = 1;
								}
							}
							else
							{
								Yspeed = 50.0f;
								isJump = 0;
							}
						}
					}
					for (int j = 1; j <= scount; j++)
					{
						if (skillf[j].getGlobalBounds().intersects(blocks[i].getGlobalBounds()))
						{
							skillf[j].setPos({ -50,-50 });
						}
						if (skilli[j].getGlobalBounds().intersects(blocks[i].getGlobalBounds()))
						{
							skilli[j].setPos({ -50,-50 });
						}
						if (skillu[j].getGlobalBounds().intersects(blocks[i].getGlobalBounds()))
						{
							skillu[j].setPos({ -50,-50 });
						}
					}
				}



				//checkSkill_vsEnemys
				for (int j = 1; j <= scount; j++)
				{
					//white
					for (int i = 0; i <= 1; i++)
					{
						if (skillf[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skillf[j].setPos({ -50,-50 });
						}
						if (skilli[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skilli[j].setPos({ -50,-50 });
						}
						if (skillu[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skillu[j].setPos({ -50,-50 });
						}
					}
					//blue
					for (int i = 2; i <= 4; i++)
					{
						if (skilli[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skilli[j].setPos({ -50,-50 });
						}
						if (skillu[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skillu[j].setPos({ -50,-50 });
						}
					}
					//red
					for (int i = 5; i <= 8; i++)
					{
						if (skillf[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skillf[j].setPos({ -50,-50 });
						}
						if (skillu[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skillu[j].setPos({ -50,-50 });
						}
					}
					//violet
					for (int i = 9; i <= 11; i++)
					{
						if (skillu[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							enemy[i].setPos({ -300,-300 });
							skillu[j].setPos({ -50,-50 });
						}
					}
					for (int i = 1; i <= 11; i++)
					{
						if (skillf[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							skillf[j].setPos({ -50,-50 });
						}
						if (skilli[j].getGlobalBounds().intersects(enemy[i].getGlobalBounds()))
						{
							skilli[j].setPos({ -50,-50 });
						}
					}
				}

				/*enemy[0].Pattern0(dt);

				enemy[4].Pattern1(dt, 8390, 8790);
				enemy[5].Pattern1(dt, 2915, 3355);
				enemy[8].Pattern1(dt, 10900, 11300);

				//Buss
				enemy[2].Pattern1(dt, 3760, 4660);
				enemy[6].Pattern1(dt, 3915, 4815);
				enemy[3].Pattern1(dt, 4080, 4980);


				if (player.getX() >= 7650)	ebus2 = 1;
				if (ebus2 == 1)
				{
					enemy[7].Pattern1(dt, 7720, 9170);
					enemy[1].Pattern1(dt, 7950, 9400);
					enemy[10].Pattern1(dt, 8160, 9610);
				}

				//Dash
				if (player.getX() >= 6300)
				{
					dashgo1 = 1;
				}
				if (enemy[9].getPos().x >= 6600)
				{
					jumppoint1 = 1;
				}
				if (enemy[9].getPos().x >= 6050)
				{
					downpoint1 = 1;
				}
				enemy[9].Pattern2(dt, dashgo1, jumppoint1, downpoint1);*/

				/*
				if (player.getX() >= 6500)
				{
					dashgo2 = 1;
				}
				if (enemy[11].getPos().x >= 6600)
				{
					jumppoint2 = 1;
				}
				if (enemy[11].getPos().x >= 6050)
				{
					downpoint2 = 1;
				}
				enemy[11].Pattern2(dt, dashgo2, jumppoint2, downpoint2);*/




				//FinishStage
				if (finish == 1)
				{
					heliR.move(100.0f * dt, 0.0f);
					isMove = 0;
					isJump = 0;
					Yspeed = 0;
					Yspeed -= 25.0f;
					player.moveRight(100.0f * dt);
					rope.move(100.0f * dt, 0.0f);
				}

				//Dead
				if (cheart <= 0)
				{
					defaultView.reset(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
					window.setView(defaultView);
					dead = 1;
					break;
				}






				//playerDoing

				if (isMove == 1)
				{
					player.moveRight(Rspeed * dt);
					isMove = 0;
				}
				if (isMove == -1)
				{
					player.moveLeft(Lspeed * dt);
					isMove = 0;
				}
				if (isJump == 1 && onGround == 0)
				{
					player.moveY(Yspeed * dt);
					Yspeed += retard;

					if (player.getY() >= groundHeigh)
					{
						onGround = 1;
						isJump = 0;
					}
				}
				if (isJump == 0 && player.getY() < groundHeigh)
				{
					Yspeed += 25.0f;
					player.moveY(Yspeed * dt);
					if (player.getY() >= groundHeigh - 1)
					{
						onGround = 1;
					}
				}




				//skillDirect

				for (int i = 1; i <= scount; i++)
				{
					if (isSkillu[i] == 1)
					{
						if (sdir[i] == 0)
						{
							sdir[i] = faceRight;
						}
						if (sdir[i] == 1)
						{
							skillu[i].move((speed + 500) * dt);
						}
						if (sdir[i] == -1)
						{
							skillu[i].move(-(speed + 500) * dt);
						}
					}
					if (isSkilli[i] == 1)
					{
						if (sdir[i] == 0)
						{
							sdir[i] = faceRight;
						}
						if (sdir[i] == 1)
						{
							skilli[i].move((speed + 500) * dt);
						}
						if (sdir[i] == -1)
						{
							skilli[i].move(-(speed + 500) * dt);
						}

					}
					if (isSkillf[i] == 1)
					{
						if (sdir[i] == 0)
						{
							sdir[i] = faceRight;
						}
						if (sdir[i] == 1)
						{
							skillf[i].move((speed + 500) * dt);
						}
						if (sdir[i] == -1)
						{
							skillf[i].move(-(speed + 500) * dt);
						}

					}
				}

				//ItemCollect
				if (player.getGlobalBounds().intersects(item.getGlobalBounds()))
				{
					co3.restart();
					item.setPOs({ -50.0f,-50.0f });
					ulti = 1;
				}
				//Ultimate
				if (Utime.asSeconds() >= 12)
				{
					ulti = 0;
					co3.restart();
				}


				std::cout << player.getX() << std::endl;








				//viewUpdate

				position.x = player.getX() + 300 - (window.getSize().x / 2.0f);
				position.y = player.getY() - (window.getSize().y / 1.0f);
				if (position.x < 0)
				{
					position.x = 0;
				}
				if (position.x >= 13439)
				{
					position.x = 13440;
				}
				if (position.y < 0)
				{
					position.y = 0;
				}
				view.reset(sf::FloatRect(position.x, position.y, window.getSize().x, window.getSize().y));
				lblStage.setPosition({ position.x + 1420.0f,10.0f });
				lblTime.setPosition({ position.x + 1420.0f,50.0f });
				heart.setPosition(10 + position.x, 10);
				mana.setPosition(19 + position.x, 57);





				//==========================================================DRAW=======================
				//ToDraw
				window.clear();
				window.setView(view);
				window.draw(bground);


				window.draw(rope);
				helicopR.Update(0, dt);
				heliR.setTextureRect(helicopR.uvRect);
				window.draw(heliR);
				


				//PlatformShow
				
				for (size_t i = 0; i < blocks.size(); i++)
				{
					window.draw(blocks[i].body);
				}
				


				for (int i = 1; i <= scount; i++)
				{
					skilli[i].toDraw(window);
					skillf[i].toDraw(window);
					skillu[i].toDraw(window);
				}
				for (int i = 0; i < 12; i++)
				{
					enemy[i].toDraw(window);
				}
				

				window.draw(heart);
				window.draw(mana);
				window.draw(lblStage);
				window.draw(lblTime);




				item.toDraw(window);

				if (faceRight == 1)
				{
					player.toDrawR(window);
				}
				else
				{
					player.toDrawL(window);
				}
				//Draw
				window.display();

			}
		}



		//GameOver
		if (dead == 1)
		{
			dead = 0;
			sf::Clock codead;
			sf::Time deadtime;
			sf::Font fontdead;
			sf::Text deadtext;
			fontdead.loadFromFile("BAUHS93.TTF");
			deadtext.setCharacterSize(150.0);
			deadtext.setFont(fontdead);
			deadtext.setPosition({ 410,270 });
			deadtext.setFillColor(sf::Color::White);
			ostringstream showdead;
			showdead << "GAME OVER";
			deadtext.setString(showdead.str());
			while (true)
			{
				deadtime = codead.getElapsedTime();
				if (deadtime.asSeconds()>=7.0|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
				{
					codead.restart();
					stage=0;
					break;
				}
				window.clear(sf::Color::Red);
				window.draw(deadtext);
				window.display();
			}
		}
	}


	return 0;
}

