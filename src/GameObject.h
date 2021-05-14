#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"
#include "ofxJoystick.h"

class GameObject {
public:
};

struct GameObjectData {
	enum ObjTypeIdx { myship = 0, ball, brick, bullet, effect };
	ObjTypeIdx object_type;
	bool is_hit = false;
	bool can_remove = false;
	ofVec2f pos;
	ofVec2f vec;
	float r;
	float bullet_speed_rate = 1.0;
};

class BulletData {
public:
	enum BulletImageIdx { round_white = 0, round_black, triangle_white, triangle_black, needle_white, needle_brack };
	BulletImageIdx bullet_img_type;
	ofVec2f pos;
	ofVec2f vec;
	float r = 0;
	int wait_time;
	bool is_homing;

	BulletData(ofVec2f _init_pos, ofVec2f _init_vec, float _r, int _wait_time, BulletImageIdx _bullet_img_type = round_white)
		:pos(_init_pos), vec(_init_vec), r(_r), wait_time(_wait_time), bullet_img_type(_bullet_img_type), is_homing(false)
	{}
	void setBulletImageType(BulletImageIdx _bullet_img_type) {
		bullet_img_type = bullet_img_type;
	}
	void setIsHoming(bool _is_homing) {
		is_homing = _is_homing;
	}
};

class Bullet
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
	}
};

class MyShip
{
private:
	std::unique_ptr<GameObjectData> private_data;
	bool is_moving[4] = { false,false,false,false };
	bool is_slow_move = false;
	int counter, rotate_deg, life, hit_anime_counter;
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
	void setSEVolume(float se_volume) {
		hit_se->setVolume(se_volume);
	}
	ofVec2f getPosition();
};

class Ball
{
private:
	std::unique_ptr<GameObjectData> private_data;
	std::unique_ptr<ofSoundPlayer> brick_hit_se,wall_hit_se;
	int window_width;
	int window_height;
	float speed = 3.0;
public:
	Ball(float _x, float _y, float _radius, int width, int height, float _se_volume);
	~Ball();
	bool canRemove();
	void draw();
	void update();
	ofVec2f getPosition() {
		return private_data->pos;
	}
	bool isHit(ofVec2f pos, ofVec2f shape);
};

class Brick
{
protected:
	std::unique_ptr<GameObjectData> private_data;
	ofVec2f pos;
	int width = 60;
	int height = 60;
	std::shared_ptr<MyShip> myship_copy;
public:
	Brick(float _x, float _y, float _v_y, std::shared_ptr<MyShip> _myship);
	~Brick();
	bool canRemove();
	bool isHit();
	void draw();
	void update();
	void setBulletSpeedRate(float _bullet_speed_rate) {
		private_data->bullet_speed_rate = _bullet_speed_rate;
	}
	void setRemoveable() {
		private_data->can_remove = true;
	}
	ofVec2f getPosition() {
		return private_data->pos;
	}
	ofVec2f getShape() {
		return ofVec2f(width, height);
	}
	virtual std::list<std::shared_ptr<BulletData>> makeBullet() = 0;
};

class Jikinerai_Single1 :public Brick {
private:

public:
	Jikinerai_Single1(float _x, float _y, float _v_y, std::shared_ptr<MyShip> _myship) :
		Brick(_x, _y, _v_y, _myship) {}
	std::list<std::shared_ptr<BulletData>> makeBullet() {
		std::list<std::shared_ptr<BulletData>> bullets;
		float speed = 6.0*private_data->bullet_speed_rate;
		bullets.emplace_back(
			std::make_unique<BulletData>(private_data->pos, ofVec2f(speed*(myship_copy->getPosition() - this->private_data->pos).normalize()), 10, 0, BulletData::triangle_black)
		);
		return bullets;
	}
};

class NWay_Around_Single1 :public Brick {
private:

	int n_way = 16;
public:
	NWay_Around_Single1(float _x, float _y, float _v_y, std::shared_ptr<MyShip> _myship) :
		Brick(_x, _y, _v_y, _myship) {}
	std::list<std::shared_ptr<BulletData>> makeBullet() {
		std::list<std::shared_ptr<BulletData>> bullets;
		float speed = 6.0*private_data->bullet_speed_rate;
		for (int i = 0; i < n_way; i++) {
			bullets.emplace_back(
				std::make_unique<BulletData>(private_data->pos, ofVec2f(speed*cos(2 * PI*i / n_way), speed*sin(2 * PI*i / n_way)), 10, 0, BulletData::round_white)
			);
		}
		return bullets;
	}
	void setNWay(int n) {
		n_way = n;
	}
};

