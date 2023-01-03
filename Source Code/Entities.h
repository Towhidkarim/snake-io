#ifndef ENTITIES_H
#define ENTITIES_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <map>

int randomRange(int start, int end) {
	srand(time(0));
	return (rand() % end) - start;
}


enum CtrlScheme { ARROW, WASD };

std::map<std::string, sf::Keyboard::Key> schemes[2] = {

	{
		{"UP", sf::Keyboard::Up},
		{"DOWN", sf::Keyboard::Down},
		{"RIGHT", sf::Keyboard::Right},
		{"LEFT", sf::Keyboard::Left},
		{"BOOST", sf::Keyboard::Num0}
	},

	{
		{"UP", sf::Keyboard::W},
		{"DOWN", sf::Keyboard::S},
		{"RIGHT", sf::Keyboard::D},
		{"LEFT", sf::Keyboard::A},
		{"BOOST", sf::Keyboard::Space}
	}

	
};


const float PI = 3.141592;
const float rad2deg = 180.f / PI;
const float deg2rad = PI / 180.f;
const int rad = 20;
const sf::Vector2f boardSize(5000, 5000);

float roundDeg(float deg) {
	if (deg < 0) return 360 + deg;
}
class Entity {
	public:
		sf::Vector2f pos, velocity, lastDir = sf::Vector2f(1, 0);
		float radius, *delta, tAngle;
		sf::CircleShape object;
		sf::RenderWindow* window;
		Entity(){}
		Entity(float r, float x, float y, sf::RenderWindow* win, float* dt) {
			radius = r;
			pos.x = x;
			pos.y = y;
			delta = dt;
			window = win;
			velocity = sf::Vector2f(0, 0);
			object.setPointCount(72);
			object.setRadius(radius);
			object.setOrigin(radius, radius);
			object.setPosition(pos.x, pos.y);
		}
		void reDraw() {
			window->draw(object);
		}
		void update(bool draw  = true) {
			object.setRadius(radius);
			pos.x += velocity.x * *delta;
			pos.y += velocity.y * *delta;
			
			if(draw)
				window->draw(object);
			object.setPosition(pos.x, pos.y);
		}

		void move(float x, float y) {
			pos.x += x * *delta;
			pos.y += y * *delta;
		}

};

class Food : public Entity {

	public:
		sf::Vector2f* pPos;
		sf::CircleShape* head;
		Food(float r, float x, float y, sf::RenderWindow* win, float* dt, sf::CircleShape* pPos)
			:Entity(r, x, y, win, dt) {
			//this->pPos->x = pPos->getPosition();
			head = pPos;
			object.setPointCount(8);
			object.setFillColor(sf::Color::Red);
		}
		void update() {
			//if (distSq() <= 400) {
			//	//std::cout << distSq()<<"\n";
			//}
			//if (distSq() - 200 < 3200) {
			//	//std::cout << distSq()<<"\n";
			//	moveToHead();
			//}
			Entity::update();

		}
		float distSq(sf::Vector2f hPos) {
			float x = hPos.x - pos.x;
			float y = hPos.y - pos.y;
			return x * x + y * y;
		}

		void moveToHead(sf::Vector2f hPos) {
			float x = hPos.x - pos.x;
			float y = hPos.y - pos.y;
			float sq = sqrt(x * x + y * y);
			x /= sq;
			y /= sq;
			//pos.x += x * 150 * *delta;
			//pos.y += y * 150 * *delta;
			move(x * 200, y * 200);

		}

};


class BodyPart : public Entity {

	public:
		BodyPart(float r, float x, float y, sf::RenderWindow* win, float* dt)
			:Entity(r, x, y, win, dt) {
				object.setOutlineThickness(-2);
				object.setOutlineColor(sf::Color(91, 37, 167));
				object.setFillColor(sf::Color(155, 107, 223));

		}
		void update(bool draw = true) {
			Entity::update(draw);
		}

		void setVelocity(float x, float y) {
			velocity.x = x;
			velocity.y = y;
		}

		void move(float x, float y) {
			pos.x += x * *delta;
			pos.y += y * *delta;
		}
};





class SnakeHead : public Entity {
	public:
		sf::Vector2f mousePos, direction = sf::Vector2f(1, 0);
		float linearVelocity = 200;
		std::vector<BodyPart> bodyParts;
		float score = 0;
		int growthBuffer = 0;
		CtrlScheme scheme = WASD;
		SnakeHead(float r, float x, float y, sf::RenderWindow* win, float* dt)
			: Entity(r, x, y, win, dt) {
			//Entity::Entity(r, x, y, win, dt);

			object.setOutlineThickness(-2.5);
			object.setOutlineColor(sf::Color::Cyan);
			object.setOutlineColor(sf::Color(91, 37, 167));
			object.setFillColor(sf::Color(155, 107, 223));
			for (int i = 0; i <= 10; i++) addChild();
		}



		float boost = 1;
		float loseThresold = 0;
		void update() {
			mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
			direction.x = 0;
			direction.y = 0;
			if (boost != 1) {
				score -= 1 * *delta;
				loseThresold += 1 * *delta;
				if (loseThresold >= 4) {
					bodyParts.pop_back();
					loseThresold = 0;
				}
			}
			boost = 1;
			//object.rotate(1 * *delta);
			if (sf::Keyboard::isKeyPressed(schemes[scheme]["UP"])) move("up");
			if (sf::Keyboard::isKeyPressed(schemes[scheme]["DOWN"])) move("down");
			if (sf::Keyboard::isKeyPressed(schemes[scheme]["RIGHT"])) move("right");
			if (sf::Keyboard::isKeyPressed(schemes[scheme]["LEFT"])) move("left");
			if (sf::Keyboard::isKeyPressed(schemes[scheme]["BOOST"])) {
				if(score > 0) boost = 1.75;
			}
			//std::cout << direction.x << " ," << direction.y << std::endl;
			setVelocity();
			if (pos.x < -2500)
				pos.x = -2500;
			if (pos.x > 2500)
				pos.x = 2500;
			if (pos.y < -2500)
				pos.y = -2500;
			if (pos.y > 2500)
				pos.y = 2500;
			updateChilds();
			Entity::update();
		}

