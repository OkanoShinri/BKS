#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxJoystick.h"

class GameObject {
public:
};

class GameObjectData {
public:
	enum ObjTypeIdx { myship = 0, ball, brick, bullet, effect };
	ObjTypeIdx object_type;
	bool is_hit = false;
	bool can_remove = false;
	ofVec2f pos;
	ofVec2f vec;
	float r = 0;
};

class BulletData {
public:
	enum BulletImageIdx { round_white = 0, round_black, triangle_white, triangle_black };
	BulletImageIdx bullet_img_type;
	ofVec2f pos;
	ofVec2f vec;
	float r = 0;
	int wait_time;
	bool is_homing;

	BulletData(ofVec2f _init_pos, ofVec2f _init_vec, float _r, int _wait_time)
		:pos(_init_pos), vec(_init_vec), r(_r), wait_time(_wait_time), is_homing(false)
	{};
	void setBulletImageType(BulletImageIdx _bullet_img_type) {
		bullet_img_type = bullet_img_type;
	};
	void setIsHoming(bool _is_homing) {
		is_homing = _is_homing;
	};
};

class Bullet :public GameObject
{
private:
	std::unique_ptr<GameObjectData> private_data;
	int counter = 0;
	int wait_time = 0;
	float angle;
	BulletData::BulletImageIdx bullet_img_type;
	ofVec2f pos1, pos2, pos3;
public:
	Bullet();
	~Bullet();
	void set(const std::shared_ptr<BulletData>& _data);
	bool canRemove();
	bool play_shotse;
	void draw();
	void update();
	ofVec2f getPosition();
	float getRadius() {
		return private_data->r;
	};
};

class MyShip :public GameObject
{
private:
	std::unique_ptr<GameObjectData> private_data;
	bool is_moving[4] = { false,false,false,false };
	bool is_slow_move = false;
	int counter;
	int rotate_deg;
	int life;
	int hit_anime_counter;
	ofxJoystick joy_;
	std::unique_ptr<ofSoundPlayer> hit_se;
public:
	MyShip();
	~MyShip() { std::cout << "Remove: MyShip" << std::endl; }

	bool canRemove() {
		return private_data->can_remove;
	}
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void update();
	void draw();
	void hit() {
		private_data->is_hit = true;
	}
	void setRemoveable() {
		private_data->can_remove = true;
	}
	ofVec2f getPosition();
};

class Ball :public ofxBox2dCircle
{
private:
	GameObjectData* private_data;
public:
	Ball(b2World * _b2dworld, float _x, float _y, float _radius, float _v_x, float _v_y);
	~Ball();
	bool canRemove();
	void draw();
	void update();
};

class Brick :public ofxBox2dRect {
private:
	GameObjectData* private_data;
	std::list<std::shared_ptr<BulletData>> bullets;
	virtual void makeBullet();
public:
	Brick(b2World * _b2dworld, float _x, float _y, float _width, float _height, float _v_y);
	~Brick();
	bool canRemove();
	void draw();
	void update();
	virtual std::list<std::shared_ptr<BulletData>> getBullet() {
		return bullets;
	};

	float bullet_speed_rate = 1.0;
	void setBulletSpeedRate(float _bullet_speed_rate) {
		bullet_speed_rate = _bullet_speed_rate;
	}
};

class NWayBrick_1 :public Brick {
private:
	GameObjectData* private_data;
	std::list<std::shared_ptr<BulletData>> bullets;
	int n_way = 12;

	void makeBullet() {
		for (int j = 0; j < 6; j++) {
			for (int i = 0; i < n_way; i++) {
				float speed = 3.0;
				bullets.emplace_back(
					std::make_unique<BulletData>(getPosition(), ofVec2f(speed*cosf((360 * i / n_way + j * 3)*DEG_TO_RAD), speed*sinf((360 * i / n_way + j * 3)*DEG_TO_RAD)), 12, j * 5)
				);
			}
		}
	};
public:
	using Brick::Brick;
	std::list<std::shared_ptr<BulletData>> getBullet() {
		return bullets;
	};
	void setNWay(int _n_way) {
		n_way = _n_way;
	}
};

class NWayBrick_2 :public Brick {
private:
	GameObjectData* private_data;
	std::list<std::shared_ptr<BulletData>> bullets;
	int n_way = 16;

	void makeBullet() {
		for (int j = 0; j < 6; j++) {
			for (int i = 0; i < n_way; i++) {
				float speed = 2.5;
				bullets.emplace_back(
					std::make_unique<BulletData>(getPosition(), ofVec2f(speed*cosf((360 * i / n_way + j * 12)*DEG_TO_RAD), speed*sinf((360 * i / n_way + j * 12)*DEG_TO_RAD)), 12, j * 10)
				);
			}
		}
	};
public:
	using Brick::Brick;
	std::list<std::shared_ptr<BulletData>> getBullet() {
		return bullets;
	};
	void setNWay(int _n_way) {
		n_way = _n_way;
	}
};

/* too complicated
class NWayBrick_3 :public Brick {
private:
	GameObjectData* private_data;
	std::list<std::shared_ptr<BulletData>> bullets;

	void makeBullet() {
		float speed = 5.5;
		for (int k = -1; k < 3; k += 2) {
			for (int j = 0; j < 6; j++) {
				for (int i = 0; i < 12; i++) {
					bullets.emplace_back(
						std::make_unique<BulletData>(getPosition(), ofVec2f(speed * cos(2 * PI* i / 12 + j * k * 2 * PI * 1 / 360), speed * sin(2 * PI* i / 12 + j * k * 2 * PI * 1 / 360)), 12, j * 3 + (k + 1) * 15,0)//( X , Y , V_X , V_Y , Radius ,  TIME )
					);
				}
			}
		}
	};
public:
	using Brick::Brick;
	std::list<std::shared_ptr<BulletData>> getBullet() {
		return bullets;
	};
};
*/

class Jikinerai_Brick :public Brick {
private:
	GameObjectData* private_data;
	std::list<std::shared_ptr<BulletData>> bullets;

	void makeBullet() {
		for (int i = 0; i < 8; i++) {
			bullets.emplace_back(
				std::make_unique<BulletData>(getPosition(), ofVec2f(NULL, NULL), 10, i * 3)
			);
		}
	};
public:
	using Brick::Brick;
	std::list<std::shared_ptr<BulletData>> getBullet() {
		return bullets;
	};
};
