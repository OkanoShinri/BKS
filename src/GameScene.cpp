#include "GameScene.h"

GameScene::GameScene(std::unique_ptr<SettingParameter>&& _setting_parameter)
{
	setting_parameter = std::move(_setting_parameter);

	this->can_change_scene = false;
	this->is_transiting = false;
	this->transition_counter = 0;
	this->transition_time = 180;

	this->myShip = std::make_shared<MyShip>();
	myShip->setSEVolume(setting_parameter->se_volume);

	this->balls.clear();

	int speed = 10;
	for (int i = 0; i < setting_parameter->num_ball; i++)
	{
		this->balls.emplace_back(std::make_unique<Ball>(
			ofVec2f(setting_parameter->window_width / 2, 100),
			ofVec2f(cos(45 * DEG_TO_RAD + 2 * PI*i / setting_parameter->num_ball), sin(45 * DEG_TO_RAD + 2 * PI*i / setting_parameter->num_ball))*speed,
			setting_parameter->window_width,
			setting_parameter->window_height,
			setting_parameter->se_volume));
	}

	this->bricks.clear();

	for (int i = 0; i < MAX_BULLETS_NUM; i++) {
		this->non_active_bullets.emplace_front(std::make_shared<Bullet>());
	}
	this->active_bullets.clear();

	verdana = std::make_unique<ofTrueTypeFont>();
	verdana->load("verdana.ttf", 30);
	back_ground = std::make_unique<BackGroundImage>();

	shot_se = std::make_unique<ofSoundPlayer>();
	shot_se->load("shotse01.mp3");
	shot_se->setVolume(setting_parameter->se_volume);
	shot_se->setMultiPlay(true);

	game_bgm = std::make_unique<ofSoundPlayer>();
	std::string bgms[] = { "????_-?ǉ?-_2.mp3","???X?J???̉?.mp3" };
	int idx = std::rand() % 2;
	game_bgm->load(bgms[idx]);
	game_bgm->setLoop(true);
	game_bgm->setVolume(setting_parameter->bgm_volume);
	game_bgm->play();
}

GameScene::~GameScene()
{
	std::cout << "Remove: GameScene" << std::endl;
}

void GameScene::addBrick(int id, float _v_y = 0.5)
{
	float init_y;
	if (_v_y < 0) {
		init_y = setting_parameter->window_height + 50;
	}
	else {
		init_y = -50;
	}

	switch (id)
	{
	case 0:
		this->bricks.emplace_front(std::make_unique<Jikinerai_Single1>(ofRandom(setting_parameter->window_width / 4, setting_parameter->window_width * 3 / 4), -50, 0.5, myShip));
		break;
	case 1:
		this->bricks.emplace_front(std::make_unique<NWay_Around_Single1>(ofRandom(setting_parameter->window_width / 4, setting_parameter->window_width * 3 / 4), -50, 0.5, myShip));
		break;
	case 2:
		this->bricks.emplace_front(std::make_unique<Jikinerai_NWay_Single1>(ofRandom(setting_parameter->window_width / 4, setting_parameter->window_width * 3 / 4), -50, 0.5, myShip));
		break;
	case 3:
		this->bricks.emplace_front(std::make_unique<Jikinerai_NWay_Single2>(ofRandom(setting_parameter->window_width / 4, setting_parameter->window_width * 3 / 4), -50, 0.5, myShip));
		break;
	case 4:
		this->bricks.emplace_front(std::make_unique<Jikinerai_Multiple1>(ofRandom(setting_parameter->window_width / 4, setting_parameter->window_width * 3 / 4), -50, 0.5, myShip));
		break;
	case 5:
		this->bricks.emplace_front(std::make_unique<NWay_Around_Multiple1>(ofRandom(setting_parameter->window_width / 4, setting_parameter->window_width * 3 / 4), -50, 0.5, myShip));
		break;
	case 6:
		this->bricks.emplace_front(std::make_unique<NWay_Around_Big1>(ofRandom(setting_parameter->window_width / 4, setting_parameter->window_width * 3 / 4), -50, 0.5, myShip));
		break;
	case 7:
		this->bricks.emplace_front(std::make_unique<FourWay_Guruguru1>(ofRandom(setting_parameter->window_width / 4, setting_parameter->window_width * 3 / 4), -50, 0.5, myShip));
		break;
	case 8:
		this->bricks.emplace_front(std::make_unique<NWay_Around_Kasoku1>(ofRandom(setting_parameter->window_width / 4, setting_parameter->window_width * 3 / 4), -50, 0.5, myShip));
		break;
	case 9:
		this->bricks.emplace_front(std::make_unique<Hibachi1>(ofRandom(setting_parameter->window_width / 4, setting_parameter->window_width * 3 / 4), -50, 0.5, myShip));
		break;
	default:
		this->bricks.emplace_front(std::make_unique<Jikinerai_Single1>(ofRandom(setting_parameter->window_width / 4, setting_parameter->window_width * 3 / 4), -50, 0.5, myShip));
		break;
	}
}