class Jikinerai_Multiple1 :public Brick {
private:

public:
	Jikinerai_Multiple1(float _x, float _y, float _v_y, std::shared_ptr<MyShip> _myship) :
		Brick(_x, _y, _v_y, _myship) {}
	std::list<std::shared_ptr<BulletData>> makeBullet() {
		std::list<std::shared_ptr<BulletData>> bullets;
		float speed = 6.0*private_data->bullet_speed_rate;
		for (int i = 0; i < 8; i++) {
			bullets.emplace_back(
				std::make_unique<BulletData>(private_data->pos, ofVec2f(speed*(myship_copy->getPosition() - this->private_data->pos).normalize()), 10, i * 4, BulletData::triangle_black)
			);
		}
		return bullets;
	}
};

class FourWay_Guruguru1 :public Brick {
private:

public:
	FourWay_Guruguru1(float _x, float _y, float _v_y, std::shared_ptr<MyShip> _myship) :
		Brick(_x, _y, _v_y, _myship) {}
	std::list<std::shared_ptr<BulletData>> makeBullet() {
		std::list<std::shared_ptr<BulletData>> bullets;
		float speed = 4.0*private_data->bullet_speed_rate;
		for (int j = 0; j < 16; j++) {
			for (int i = 0; i < 4; i++) {
				bullets.emplace_back(
					std::make_unique<BulletData>(private_data->pos, ofVec2f(speed*cos(j * 19 * DEG_TO_RAD + 2 * PI* i / 4), speed*sin(j * 19 * DEG_TO_RAD + 2 * PI* i / 4)), 10, j * 2, BulletData::needle_white)
				);
			}
		}
		return bullets;
	}
};

class NWay_Around_Multiple1 :public Brick {
private:

	int n_way = 8;
public:
	NWay_Around_Multiple1(float _x, float _y, float _v_y, std::shared_ptr<MyShip> _myship) :
		Brick(_x, _y, _v_y, _myship) {}
	std::list<std::shared_ptr<BulletData>> makeBullet() {
		std::list<std::shared_ptr<BulletData>> bullets;
		float speed = 4.0*private_data->bullet_speed_rate;
		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < n_way; i++) {
				bullets.emplace_back(
					std::make_unique<BulletData>(private_data->pos, ofVec2f(speed*cos(j * 6 * DEG_TO_RAD + 2 * PI*i / n_way), speed*sin(j * 6 * DEG_TO_RAD + 2 * PI*i / n_way)), 10, j, BulletData::round_white)
				);
			}
		}
		return bullets;
	}
	void setNWay(int n) {
		n_way = n;
	}
};

class Jikinerai_NWay_Single1 :public Brick {
private:

	int n_way = 5;
public:
	Jikinerai_NWay_Single1(float _x, float _y, float _v_y, std::shared_ptr<MyShip> _myship) :
		Brick(_x, _y, _v_y, _myship) {}
	std::list<std::shared_ptr<BulletData>> makeBullet() {
		assert(n_way > 1);
		std::list<std::shared_ptr<BulletData>> bullets;
		float speed = 6.0*private_data->bullet_speed_rate;
		float angle = std::atan2(myship_copy->getPosition().y - this->private_data->pos.y, myship_copy->getPosition().x - this->private_data->pos.x);
		for (int i = 0; i < n_way; i++) {
			bullets.emplace_back(
				std::make_unique<BulletData>(private_data->pos, ofVec2f(speed*cos(angle + (i * 30 / (n_way - 1) - 15)*DEG_TO_RAD), speed*sin(angle + (i * 30 / (n_way - 1) - 15)* DEG_TO_RAD)), 10, 0, BulletData::triangle_white)
			);
		}
		return bullets;
	}
	void setNWay(int n) {
		n_way = n;
	}
};

class Jikinerai_NWay_Single2 :public Brick {
private:

