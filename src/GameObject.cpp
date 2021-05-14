#include "GameObject.h"

Ball::Ball(float _x, float _y, float _radius, int width, int height, float _se_volume)
{
	private_data = make_unique<GameObjectData>();
	private_data->object_type = GameObjectData::ball;
	private_data->is_hit = false;
	private_data->can_remove = false;
	private_data->pos = ofVec2f(_x, _y);
	private_data->vec = ofVec2f(2, 2);
	private_data->r = _radius;

	window_width = width;
	window_height = height;

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
	private_data->pos += private_data->vec;
	
	if ((private_data->pos.x - private_data->r) < window_width / 4) {
		private_data->pos.x = window_width  / 4 + private_data->r;
		private_data->vec.x *= -1;
		wall_hit_se->play();
	}
	else if (window_width * 3 / 4 < (private_data->pos.x + private_data->r)) {
		private_data->pos.x = window_width * 3 / 4 - private_data->r;
		private_data->vec.x *= -1;
		wall_hit_se->play();
	}
	else if ((private_data->pos.y - private_data->r) < 0) {
		private_data->pos.y = private_data->r;
		private_data->vec.y *= -1;
		wall_hit_se->play();
	}
	else if (window_height < (private_data->pos.y + private_data->r)) {
		private_data->pos.y = window_height - private_data->r;
		private_data->vec.y *= -1;
		wall_hit_se->play();
	}
}

bool Ball::isHit(ofVec2f pos, ofVec2f shape)
{
	if ((pos-private_data->pos).length()<42)
	{
		return true;
	}
	return false;
}

bool Ball::canRemove()
{
	return private_data->can_remove;
}

void Ball::draw()
{
	ofPushMatrix();
	ofTranslate(private_data->pos);
	ofFill();
	ofSetColor(255, 214, 98);
	ofDrawCircle(0, 0, private_data->r);
	ofPopMatrix();
}

Bullet::Bullet()
{
	private_data = make_unique<GameObjectData>();
}

void Bullet::set(const std::shared_ptr<BulletData>& _data)
{
	private_data->pos = _data->pos;
	private_data->vec = _data->vec;
	private_data->r = _data->r;
	private_data->object_type = GameObjectData::bullet;
	private_data->is_hit = false;
	private_data->can_remove = false;

	this->bullet_img_type = _data->bullet_img_type;
	wait_time = _data->wait_time;
	play_shotse = false;
	counter = 0;

	this->angle = std::atan2(private_data->vec.y, private_data->vec.x);
	pos1 = ofVec2f(private_data->r*cosf(angle), private_data->r*sinf(angle));
	pos2 = ofVec2f(private_data->r*cosf(angle + 2 * PI / 3), private_data->r*sinf(angle + 2 * PI / 3));
	pos3 = ofVec2f(private_data->r*cosf(angle - 2 * PI / 3), private_data->r*sinf(angle - 2 * PI / 3));
}

Bullet::~Bullet()
{
}

bool Bullet::canRemove()
{
	return private_data->can_remove;
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
		ofDrawCircle(private_data->pos, private_data->r);
		ofDrawCircle(private_data->pos, private_data->r*0.5);
		ofFill();
		break;
	case BulletData::round_black:
		ofNoFill();
		ofDrawCircle(private_data->pos, private_data->r);
		ofFill();
		ofDrawCircle(private_data->pos, private_data->r*0.5);
		break;
	case BulletData::triangle_white:
		ofNoFill();
		ofPushMatrix();
		ofTranslate(this->getPosition());
		ofDrawCircle(0, 0, private_data->r);
		ofDrawTriangle(pos1, pos2, pos3);
		ofPopMatrix();
		ofFill();
		break;
	case BulletData::triangle_black:
		ofNoFill();
		ofPushMatrix();
		ofTranslate(this->getPosition());
		ofDrawCircle(0, 0, private_data->r);
		ofFill();
		ofDrawTriangle(pos1, pos2, pos3);
		ofPopMatrix();
		break;
	case BulletData::needle_white:
		ofNoFill();
		ofPushMatrix();
		ofTranslate(this->getPosition());
		ofRotateRad(angle);
		ofDrawRectangle(-private_data->r, -private_data->r / 2, private_data->r * 2, private_data->r);
		ofFill();
		ofPopMatrix();
		break;
	case BulletData::needle_brack:
		ofFill();
		ofPushMatrix();
		ofTranslate(this->getPosition());
		ofRotateRad(angle);
		ofDrawRectangle(-private_data->r/2, -private_data->r, private_data->r, private_data->r*2);
		ofPopMatrix();
		break;
	default://round_white
		ofNoFill();
		ofDrawCircle(private_data->pos, private_data->r);
		ofDrawCircle(private_data->pos, private_data->r*0.5);
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

	private_data->pos += private_data->vec;

	if (private_data->pos.x < 0 || 1280 < private_data->pos.x ||
		private_data->pos.y < -100 || 720 + 100 < private_data->pos.y ||
		private_data->is_hit
		)
	{
		private_data->can_remove = true;
	}

	this->counter++;
}

