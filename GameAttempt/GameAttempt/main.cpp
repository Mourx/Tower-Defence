#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>


const int SPIDER_HEALTH = 50;
const int GHOST_HEALTH = 100;


class node {
	public:
		sf::Sprite icon;
		int order;
		int xpos, ypos;
};



class menuItem {

	public:
		sf::Sprite icon;
		int xpos, ypos;
		int towertype;

};

class mob {

	public:
		sf::Sprite icon;
		int currentNode, nextNode;
		int xdirection, ydirection;
		int width, height;
		int health;
		int bounty;
		bool dead = false;
		void getDirection(node c,node n) {
			if (c.xpos < n.xpos) {
				xdirection = 1;
			}
			else if (c.xpos == n.xpos) {
				xdirection = 0;
			}
			else {
				xdirection = -1;
			}
			if (c.ypos < n.ypos) {
				ydirection = 1;
			}
			else if (c.ypos == n.ypos) {
				ydirection = 0;
			}
			else {
				ydirection = -1;
			}

		}
};

float getXcoefficient(sf::Vector2f start, sf::Vector2f end) {

	return (end.x + 10 - start.x) / sqrt(pow(start.x - end.x + 10, 2) + pow(start.y - end.y + 10, 2));

}

float getYcoefficient(sf::Vector2f start, sf::Vector2f end) {

	return (end.y + 10 - start.y) / sqrt(pow(start.x - end.x + 10, 2) + pow(start.y - end.y + 10, 2));

}

class projectile {

	public:
		sf::Sprite icon;
		int xpos, ypos;
		float xdirection, ydirection;
		int damage;
		mob *target;
		bool inUse = false;
		void getMissileDirection() {


			//if (icon.getPosition().x < target.icon.getPosition().x + 10) {
				xdirection = getXcoefficient(icon.getPosition(), (*target).icon.getPosition());
			//}
			//else if (icon.getPosition().x == target.icon.getPosition().x + 10) {
			//	xdirection = 0;
			//}
			//else {
			//	xdirection = -getXcoefficient(icon.getPosition(), target.icon.getPosition());;
			//}
			//if (icon.getPosition().y < target.icon.getPosition().y + 10) {
				ydirection = getYcoefficient(icon.getPosition(),(*target).icon.getPosition());
			//}
			/*else if (icon.getPosition().y == target.icon.getPosition().y + 10) {
				ydirection = 0;
			}
			else {
				ydirection = -getYcoefficient(icon.getPosition(), target.icon.getPosition());
			}*/
			

		}
		bool checkHit() {
			if (target->icon.getGlobalBounds().contains(icon.getPosition())) {
				return true;
			}
			return false;
		}

};

class tower {

public:
	sf::Sprite icon;
	int xpos, ypos;
	int damage,range;
	int type, count;
	int cost;
	projectile shot;
	bool firing = false;
	float cooldown = 60;


};

mob checkDestReached(mob m, node one, node two) {
	sf::FloatRect mBox = m.icon.getGlobalBounds();
	sf::FloatRect oBox = one.icon.getGlobalBounds();
	sf::Vector2f botright;
	sf::Vector2f topleft;
	topleft.x = m.icon.getPosition().x-5;
	topleft.y = m.icon.getPosition().y-5;
	botright.x = m.icon.getPosition().x + 34;
	botright.y = m.icon.getPosition().y + 34;
	if ( (oBox.contains(topleft)) && (oBox.contains(botright))) {
		m.getDirection(one, two);
		m.currentNode += 1;
		m.nextNode += 1;
		
	}
	

	return m;
}

void killmob(mob *monster,int *score,int *money) {

	monster->icon.setPosition(-100, -100);
	monster->dead = true;
	*score += 10;
	*money += monster->bounty;
	
}

void checkDead(mob *monster,int *score,int *money, int *progress) {

	if (monster->health <= 0) {
		killmob(monster,score,money);
		*progress += 1;
	}

}