void GameScene::update()
{
	if (is_transiting) {
		game_bgm->setVolume(ofMap(transition_counter, 0.0, transition_time, setting_parameter->bgm_volume, 0.0));

		if (transition_counter < transition_time) {
			transition_counter++;
		}
		else {
			can_change_scene = true;
		}
		return;
	}

	//-------add brick----------------
	if (!(counter % 30))
	{
		addBrick(0);
	}
	if (counter < 1800)
	{
		if (counter % 120 == 0)
		{
			addBrick(std::rand() % 3);
		}
	}
	else if (counter < 3600)
	{
		if (counter % 90 == 0)
		{
			addBrick(std::rand() % 4 + 2);
		}
	}
	else if (counter < 7200)
	{
		if (counter % 90 == 0)
		{
			addBrick(std::rand() % 3 + 5);
		}
	}
	else
	{
		if (counter % 180 == 0)
		{
			addBrick(std::rand() % 3 + 7);
		}
	}

	//-------myship update------------
	this->myShip->update();
	if (myShip->canRemove()) {
		is_transiting = true;
	}

	//-------ball updata---------
	ofSetColor(255, 214, 98);
	for (auto it = this->balls.begin(); it != this->balls.end();)
	{
		(*it)->update();
		if ((*it)->canRemove())
		{
			it = this->balls.erase(it);
		}
		else {
			++it;
		}
	}

	//-------back ground------------
	back_ground->updata();

	this->counter++;
}