ofVec2f Bullet::getPosition()
{
	return private_data->pos;
}

Brick::Brick(float _x, float _y, float _v_y, std::shared_ptr<MyShip> _myship)
{
	private_data = make_unique<GameObjectData>();
	private_data->object_type = GameObjectData::brick;
	private_data->is_hit = false;
	private_data->can_remove = false;
	private_data->pos = ofVec2f(_x, _y);
	private_data->vec = ofVec2f(0, _v_y);
	private_data->bullet_speed_rate = 1.0;
	myship_copy = _myship;
}

Brick::~Brick()
{}

void Brick::draw()
{
	ofPushMatrix();

	ofTranslate(private_data->pos);
	ofSetColor(255, 214, 98);
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
	private_data->pos += private_data->vec;

	if (this->private_data->pos.y > 720 + 50 || this->private_data->pos.y < -50 || private_data->is_hit) {
		//makeBullet();
		private_data->can_remove = true;
	}
}

bool Brick::canRemove()
{
	return private_data->can_remove;
}

bool Brick::isHit()
{
	return private_data->is_hit;
}

MyShip::MyShip() :
	counter(0), rotate_deg(0), life(3), hit_anime_counter(0)
{
	private_data = make_unique<GameObjectData>();
	private_data->object_type = GameObjectData::myship;
	private_data->is_hit = false;
	private_data->can_remove = false;
	private_data->pos = ofVec2f(1280 / 2, 720 - 100);
	private_data->vec = ofVec2f(5, 5);

	joy_.setup(GLFW_JOYSTICK_1);

	this->hit_se = std::make_unique<ofSoundPlayer>();
	hit_se->load("hitse01.mp3");
}

void MyShip::update()
{
	if (counter < 1000000) {
		counter++;
	}
	else {
		counter = 0;
	}

	if (hit_anime_counter > 0) {
		hit_anime_counter--;
	}
	if (life < 1)
	{
		private_data->can_remove = true;
		return;
	}

	ofVec2f v = private_data->vec;

	if (joy_.isPushing(0) || is_slow_move) {
		v *= 0.5;
	}

	private_data->pos.x += v.x * joy_.getAxis(0);
	private_data->pos.y += v.y * joy_.getAxis(1);

	if (is_moving[0]) {
		private_data->pos.y -= v.y;
	}
	if (is_moving[1]) {
		private_data->pos.y += v.y;
	}
	if (is_moving[2]) {
		private_data->pos.x -= v.x;
	}
	if (is_moving[3]) {
		private_data->pos.x += v.x;
	}

	if (private_data->pos.x < 1280 / 4) {
		private_data->pos.x = 1280 / 4;
	}
	if (1280 * 3 / 4 < private_data->pos.x) {
		private_data->pos.x = 1280 * 3 / 4;
	}
	if (private_data->pos.y < 0) {
		private_data->pos.y = 0;
	}
	if (720 < private_data->pos.y) {
		private_data->pos.y = 720;
	}

	if (private_data->is_hit) {
		private_data->is_hit = false;
		if (hit_anime_counter > 0) {
			return;
		}
		//else
		life--;
		hit_se->play();
		hit_anime_counter = 60;
	}
}

void MyShip::draw()
{
	if (private_data->is_hit) ofDrawBitmapString("Hit", 1280 / 2, 720 - 150);
	if (life == 3) ofDrawBitmapString("LIFE:3", 1280 / 2, 720 - 100);
	if (life == 2) ofDrawBitmapString("LIFE:2", 1280 / 2, 720 - 100);
	if (life == 1) ofDrawBitmapString("LIFE:1", 1280 / 2, 720 - 100);

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
	ofDrawCircle(private_data->pos, 5.0);

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
	return private_data->pos;
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
	private_data->pos = ofVec2f(_x, _y);
}