void setNodes(node waypoints[]) {
	waypoints[0].xpos = 120;
	waypoints[0].ypos = 0;
	waypoints[0].icon.setPosition(waypoints[0].xpos, waypoints[0].ypos);

	waypoints[1].xpos = 120;
	waypoints[1].ypos = 120;
	waypoints[1].icon.setPosition(waypoints[1].xpos, waypoints[1].ypos);

	waypoints[2].xpos = 280;
	waypoints[2].ypos = 120;
	waypoints[2].icon.setPosition(waypoints[2].xpos, waypoints[2].ypos);

	waypoints[3].xpos = 280;
	waypoints[3].ypos = 400;
	waypoints[3].icon.setPosition(waypoints[3].xpos, waypoints[3].ypos);

	waypoints[4].xpos = 160;
	waypoints[4].ypos = 400;
	waypoints[4].icon.setPosition(waypoints[4].xpos, waypoints[4].ypos);

	waypoints[5].xpos = 160;
	waypoints[5].ypos = 240;
	waypoints[5].icon.setPosition(waypoints[5].xpos, waypoints[5].ypos);

	waypoints[6].xpos = 40;
	waypoints[6].ypos = 240;
	waypoints[6].icon.setPosition(waypoints[6].xpos, waypoints[6].ypos);

	waypoints[7].xpos = 40;
	waypoints[7].ypos = 520;
	waypoints[7].icon.setPosition(waypoints[7].xpos, waypoints[7].ypos);

	waypoints[8].xpos = 440;
	waypoints[8].ypos = 520;
	waypoints[8].icon.setPosition(waypoints[8].xpos, waypoints[8].ypos);

	waypoints[9].xpos = 440;
	waypoints[9].ypos = 80;
	waypoints[9].icon.setPosition(waypoints[9].xpos, waypoints[9].ypos);

	waypoints[10].xpos = 360;
	waypoints[10].ypos = 80;
	waypoints[10].icon.setPosition(waypoints[10].xpos, waypoints[10].ypos);

	waypoints[11].xpos = 360;
	waypoints[11].ypos = 0;
	waypoints[11].icon.setPosition(waypoints[11].xpos, waypoints[11].ypos);

	waypoints[12].xpos = 360;
	waypoints[12].ypos = -40;
	waypoints[12].icon.setPosition(waypoints[12].xpos, waypoints[12].ypos);

}

void createDefendo(tower defendo[],sf::Sprite grass,int count) {

	
	
	defendo[count].icon.setOrigin(0, 0);
	defendo[count].range = 100;
	defendo[count].icon.setPosition(grass.getPosition().x, grass.getPosition().y);
	defendo[count].damage = 50;
	defendo[count].cost = 50;
	
}

void createExplodo(tower explodo[], sf::Sprite grass, int count) {



	explodo[count].icon.setOrigin(0, 0);

	explodo[count].icon.setPosition(grass.getPosition().x, grass.getPosition().y);

	
}

projectile getAvailableMissile(projectile missiles[]) {

	for (int i = 0; i < 200; i++) {

		if (!missiles[i].inUse) {
			missiles[i].inUse = true;
			return missiles[i];
		}

	}

}

bool checkRange(sf::Vector2f start, sf::Vector2f end,int max) {

	if (max >= sqrt(pow(start.x - end.x, 2) + pow(start.y - end.y, 2))) {
		return true;
	}
	else
		return false;


}

void makeEnemies(mob enemy[],int spiders,int ghosts, node waypoints[]) {
	
	for (int j = 0; j < spiders; j++) {
		enemy[j].icon.setOrigin(0, 0);
		enemy[j].health = SPIDER_HEALTH;
		enemy[j].currentNode = 0;
		enemy[j].bounty = 5;
		enemy[j].nextNode =  1;
		enemy[j].dead = false;
		enemy[j].icon.setPosition(waypoints[enemy[j].currentNode].xpos + 5, waypoints[enemy[j].currentNode].ypos + 5 - j * 25);
		enemy[j].getDirection(waypoints[enemy[j].currentNode], waypoints[enemy[j].nextNode]);
	}
	for (int j = spiders; j < spiders+ghosts; j++) {
		enemy[j].icon.setOrigin(0, 0);
		enemy[j].health = GHOST_HEALTH;
		enemy[j].currentNode = 0;
		enemy[j].bounty = 5;
		enemy[j].nextNode = 1;
		enemy[j].dead = false;
		enemy[j].icon.setPosition(waypoints[enemy[j].currentNode].xpos + 5, waypoints[enemy[j].currentNode].ypos + 5 - j * 25);
		enemy[j].getDirection(waypoints[enemy[j].currentNode], waypoints[enemy[j].nextNode]);
	}


}


