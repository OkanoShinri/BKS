#include "GameObject.h"

Ball::Ball(ofVec2f pos, ofVec2f vec, int width, int height, float _se_volume)
{
	is_hit = false;
	object_type = ball;
	r = radius;
	speed = vec.length();
	this->pos = pos;
	this->vec = vec;
	window_height = height;
	window_width = width;

	brick_hit_se = std::make_unique<ofSoundPlayer>();
	brick_hit_se->load("se02.mp3");
	brick_hit_se->setVolume(_se_volume);
	brick_hit_se->setMultiPlay(true);

	wall_hit_se = std::make_unique<ofSoundPlayer>();
	wall_hit_se->load("se01.mp3");
	wall_hit_se->setVolume(_se_volume);
	wall_hit_se->setMultiPlay(true);
}

Ball::~Ball()
{
}

void Ball::update()
{
	pos += vec;

	if ((pos.x - r) < window_width / 4) {
		pos.x = window_width / 4 + r;
		vec.x *= -1;
		wall_hit_se->play();
	}
	else if (window_width * 3 / 4 < (pos.x + r)) {
		pos.x = window_width * 3 / 4 - r;
		vec.x *= -1;
		wall_hit_se->play();
	}
	else if ((pos.y - r) < 0) {
		pos.y = r;
		vec.y *= -1;
		wall_hit_se->play();
	}
	else if (window_height < (pos.y + r)) {
		pos.y = window_height - r;
		vec.y *= -1;
		wall_hit_se->play();
	}
}

bool Ball::isHit(ofVec2f brick_pos, ofVec2f shape)
{
	if (
		brick_pos.x - shape.x / 2 - r < pos.x && pos.x < brick_pos.x + shape.x / 2 + r
		&&
		brick_pos.y - shape.y / 2 - r < pos.y && pos.y < brick_pos.y + shape.y / 2 + r
		)
	{
		if (brick_pos.x - shape.x / 2 < pos.x && pos.x < brick_pos.x + shape.x / 2)
		{
			vec.y *= -1;
		}
		else if (brick_pos.y - shape.y / 2 < pos.y && pos.y < brick_pos.y + shape.y / 2)
		{
			vec.x *= -1;
		}
		else if (pos.x < brick_pos.x - shape.x / 2 && pos.y < brick_pos.y - shape.y / 2)
		{
			vec = ofVec2f(-speed, -speed);
		}
		else if (brick_pos.x + shape.x / 2 < pos.x && pos.y < brick_pos.y - shape.y / 2)
		{
			vec = ofVec2f(speed, -speed);
		}
		else if (brick_pos.x + shape.x / 2 < pos.x &&  brick_pos.y + shape.y / 2 < pos.y)
		{
			vec = ofVec2f(speed, speed);
		}
		else if (brick_pos.x < pos.x - shape.x / 2 && brick_pos.y + shape.y / 2 < pos.y)
		{
			vec = ofVec2f(-speed, speed);
		}
		brick_hit_se->play();
		return true;
	}
	return false;
}

bool Ball::canRemove()
{
	return false;
}

void Ball::draw()
{
	ofPushMatrix();
	ofTranslate(pos);
	ofFill();
	ofSetColor(255, 214, 98);
	ofDrawCircle(0, 0, r);
	ofNoFill();
	ofSetColor(0, 0, 0);
	ofDrawCircle(0, 0, r);
	ofFill();
	ofPopMatrix();
}

Bullet::Bullet()
{
}

void Bullet::set(const std::shared_ptr<BulletData>& _data)
{
	pos = _data->pos;
	vec = _data->vec;
	r = _data->r;
	object_type = bullet;
	can_remove = false;

	this->bullet_img_type = _data->bullet_img_type;
	wait_time = _data->wait_time;
	play_shotse = false;
	counter = 0;

	this->angle = std::atan2(vec.y, vec.x);
	pos1 = ofVec2f(r*cosf(angle), r*sinf(angle));
	pos2 = ofVec2f(r*cosf(angle + 2 * PI / 3), r*sinf(angle + 2 * PI / 3));
	pos3 = ofVec2f(r*cosf(angle - 2 * PI / 3), r*sinf(angle - 2 * PI / 3));
}

Bullet::~Bullet()
{
}

bool Bullet::canRemove()
{
	return can_remove;
}