	int n_way = 7;
public:
	Jikinerai_NWay_Single2(float _x, float _y, float _v_y, std::shared_ptr<MyShip> _myship) :
		Brick(_x, _y, _v_y, _myship) {}
	std::list<std::shared_ptr<BulletData>> makeBullet() {
		assert(n_way > 1);
		std::list<std::shared_ptr<BulletData>> bullets;
		float speed = 6.0*private_data->bullet_speed_rate;
		float angle = std::atan2(myship_copy->getPosition().y - this->private_data->pos.y, myship_copy->getPosition().x - this->private_data->pos.x);
		for (int i = 0; i < n_way; i++) {
			bullets.emplace_back(
				std::make_unique<BulletData>(private_data->pos, ofVec2f(speed*cos(angle + (i * 60 / (n_way - 1) - 30)*DEG_TO_RAD), speed*sin(angle + (i * 60 / (n_way - 1) - 30)* DEG_TO_RAD)), 10, 0, BulletData::triangle_white)
			);
		}
		return bullets;
	}
	void setNWay(int n) {
		n_way = n;
	}
};

class NWay_Around_Big1 :public Brick {
private:

	int n_way = 12;
public:
	NWay_Around_Big1(float _x, float _y, float _v_y, std::shared_ptr<MyShip> _myship) :
		Brick(_x, _y, _v_y, _myship) {}
	std::list<std::shared_ptr<BulletData>> makeBullet() {
		std::list<std::shared_ptr<BulletData>> bullets;
		float speed = 5.0*private_data->bullet_speed_rate;
		for (int i = 0; i < n_way; i++) {
			for (int j = 0; j < 4; j++) {
				bullets.emplace_back(
					std::make_unique<BulletData>(private_data->pos, ofVec2f(speed*cos(2 * PI*i / n_way), speed*sin(2 * PI*i / n_way)), 25 - j * 5, j * 10, BulletData::round_white)
				);
			}
		}
		return bullets;
	}
	void setNWay(int n) {
		n_way = n;
	}
};

class NWay_Around_Kasoku1 :public Brick {
private:

	int n_way = 6;
public:
	NWay_Around_Kasoku1(float _x, float _y, float _v_y, std::shared_ptr<MyShip> _myship) :
		Brick(_x, _y, _v_y, _myship) {}
	std::list<std::shared_ptr<BulletData>> makeBullet() {
		std::list<std::shared_ptr<BulletData>> bullets;
		float speed = 4.0*private_data->bullet_speed_rate;
		for (int j = 0; j < 8; j++) {
			for (int i = 0; i < n_way; i++) {
				bullets.emplace_back(
					std::make_unique<BulletData>(private_data->pos, ofVec2f(speed*cos(j * 6 * DEG_TO_RAD + 2 * PI*i / n_way), speed*sin(j * 6 * DEG_TO_RAD + 2 * PI*i / n_way)), 10, j * 4, BulletData::round_white)
				);
				speed *= 1.02;
			}
		}
		return bullets;
	}
	void setNWay(int n) {
		n_way = n;
	}
};

class Hibachi1 :public Brick {
private:

public:
	Hibachi1(float _x, float _y, float _v_y, std::shared_ptr<MyShip> _myship) :
		Brick(_x, _y, _v_y, _myship) {
		width = 50;
		height = 50;
	}
	std::list<std::shared_ptr<BulletData>> makeBullet() {
		std::list<std::shared_ptr<BulletData>> bullets;
		float speed = 9.0*private_data->bullet_speed_rate;
		for (int j = 0; j < 64; j++) {
			for (int i = 0; i < 12; i++) {
				if (j % 8 < 6) {
					bullets.emplace_back(
						std::make_unique<BulletData>(private_data->pos, ofVec2f(speed*cos(j  * DEG_TO_RAD + 2 * PI*i / 12), speed*sin(j * DEG_TO_RAD + 2 * PI*i / 12)), 15, j * 2, BulletData::round_black)
					);
					bullets.emplace_back(
						std::make_unique<BulletData>(private_data->pos, ofVec2f(speed*cos(-j * DEG_TO_RAD + 2 * PI*i / 12), speed*sin(-j * DEG_TO_RAD + 2 * PI*i / 12)), 12, j * 2, BulletData::round_black)
					);
				}
			}
		}
		return bullets;
	}
};