int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SpiderCreepy!");

	const int maxNodes = 13;
	
	bool gameOver = false;
	int current_round = 0;
	int round_progress = 0;
	int spiders_per_round[20] = { 4,2,20,30,50,60,80,100,120 };
	int ghosts_per_round[20] = { 0,5,10,20,30,40,50,80,100 };


	//manually entering all dirt nodes for gfx
	int dirtTiles[60] = { 45, 46, 47, 48, 63, 78, 93,108,109,110,111,112,113,114,115,100,85,70,69,68,67,66,51,36,21,22,23,24,25,26,27,28,43,58,73,88,103,118,133,148,163,178,177,176,175,174,173,172,171,170,169,168,167,152,137,136,135,45,45,45 };
	int towerTiles[600] = { 45 };

	tower defendo[200];
	int defendo_count = 0;
	
	tower explodo[200];
	int explodo_count = 0;

	projectile missiles[200];
	
	const int initial_mobs = 150;
	int total_mobs = spiders_per_round[current_round]+ ghosts_per_round[current_round];
	
	int total_tower_count = 0;

	int current_tower = 0;

	int defendo_cost = 50;

	

	int score = 0;
	int life = 5;
	int money = 80;

	//load fonts and texts
	sf::Font font;
	font.loadFromFile("impact.ttf");

	sf::Text text_game_over;
	text_game_over.setFont(font);
	text_game_over.setString("Game Over");
	text_game_over.setPosition(320, 260);
	text_game_over.setCharacterSize(40);
	text_game_over.setOutlineColor(sf::Color::Yellow);
	text_game_over.setFillColor(sf::Color::Yellow);

	sf::Text text_score;
	text_score.setFont(font);
	text_score.setPosition(720, 40);
	text_score.setCharacterSize(32);
	text_score.setOutlineColor(sf::Color::Black);
	text_score.setFillColor(sf::Color::Black);

	sf::Text text_life;
	text_life.setFont(font);
	text_life.setPosition(720, 110);
	text_life.setCharacterSize(32);
	text_life.setOutlineColor(sf::Color::Black);
	text_life.setFillColor(sf::Color::Black);

	sf::Text text_money;
	text_money.setFont(font);
	text_money.setPosition(720, 540);
	text_money.setCharacterSize(32);
	text_money.setOutlineColor(sf::Color::Black);
	text_money.setFillColor(sf::Color::Black);

	//load textures
	sf::Texture tex_ident[225];
	for (int i = 0; i < 225; i++)
	{
		
		if (!tex_ident[i].loadFromFile("grass.png")) {
			//error
		}
	}
	
	sf::Texture tex_block;
	tex_block.loadFromFile("block.png");

	sf::Texture tex_menu_background;
	tex_menu_background.loadFromFile("menubackground.png");

	sf::Texture tex_menu_score;
	tex_menu_score.loadFromFile("menu_score.png");

	sf::Texture tex_menu_life;
	tex_menu_life.loadFromFile("menu_life.png");

	sf::Texture tex_menu_money;
	tex_menu_money.loadFromFile("menu_money.png");

	sf::Texture tex_menu_title_tower;
	tex_menu_title_tower.loadFromFile("menu_tower_title.png");

	sf::Texture tex_dirt;
	tex_dirt.loadFromFile("dirt.png");
	
	sf::Texture tex_defendo;
	tex_defendo.loadFromFile("defendo.png");

	sf::Texture tex_explodo;
	tex_explodo.loadFromFile("explodo.png");

	sf::Texture tex_menu_defendo;
	tex_menu_defendo.loadFromFile("menu_defendo.png");

	sf::Texture tex_menu_explodo;
	tex_menu_explodo.loadFromFile("menu_explodo.png");

	sf::Texture tex_spider;
	tex_spider.loadFromFile("spider.png");

	sf::Texture tex_ghost;
	tex_ghost.loadFromFile("ghost.png");

	sf::Texture tex_missile;
	tex_missile.loadFromFile("missile.png");

	for (int i = 0; i < 200; i++) {
		missiles[i].icon.setOrigin(0, 0);
		missiles[i].icon.setTexture(tex_missile);
	}

	//creating tower menu
	sf::Sprite menu_background;
	menu_background.setTexture(tex_menu_background);
	menu_background.setPosition(600, 0);
	
	//creating score interface
	sf::Sprite menu_score;
	menu_score.setTexture(tex_menu_score);
	menu_score.setOrigin(0, 0);
	menu_score.setPosition(620, 20);

	//creating score interface
	sf::Sprite menu_life;
	menu_life.setTexture(tex_menu_life);
	menu_life.setOrigin(0, 0);
	menu_life.setPosition(620, 90);

	//creating menu title for towers
	sf::Sprite menu_title_towers;
	menu_title_towers.setTexture(tex_menu_title_tower);
	menu_title_towers.setOrigin(0, 0);
	menu_title_towers.setPosition(620, 160);

	//creating defendo menu item
	menuItem buy_defendo;
	buy_defendo.icon.setTexture(tex_menu_defendo);
	buy_defendo.icon.setOrigin(0, 0);
	buy_defendo.icon.setPosition(640, 280);

	//creating explodo menu item
	menuItem buy_explodo;
	buy_explodo.icon.setTexture(tex_menu_explodo);
	buy_explodo.icon.setOrigin(0, 0);
	buy_explodo.icon.setPosition(700, 280);

	//creating score interface
	sf::Sprite menu_money;
	menu_money.setTexture(tex_menu_money);
	menu_money.setOrigin(0, 0);
	menu_money.setPosition(620, 520);

	//creating array of grass 
	sf::Sprite spr_grass[225];
	
	for (int i = 0; i < 15; i++)
	{
		
		for (int j = 0; j < 15; j++) {
			spr_grass[i*15 + j].setTexture(tex_ident[i]);
			spr_grass[i*15 + j].setPosition(i * 40, j * 40);
	
		}

	}
	//and dirt
	for (int i = 0; i < 60; i++) {
		spr_grass[dirtTiles[i]].setTexture(tex_dirt);
	
	}
	//and waypoint nodes
	sf::Sprite spr_node[14];
	node waypoints[maxNodes];
	for (int i = 0; i < 14; i++) {
		waypoints[i].icon.setTexture(tex_block);
		
	}
	
	//setting nodes
	setNodes(waypoints);

	for (int i = 0; i < 200; i++) {
		defendo[i].icon.setTexture(tex_defendo);
		defendo[i].icon.setPosition(-40,-40);
	}

	for (int i = 0; i < 200; i++) {
		explodo[i].icon.setTexture(tex_explodo);
		explodo[i].icon.setPosition(-40, -40);
	}

	//making a spider
	mob enemy[initial_mobs];
	makeEnemies(enemy,spiders_per_round[current_round],ghosts_per_round[current_round],waypoints);
	for (int j = 0; j < spiders_per_round[current_round]; j++) {
		enemy[j].icon.setTexture(tex_spider);
	}
	for (int j = spiders_per_round[current_round]; j < spiders_per_round[current_round] + ghosts_per_round[current_round]; j++) {
		enemy[j].icon.setTexture(tex_ghost);
	}
	
	while (window.isOpen())
	{
		sf::Event event;
		window.setFramerateLimit(60);
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (!gameOver) {
			for (int i = 0; i < defendo_count; i++) {
				if (!defendo[i].firing) {

					//for (int j = 0; j < 4; j++) {
					if (!defendo[i].shot.inUse) {
						for (int k = total_mobs - 1; k >= 0; k--) {
							if (checkRange(defendo[i].icon.getPosition(), enemy[k].icon.getPosition(), defendo[i].range)) {
								defendo[i].firing = true;

								defendo[i].shot = missiles[0];
								defendo[i].shot.inUse = true;
								defendo[i].shot.icon.setPosition(defendo[i].icon.getPosition().x +15, defendo[i].icon.getPosition().y + 15);
								defendo[i].shot.target = &enemy[k];
								defendo[i].shot.damage = defendo[i].damage;
							}
							else {
								//defendo[i].firing = false;

							}
						}

					}
					//}

				}


			}




			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				for (int i = 0; i < 225; i++) {
					if (std::find(std::begin(dirtTiles), std::end(dirtTiles), i) == std::end(dirtTiles)) {
						if (buy_defendo.icon.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
							current_tower = 0;
						}
						if (buy_explodo.icon.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
							current_tower = 1;
						}
						if (std::find(std::begin(towerTiles), std::end(towerTiles), i) == std::end(towerTiles)) {
							if (spr_grass[i].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
								switch (current_tower) {
								case 0:
									if (money >= defendo_cost) {
										createDefendo(defendo, spr_grass[i], defendo_count);
										towerTiles[total_tower_count] = i;
										total_tower_count += 1;
										defendo_count += 1;
										money -= defendo_cost;
										break;
									}
									else {
										break;
									}
								case 1:
									createExplodo(explodo, spr_grass[i], explodo_count);
									towerTiles[total_tower_count] = i;
									total_tower_count += 1;
									explodo_count += 1;
									break;
								}
							}
						}
					}
				}
			}

			window.clear(sf::Color::Black);
			for (int i = 0; i < 225; i++)
			{
				window.draw(spr_grass[i]);
			}

			window.draw(menu_background);
			window.draw(menu_score);
			window.draw(menu_life);
			window.draw(menu_title_towers);
			window.draw(buy_defendo.icon);
			window.draw(buy_explodo.icon);
			window.draw(menu_money);

			text_score.setString(std::to_string(score));
			text_life.setString(std::to_string(life));
			text_money.setString(std::to_string(money));
			window.draw(text_score);
			window.draw(text_life);
			window.draw(text_money);

			for (int i = 0; i < 13; i++)
			{
				window.draw(waypoints[i].icon);
			}


			for (int k = 0; k < total_mobs; k++) {
				window.draw(enemy[k].icon);
				enemy[k].icon.move(enemy[k].xdirection * 0.8, enemy[k].ydirection * 0.8);
			}


			for (int i = 0; i < defendo_count; i++) {
				window.draw(defendo[i].icon);

			}
			for (int i = 0; i < defendo_count; i++) {

				for (int j = 0; j < 4; j++) {
					if (defendo[i].shot.inUse) {
						window.draw(defendo[i].shot.icon);
						//defendo[i].shot.target = spider[1];
						defendo[i].shot.getMissileDirection();
						defendo[i].shot.icon.move(defendo[i].shot.xdirection * 1, defendo[i].shot.ydirection * 1);
						if (defendo[i].shot.checkHit() == true) {
							defendo[i].shot.target->health -= defendo[i].shot.damage;
							checkDead(defendo[i].shot.target, &score, &money, &round_progress);
							defendo[i].shot.icon.setPosition(-40, -40);
							defendo[i].shot.inUse = false;

						}
						if (defendo[i].shot.target->dead) {
							defendo[i].shot.inUse = false;
						}
					}
				}


				if (defendo[i].firing) {
					defendo[i].cooldown -= 1;
				}
				if (defendo[i].cooldown == 0) {
					defendo[i].cooldown = 60;
					defendo[i].firing = false;
				}
			}


			for (int i = 0; i < 200; i++) {
				if (missiles[i].inUse) {

					window.draw(missiles[i].icon);

				}

			}

			for (int i = 0; i < explodo_count; i++) {
				window.draw(explodo[i].icon);
			}

			window.display();
			for (int k = 0; k < total_mobs; k++) {
				if (!enemy[k].dead) {
					enemy[k] = checkDestReached(enemy[k], waypoints[enemy[k].nextNode], waypoints[enemy[k].nextNode + 1]);
					if (enemy[k].nextNode == maxNodes) {
						life -= 1;
						enemy[k].currentNode = 0;
						enemy[k].nextNode = enemy[k].currentNode + 1;
						enemy[k].icon.setPosition(waypoints[enemy[k].currentNode].xpos + 5, waypoints[enemy[k].currentNode].ypos + 5);
						enemy[k].getDirection(waypoints[enemy[k].currentNode], waypoints[enemy[k].nextNode]);
					}
				}
			}
			if (round_progress == total_mobs) {
				for (int i = 0; i < 200; i++) {
					defendo[i].shot.inUse = false;
				}
				current_round += 1;
				total_mobs = spiders_per_round[current_round] + ghosts_per_round[current_round];
				makeEnemies(enemy, spiders_per_round[current_round], ghosts_per_round[current_round],waypoints);
				for (int j = 0; j < spiders_per_round[current_round]; j++) {
					enemy[j].icon.setTexture(tex_spider);
				}
				for (int j = spiders_per_round[current_round]; j < spiders_per_round[current_round] + ghosts_per_round[current_round]; j++) {
					enemy[j].icon.setTexture(tex_ghost);
				}
				round_progress = 0;
			}

			if (life == 0) {
				gameOver = true;
			}
		} else {
			window.draw(text_game_over);
			window.display();
		}
		
	}

	return 0;
}