		void setVelocity() {
			float angle = atan2(lastDir.y, lastDir.x);
			velocity.x = linearVelocity * boost * cos(angle);
			velocity.y = linearVelocity * boost * sin(angle);
		}
		float rotated = 0;
		float tobe = 0;
		float multi = 1;
		//bool move = false;
		void setVelocity2() {
			float newRotation = atan2f(lastDir.y, lastDir.x) * 180 / PI;
			float rotation = object.getRotation();
			if (newRotation < 0) {
				newRotation = 360 + newRotation;
			}
			else {
				//newRotation += 180;
			}
			float tobeRotated = 0; //abs(newRotation - object.getRotation());
			int multiplier = (newRotation - rotation) / abs((newRotation - rotation));
			float diff = tobe - rotation;
			int multi = 1;
			/*if (tobe <= (360 - tobe)) {
				multi = -1;
			}
			else {
				multi = 1;
			}*/
			float appliedRotation = rotation + 120 * multiplier * *delta;
			//if (rotation >= newRotation + 1 || rotation <= roundDeg(newRotation - 1))
			if (rotated < tobe) {
				//object.setRotation(rotation + 180 * multi * *delta);
				object.rotate(180 * multi * *delta);
				rotated += 180 * *delta;
			}
			

				
			//if(newRotation == 0)
			std::cout << rotation << " " << rotated << " " << tobeRotated << std::endl;
			velocity.x = cos(rotation * PI / 180) * linearVelocity;
			velocity.y = sin(rotation * PI / 180) * linearVelocity;
		}

		void move(std::string d) {
			tobe = (roundDeg(atan2f(lastDir.y, lastDir.x) * 180 / PI)) - object.getRotation();
			if (abs(tobe) >= (360 - abs(tobe))) {
				tobe = (360 - abs(tobe));
				multi = 1;
			}
			else {
				tobe = abs(tobe);
				multi = -1;
			}
			rotated = 0;
			//move = true;
			if (d == "up") {
				direction.y = -1;
			}
			if (d == "down") {
				direction.y = 1;
			}
			if (d == "right") {
				direction.x = 1;
			}
			if (d == "left") {
				direction.x = -1;
			}

			lastDir = direction;
		}

		void addChild() {
			float x, y;
			if (bodyParts.size() <= 1) {
				x = pos.x - lastDir.x * (rad / 2);
				y = pos.y - lastDir.y * (rad / 2);
			}
			else {
				x = bodyParts[bodyParts.size() - 1].pos.x - bodyParts[bodyParts.size() - 1].lastDir.x * (25);
				y = bodyParts[bodyParts.size() - 1].pos.y - bodyParts[bodyParts.size() - 1].lastDir.y * (25);
			}
			bodyParts.push_back(BodyPart(rad, x, y, window, delta));
		}

		void updateChilds() {
			for (int i = 0; i < bodyParts.size(); i++) {
			//for (int i = bodyParts.size() - 1; i >= 0; i--) {
				sf::Vector2f p, d;
				if(i == 0) {
					p = object.getPosition();
				}
				else {
					p = bodyParts[i - 1].object.getPosition();
				}
				//d = p - bodyParts[i].pos;
				d.x = p.x - bodyParts[i].object.getPosition().x;
				d.y = p.y - bodyParts[i].object.getPosition().y;
				float sq = sqrt(d.x * d.x + d.y * d.y);
				float x = d.x / sq;
				float y = d.y / sq;
				//bodyParts[i].setVelocity(x * 100.f, y * 100.f);
				float distSq = d.x * d.x + d.y * d.y;
				if (bodyParts[i].radius != radius) bodyParts[i].radius = radius;
				if (distSq >= 300) {
						bodyParts[i].move(x * linearVelocity * boost, y * boost * linearVelocity);
				}
				else
				{
						bodyParts[i].move(x * 20, y * 20);
				}
				bodyParts[i].object.setFillColor(object.getFillColor());
				bodyParts[i].object.setOutlineColor(object.getOutlineColor());
				bodyParts[i].update(false);
				//bodyParts[i].reDraw();
				//std::cout << i << std::endl;

			}
			for (int i = bodyParts.size() - 1; i >= 0; i--)
			{
				bodyParts[i].reDraw();
				//std::cout << bodyParts[i].pos.x;
			}

			
		}
		void reDrawBody() {
			for (int i = bodyParts.size() - 1; i >= 0; i--)
			{
				bodyParts[i].reDraw();
			}
			window->draw(object);
		}

		void setColor(sf::Color fill, sf::Color outline) {
			object.setFillColor(fill);
			object.setOutlineColor(outline);
			for (int i = bodyParts.size() - 1; i >= 0; i--)
			{
				bodyParts[i].object.setFillColor(fill);
				bodyParts[i].object.setOutlineColor(outline);
			}
		}

		void grow(int count = 1) {
			for (int i = 1; i <= count; i++) addChild();
		}
		void updateScore(int s) {
			score += s;
			growthBuffer += s;
			if (growthBuffer >= 4) {
				grow();
				radius += 0.25;
				growthBuffer = 0;
			}
		}
};		














#endif // !Entities_H