void GameScene::draw()
{
	ofTranslate(setting_parameter->offset_x, setting_parameter->offset_y);
	ofScale(setting_parameter->scale, setting_parameter->scale);

	ofSetColor(255, 255, 255);
	//-------back ground------------
	back_ground->draw();

	//-------bricks update&draw--------
	ofSetColor(255, 100, 100);
	for (auto it_brick = this->bricks.begin(); it_brick != this->bricks.end();)
	{
		if (!is_transiting) {
			(*it_brick)->update();
		}

		for (auto it_ball = this->balls.begin(); it_ball != this->balls.end(); ++it_ball)
		{
			if ((*it_ball)->isHit((*it_brick)->getPosition(), (*it_brick)->getShape()))
			{
				// destroy brick(s) and make bullets
				for (auto & bullet_data : (*it_brick)->makeBullet()) {
					if (non_active_bullets.empty()) {
						break;
					}
					active_bullets.push_back(non_active_bullets.back());
					non_active_bullets.pop_back();
					active_bullets.back()->set(bullet_data);
				}
				(*it_brick)->setRemoveable();
			}
		}
		if ((*it_brick)->canRemove()) {
			it_brick = this->bricks.erase(it_brick);
		}
		else {
			(*it_brick)->draw();
			(*it_brick)->setBulletSpeedRate(1 + counter * 0.0001);
			++it_brick;
		}
	}

	//-------ball draw---------
	ofSetColor(255, 214, 98);
	for (auto it = this->balls.begin(); it != this->balls.end();)
	{
		{
			(*it)->draw();
			++it;
		}
	}
	
	//-------bullets update&draw------
	ofSetColor(10, 10, 10);
	for (auto it = this->active_bullets.begin(); it != this->active_bullets.end();)
	{
		(*it)->update();

		if (false && (*it)->play_shotse && !shot_se->isPlaying())
		{
			shot_se->play();
		}

		float x1 = (*it)->getPosition().x;
		float x2 = myShip->getPosition().x;
		float y1 = (*it)->getPosition().y;
		float y2 = myShip->getPosition().y;
		float r = (*it)->getRadius();
		if ((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) < r*r) {
			myShip->hit();
		}

		if ((*it)->canRemove())
		{
			non_active_bullets.push_back((*it));
			it = this->active_bullets.erase(it);
		}
		else {
			(*it)->draw();
			++it;
		}
	}

	//-------myShip draw--------------
	ofSetColor(30, 30, 30);
	this->myShip->draw();

	//-------side infos---------------
	ofSetColor(0, 0, 0);
	ofDrawRectangle(0, 0, setting_parameter->window_width / 4, setting_parameter->window_height);
	ofDrawRectangle(setting_parameter->window_width * 3 / 4, 0, setting_parameter->window_width / 4, setting_parameter->window_height);
	ofSetColor(255, 255, 255);
	ofDrawRectangle(0, 0, setting_parameter->window_width / 4 - 5, setting_parameter->window_height);
	ofDrawRectangle(setting_parameter->window_width * 3 / 4 + 5, 0, setting_parameter->window_width / 4, setting_parameter->window_height);

	(this->is_transiting) ? ofSetColor(255, 10, 10) : ofSetColor(0, 0, 0);
	char minute[3];
	sprintf_s(minute, "%02d", finish_time / 3600);
	char second[3];
	sprintf_s(second, "%02d", (finish_time / 60) % 60);
	char m_second[3];
	sprintf_s(m_second, "%02d", (int)ofMap(finish_time % 60, 0, 60, 0, 99));
	verdana->drawString("" + std::string(minute) + ":" + std::string(second) + "." + std::string(m_second), 50, setting_parameter->window_height / 4);

	ofSetColor(0, 0, 0);
	if (game_bgm->isPlaying())
	{
		std::ostringstream bgm_param;
		bgm_param << setting_parameter->bgm_volume;
		ofDrawBitmapString("bgm: " + bgm_param.str(), 100, setting_parameter->window_height / 2);
	}

	std::ostringstream se_param;
	se_param << setting_parameter->se_volume;
	ofDrawBitmapString("se:  " + se_param.str(), 100, setting_parameter->window_height / 2 + 50);

	ofDrawBitmapString(ofToString(ofGetFrameRate()) + "fps", 20, setting_parameter->window_height - 50);

	//-------transition_in------------
	if (counter < 30)
	{
		ofSetColor(255, 255, 255, ofMap(counter, 0, 30, 255, 0));
		ofDrawRectangle(0, 0, setting_parameter->window_width, setting_parameter->window_height);
	}

	//-------transition_out-----------
	if (this->is_transiting)
	{
		ofSetColor(255, 255, 255, ofMap(transition_counter, 0, transition_time, 0, 255));
		ofDrawRectangle(0, 0, setting_parameter->window_width, setting_parameter->window_height);
	}
	else
	{
		finish_time = counter;
	}
}

void GameScene::drawHowToPlay(float _x, float _y)
{
	ofSetColor(255, 255, 255);
	ofDrawRectangle(_x + 55, _y, 50, 50);
	ofDrawRectangle(_x, _y + 55, 50, 50);
	ofDrawRectangle(_x + 55, _y + 55, 50, 50);
	ofDrawRectangle(_x + 110, _y + 55, 50, 50);

	ofSetColor(77, 96, 130);
	ofDrawTriangle(_x + 55 + 25, _y + 15, _x + 55 + 35, _y + 35, _x + 55 + 15, _y + 35);
	ofDrawTriangle(_x + 55 + 25, _y + 95, _x + 55 + 35, _y + 75, _x + 55 + 15, _y + 75);
	ofDrawTriangle(_x + 15, _y + 80, _x + 35, _y + 70, _x + 35, _y + 90);
	ofDrawTriangle(_x + 15 + 130, _y + 80, _x + 35 + 90, _y + 70, _x + 35 + 90, _y + 90);
}

void GameScene::keyPressed(int key) {
	this->myShip->keyPressed(key);

	switch (key) {
	case 'q':

		this->can_change_scene = true;
		break;
	}
}

void GameScene::keyReleased(int key)
{
	this->myShip->keyReleased(key);
}

void GameScene::mouseMoved(int x, int y)
{
}