void Bullet::draw()
{
	if (counter < wait_time) {
		return;
	}
	ofSetColor(0, 0, 0);
	switch (bullet_img_type)
	{
	case BulletData::round_white:
		ofNoFill();
		ofDrawCircle(pos, r);
		ofDrawCircle(pos, r*0.5);
		ofFill();
		break;
	case BulletData::round_black:
		ofNoFill();
		ofDrawCircle(pos, r);
		ofFill();
		ofDrawCircle(pos, r*0.5);
		break;
	case BulletData::triangle_white:
		ofNoFill();
		ofPushMatrix();
		ofTranslate(this->getPosition());
		ofDrawCircle(0, 0, r);
		ofDrawTriangle(pos1, pos2, pos3);
		ofPopMatrix();
		ofFill();
		break;
	case BulletData::triangle_black:
		ofNoFill();
		ofPushMatrix();
		ofTranslate(this->getPosition());
		ofDrawCircle(0, 0, r);
		ofFill();
		ofDrawTriangle(pos1, pos2, pos3);
		ofPopMatrix();
		break;
	case BulletData::needle_white:
		ofNoFill();
		ofPushMatrix();
		ofTranslate(this->getPosition());
		ofRotateRad(angle);
		ofDrawRectangle(-r, -r / 2, r * 2, r);
		ofFill();
		ofPopMatrix();
		break;
	case BulletData::needle_brack:
		ofFill();
		ofPushMatrix();
		ofTranslate(this->getPosition());
		ofRotateRad(angle);
		ofDrawRectangle(-r / 2, -r, r, r * 2);
		ofPopMatrix();
		break;
	default://round_white
		ofNoFill();
		ofDrawCircle(pos, r);
		ofDrawCircle(pos, r*0.5);
		ofFill();
		break;
	}
}

void Bullet::update()
{
	if (counter < wait_time) {
		this->counter++;
		return;
	}
	if (counter == wait_time + 1) {
		play_shotse = true;
	}
	else {
		play_shotse = false;
	}

	pos += vec;

	if (pos.x < 0 || 1280 < pos.x ||
		pos.y < -100 || 720 + 100 < pos.y)
	{
		can_remove = true;
	}

	this->counter++;
}

ofVec2f Bullet::getPosition()
{
	return pos;
}

Brick::Brick(float _x, float _y, float _v_y, std::shared_ptr<MyShip> _myship)
{
	object_type = brick;
	is_hit = false;
	can_remove = false;
	pos = ofVec2f(_x, _y);
	vec = ofVec2f(0, _v_y);
	bullet_speed_rate = 1.0;
	myship_copy = _myship;
}

Brick::~Brick()
{}

void Brick::draw()
{
	ofPushMatrix();

	ofTranslate(pos);
	ofSetColor(255, 0, 0);
	ofSetLineWidth(2.0);
	ofDrawLine(-width / 2, -height / 2, width / 2, -height / 2);
	ofDrawLine(width / 2, -height / 2, width / 2, height / 2);
	ofDrawLine(width / 2, height / 2, -width / 2, height / 2);
	ofDrawLine(-width / 2, height / 2, -width / 2, -height / 2);

	ofFill();
	ofSetColor(0, 0, 0);
	ofDrawCircle(-width / 2, -height / 2, 2);
	ofDrawCircle(width / 2, -height / 2, 2);
	ofDrawCircle(width / 2, height / 2, 2);
	ofDrawCircle(-width / 2, height / 2, 2);

	ofPopMatrix();

	ofFill();
}

void Brick::update()
{
	pos += vec;

	if (this->pos.y > 720 + 50 || this->pos.y < -50 || is_hit) {
		can_remove = true;
	}
}

bool Brick::canRemove()
{
	return can_remove;
}

bool Brick::isHit()
{
	return is_hit;
}

MyShip::MyShip() :
	counter(0), rotate_deg(0), life(3), hit_anime_counter(0)
{
	object_type = myship;
	is_hit = false;
	can_remove = false;
	pos = ofVec2f(1280 / 2, 720 - 100);
	vec = ofVec2f(5, 5);

	joy_.setup(GLFW_JOYSTICK_1);

	this->hit_se = std::make_unique<ofSoundPlayer>();
	hit_se->load("hitse01.mp3");
}

