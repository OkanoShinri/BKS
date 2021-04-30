#include "GameObject.h"

Ball::Ball(b2World * _b2dworld, float _x, float _y, float _radius, float _v_x, float _v_y)
{
	setPhysics(1.0, 1.0, 0.0);
	setup(_b2dworld, _x, _y, _radius);
	setVelocity(_v_x, _v_y);

	this->setData(new GameObjectData());
	private_data = (GameObjectData*)getData();
	private_data->object_type = GameObjectData::ball;
	private_data->is_hit = false;
	private_data->can_remove = false;
}

Ball::~Ball()
{
}

void Ball::update()
{
	if (abs(getVelocity().x) < 2.0 || abs(getVelocity().y) < 2.0) {
		setVelocity(10, 10);
	}
	if (this->getPosition().y > ofGetHeight() + 10) {
		private_data->can_remove = true;
	}
}

bool Ball::canRemove()
{
	return private_data->can_remove;
}

void Ball::draw()
{
	ofPushMatrix();
	ofTranslate(this->getPosition());
	ofFill();
	ofSetColor(255, 214, 98);
	ofDrawCircle(0, 0, getRadius());
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

	this->angle = std::atan2(private_data->vec.x, private_data->vec.y);
	pos1 = ofVec2f(private_data->r*sinf(angle), private_data->r*cosf(angle));
	pos2 = ofVec2f(private_data->r*sinf(angle + 2 * PI / 3), private_data->r*cosf(angle + 2 * PI / 3));
	pos3 = ofVec2f(private_data->r*sinf(angle - 2 * PI / 3), private_data->r*cosf(angle - 2 * PI / 3));

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
	default://round_white
		ofNoFill();
		ofDrawCircle(private_data->pos, private_data->r);
		ofDrawCircle(private_data->pos, private_data->r*0.5);
		ofFill();
		break;
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

	if (private_data->pos.x < 0 || ofGetWidth() < private_data->pos.x ||
		private_data->pos.y < -100 || ofGetHeight() + 100 < private_data->pos.y ||
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

Brick::Brick(b2World * _b2dworld, float _x, float _y, float _width, float _height, float _v_y)
{
	this->setPhysics(0.0, 1.0, 0.0);
	this->setup(_b2dworld, _x, _y, _width, _height);

	this->setData(new GameObjectData());
	this->private_data = (GameObjectData*)getData();
	this->private_data->object_type = GameObjectData::brick;
	this->private_data->is_hit = false;
	this->private_data->can_remove = false;

	this->private_data->vec.y = _v_y;
}

Brick::~Brick()
{
}

void Brick::makeBullet()
{
	float speed = 4.0;
	for (int i = 0; i < 16; i++)
	{	
		bullets.emplace_back(std::make_shared<BulletData>(getPosition(), ofVec2f(speed*cosf(2.0*PI*i / 16), speed*sinf(2.0 * PI*i / 16)), 10, 0));
	}
}

void Brick::draw()
{
	float width = getWidth();
	float height = getHeight();

	ofPushMatrix();

	ofTranslate(getPosition());
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
	setPosition(getPosition() + private_data->vec);

	if (this->getPosition().y > ofGetHeight() + 50 || this->getPosition().y < -50 || private_data->is_hit) {
		makeBullet();
		private_data->can_remove = true;
	}
}

bool Brick::canRemove()
{
	return private_data->can_remove;
}

MyShip::MyShip() :
	counter(0), rotate_deg(0), life(3), hit_anime_counter(0)
{

	private_data = make_unique<GameObjectData>();
	private_data->object_type = GameObjectData::myship;
	private_data->is_hit = false;
	private_data->can_remove = false;
	private_data->pos = ofVec2f(ofGetWidth() / 2, ofGetHeight() - 100);
	private_data->vec = ofVec2f(5, 5);

	joy_.setup(GLFW_JOYSTICK_1);
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

	if (private_data->pos.x < ofGetWidth() / 4) {
		private_data->pos.x = ofGetWidth() / 4;
	}
	if (ofGetWidth() * 3 / 4 < private_data->pos.x) {
		private_data->pos.x = ofGetWidth() * 3 / 4;
	}
	if (private_data->pos.y < 0) {
		private_data->pos.y = 0;
	}
	if (ofGetHeight() < private_data->pos.y) {
		private_data->pos.y = ofGetHeight();
	}

	if (private_data->is_hit) {
		private_data->is_hit = false;
		if (life == 0)
		{
			private_data->can_remove = true;
			return;
		}
		if (hit_anime_counter > 0) {
			return;
		}
		life--;
		hit_anime_counter = 60;
	}
}

void MyShip::draw()
{
	if (private_data->is_hit) ofDrawBitmapString("IsHit", ofGetWidth() / 2, ofGetHeight() - 150);
	if (life == 3) ofDrawBitmapString("LIFE:3", ofGetWidth() / 2, ofGetHeight() - 100);
	if (life == 2) ofDrawBitmapString("LIFE:2", ofGetWidth() / 2, ofGetHeight() - 100);
	if (life == 1) ofDrawBitmapString("LIFE:1", ofGetWidth() / 2, ofGetHeight() - 100);

	ofNoFill();

	if (hit_anime_counter > 0) {
		if (life == 2)
		{
			ofSetColor(30, 30, 30, ofMap(hit_anime_counter, 60, 0, 255, 0));

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
			ofSetColor(30, 30, 30, ofMap(hit_anime_counter, 60, 0, 255, 0));

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

	/*
	if (life > 2) {
		if (hit_anime_counter > 0) {
			ofSetColor(30, 30, 30, ofMap(hit_anime_counter, 60, 0, 255, 0));
		}
		ofPushMatrix();
		ofTranslate(getPosition());
		ofRotateDeg(rotate_deg);
		ofDrawRectangle(-21, -21, 42, 42);
		ofDrawCircle(0, 0, 21 * 1.4);
		ofPopMatrix();
	}

	if (life > 1) {
		if ((life == 2) && (hit_anime_counter > 0)) {
			ofSetColor(30, 30, 30, ofMap(hit_anime_counter, 60, 0, 255, 0));
		}
		else
		{
			ofSetColor(30, 30, 30, 255);
		}
		ofPushMatrix();
		ofTranslate(getPosition());
		ofRotateDeg(-rotate_deg);	
		ofDrawRectangle(-15, -15, 30, 30);
		
		ofPopMatrix();
	}
	*/
	
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