void MyShip::update()
{
	counter++;
	
	if (hit_anime_counter > 0) {
		hit_anime_counter--;
	}
	if (life < 1)
	{
		can_remove = true;
		return;
	}

	ofVec2f v = vec;

	if (joy_.isPushing(0) || is_slow_move) {
		v *= 0.5;
	}

	pos.x += v.x * joy_.getAxis(0);
	pos.y += v.y * joy_.getAxis(1);

	if (is_moving[0]) {
		pos.y -= v.y;
	}
	if (is_moving[1]) {
		pos.y += v.y;
	}
	if (is_moving[2]) {
		pos.x -= v.x;
	}
	if (is_moving[3]) {
		pos.x += v.x;
	}

	if (pos.x < 1280 / 4) {
		pos.x = 1280 / 4;
	}
	if (1280 * 3 / 4 < pos.x) {
		pos.x = 1280 * 3 / 4;
	}
	if (pos.y < 0) {
		pos.y = 0;
	}
	if (720 < pos.y) {
		pos.y = 720;
	}

	if (is_hit) {
		is_hit = false;
		if (hit_anime_counter > 0) {
			return;
		}
		life--;
		hit_se->play();
		hit_anime_counter = 60;
	}
}

void MyShip::draw()
{
	ofNoFill();

	if (hit_anime_counter > 0) {
		if (life == 2)
		{
			ofSetColor(ofMap(hit_anime_counter, 60, 0, 255, 30), 30, 30, ofMap(hit_anime_counter, 60, 0, 255, 0));

			ofPushMatrix();
			ofTranslate(getPosition());
			ofRotateDeg(rotate_deg);
			ofDrawRectangle(-21, -21, 42, 42);
			ofDrawCircle(0, 0, 21 * 1.4);
			ofPopMatrix();

			ofSetColor(30, 30, 30, 255);
			ofPushMatrix();
			ofTranslate(getPosition());
			ofRotateDeg(-rotate_deg);
			ofDrawRectangle(-15, -15, 30, 30);
			ofDrawCircle(0, 0, 15 * 1.4);
			ofPopMatrix();
		}
		if (life == 1)
		{
			ofSetColor(ofMap(hit_anime_counter, 60, 0, 255, 30), 30, 30, ofMap(hit_anime_counter, 60, 0, 255, 0));

			ofPushMatrix();
			ofTranslate(getPosition());
			ofRotateDeg(-rotate_deg);
			ofDrawRectangle(-15, -15, 30, 30);
			ofDrawCircle(0, 0, 15 * 1.4);
			ofPopMatrix();

			ofSetColor(30, 30, 30, 255);
		}
	}
	else {
		ofSetColor(30, 30, 30);
		if (life == 3)
		{
			ofPushMatrix();
			ofTranslate(getPosition());
			ofRotateDeg(rotate_deg);
			ofDrawRectangle(-21, -21, 42, 42);
			ofDrawCircle(0, 0, 21 * 1.4);
			ofPopMatrix();

			ofPushMatrix();
			ofTranslate(getPosition());
			ofRotateDeg(-rotate_deg);
			ofDrawRectangle(-15, -15, 30, 30);
			ofDrawCircle(0, 0, 15 * 1.4);
			ofPopMatrix();
		}
		if (life == 2)
		{
			ofPushMatrix();
			ofTranslate(getPosition());
			ofRotateDeg(-rotate_deg);
			ofDrawRectangle(-15, -15, 30, 30);
			ofDrawCircle(0, 0, 15 * 1.4);
			ofPopMatrix();
		}
	}

	if (life == 0) {
		ofSetColor(255, 30, 30);
	}
	ofFill();
	ofDrawCircle(pos, 5.0);

	if (joy_.isPushing(0)) {
		rotate_deg++;
	}
	else {
		rotate_deg += 2;
	}

	if (rotate_deg > 10000000) {
		rotate_deg = 0;
	}
}

ofVec2f MyShip::getPosition()
{
	return pos;
}

void MyShip::keyPressed(int key)
{
	switch (key) {
	case OF_KEY_UP:
		this->is_moving[0] = true;
		break;
	case OF_KEY_DOWN:
		this->is_moving[1] = true;
		break;
	case OF_KEY_LEFT:
		this->is_moving[2] = true;
		break;
	case OF_KEY_RIGHT:
		this->is_moving[3] = true;
		break;
	case OF_KEY_SHIFT:
		this->is_slow_move = true;
		break;
	default:
		break;
	}
}

void MyShip::keyReleased(int key)
{
	switch (key) {
	case OF_KEY_UP:
		this->is_moving[0] = false;
		break;
	case OF_KEY_DOWN:
		this->is_moving[1] = false;
		break;
	case OF_KEY_LEFT:
		this->is_moving[2] = false;
		break;
	case OF_KEY_RIGHT:
		this->is_moving[3] = false;
		break;
	case OF_KEY_SHIFT:
		this->is_slow_move = false;
		break;
	default:
		break;
	}
}

void MyShip::mouseMoved(int _x, int _y)
{
	pos = ofVec2f(_x, _y);
}
