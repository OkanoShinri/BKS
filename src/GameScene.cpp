#include "GameScene.h"

GameScene::GameScene(std::unique_ptr<SettingParameter>&& _setting_parameter)
{
	setting_parameter = std::move(_setting_parameter);

	this->box2d_for_breakout = std::make_shared<ofxBox2d>();
	this->box2d_for_breakout->init();
	this->box2d_for_breakout->enableEvents();
	this->box2d_for_breakout->setGravity(0, 0);
	this->box2d_for_breakout->createBounds(ofGetWidth() / 4, 0, ofGetWidth() / 2, ofGetHeight());
	this->box2d_for_breakout->setFPS(30);

	ofAddListener(box2d_for_breakout->contactStartEvents, this, &GameScene::b_contactStart);

	this->can_change_scene = false;
	this->is_transiting = false;
	this->transition_counter = 0;
	this->transition_time = 180;

	this->myShip = std::make_shared<MyShip>();
	myShip->setSEVolume(setting_parameter->se_volume);

	/*paddle
	this->myPaddle = std::make_unique<ofxBox2dRect>();
	this->myPaddle->setPhysics(1.0, 1.0, 0.0);
	this->myPaddle->setup(this->box2d_for_breakout->getWorld(), myShip->getPosition().x - 100, myShip->getPosition().y - 25, 200, 50);
	this->myPaddle->setFixedRotation(true);
	*/

	this->balls.clear();

	this->balls.emplace_back(std::make_unique<Ball>(this->box2d_for_breakout->getWorld(), ofGetWidth() / 2, 100, 10, 10, 10));
	if (ofGetWidth() > 1000)
	{
		this->balls.emplace_back(std::make_unique<Ball>(this->box2d_for_breakout->getWorld(), ofGetWidth() / 2, 100, 10, -10, 10));
	}

	this->bricks.clear();

	for (int i = 0; i < MAX_BULLETS_NUM; i++) {
		this->non_active_bullets.emplace_front(std::make_shared<Bullet>());
	}
	this->active_bullets.clear();

	verdana = std::make_unique<ofTrueTypeFont>();
	verdana->load("verdana.ttf", 30);
	back_ground = std::make_unique<BackGroundImage>();

	wall_hit_se = std::make_unique<ofSoundPlayer>();
	wall_hit_se->load("se01.mp3");
	wall_hit_se->setVolume(setting_parameter->se_volume);
	wall_hit_se->setMultiPlay(true);

	brick_hit_se = std::make_unique<ofSoundPlayer>();
	brick_hit_se->load("se02.mp3");
	brick_hit_se->setVolume(setting_parameter->se_volume);
	brick_hit_se->setMultiPlay(true);

	shot_se = std::make_unique<ofSoundPlayer>();
	shot_se->load("shotse01.mp3");
	shot_se->setVolume(setting_parameter->se_volume);
	shot_se->setMultiPlay(true);

	game_bgm = std::make_unique<ofSoundPlayer>();
	std::string bgms[] = { "Ž‡‰‘_-’Ç‰¯-_2.mp3","ƒ€ƒXƒJƒŠ‚Ì‰Ô.mp3" };
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
		init_y = ofGetHeight() + 50;
	}
	else {
		init_y = -50;
	}

	switch (id)
	{
	case 0:
		this->bricks.emplace_front(std::make_unique<Jikinerai_Single1>(this->box2d_for_breakout->getWorld(), ofRandom(setting_parameter->window_width / 4, setting_parameter->window_width * 3 / 4), -50, 0.5, myShip));
		break;
	case 1:
		this->bricks.emplace_front(std::make_unique<NWay_Around_Single1>(this->box2d_for_breakout->getWorld(), ofRandom(setting_parameter->window_width / 4, setting_parameter->window_width * 3 / 4), -50, 0.5));
		break;
	case 2:
		this->bricks.emplace_front(std::make_unique<Jikinerai_NWay_Single1>(this->box2d_for_breakout->getWorld(), ofRandom(setting_parameter->window_width / 4, setting_parameter->window_width * 3 / 4), -50, 0.5, myShip));
		break;
	case 3:
		this->bricks.emplace_front(std::make_unique<Jikinerai_NWay_Single2>(this->box2d_for_breakout->getWorld(), ofRandom(setting_parameter->window_width / 4, setting_parameter->window_width * 3 / 4), -50, 0.5, myShip));
		break;
	case 4:
		this->bricks.emplace_front(std::make_unique<Jikinerai_Multiple1>(this->box2d_for_breakout->getWorld(), ofRandom(setting_parameter->window_width / 4, setting_parameter->window_width * 3 / 4), -50, 0.5, myShip));
		break;
	case 5:
		this->bricks.emplace_front(std::make_unique<NWay_Around_Multiple1>(this->box2d_for_breakout->getWorld(), ofRandom(setting_parameter->window_width / 4, setting_parameter->window_width * 3 / 4), -50, 0.5));
		break;
	case 6:
		this->bricks.emplace_front(std::make_unique<NWay_Around_Big1>(this->box2d_for_breakout->getWorld(), ofRandom(setting_parameter->window_width / 4, setting_parameter->window_width * 3 / 4), -50, 0.5));
		break;
	case 7:
		this->bricks.emplace_front(std::make_unique<FourWay_Guruguru1>(this->box2d_for_breakout->getWorld(), ofRandom(setting_parameter->window_width / 4, setting_parameter->window_width * 3 / 4), -50, 0.5));
		break;
	case 8:
		this->bricks.emplace_front(std::make_unique<NWay_Around_Kasoku1>(this->box2d_for_breakout->getWorld(), ofRandom(setting_parameter->window_width / 4, setting_parameter->window_width * 3 / 4), -50, 0.5));
		break;
	case 9:
		this->bricks.emplace_front(std::make_unique<Hibachi1>(this->box2d_for_breakout->getWorld(), ofRandom(setting_parameter->window_width / 4, setting_parameter->window_width * 3 / 4), -50, 0.5));
		break;
	default:
		this->bricks.emplace_front(std::make_unique<Jikinerai_Single1>(this->box2d_for_breakout->getWorld(), ofRandom(setting_parameter->window_width / 4, setting_parameter->window_width * 3 / 4), -50, 0.5, myShip));
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
	/*
	if (counter > 1200 && ((counter % 360) == 0)) {
		this->bricks.emplace_back(std::move(brickFactory((int)ofRandom(3), true)));
	}*/

	//-------box2d update-------------
	this->box2d_for_breakout->update();

	//-------myship update------------
	this->myShip->update();
	if (myShip->canRemove()) {
		is_transiting = true;
		//transition_counter = 0;
	}

	//-------back ground------------
	back_ground->updata();

	this->counter++;
}

void GameScene::draw()
{
	//-------back ground------------
	back_ground->draw();

	//-------bricks update&draw--------
	ofSetColor(255, 100, 100);
	for (auto it = this->bricks.begin(); it != this->bricks.end();)
	{
		(*it)->update();
		if ((*it)->canRemove()) {
			if ((*it)->isHit()) {
				// destroy brick(s) and make bullets
				for (auto & bullet_data : (*it)->makeBullet()) {
					if (non_active_bullets.empty()) {
						break;
					}
					active_bullets.push_back(non_active_bullets.back());
					non_active_bullets.pop_back();
					active_bullets.back()->set(bullet_data);
				}
			}
			it = this->bricks.erase(it);
		}
		else {
			(*it)->draw();
			(*it)->setBulletSpeedRate(1 + counter * 0.0001);
			++it;
		}
	}

	//-------ball update&draw---------
	ofSetColor(255, 214, 98);
	for (auto it = this->balls.begin(); it != this->balls.end();)
	{
		(*it)->update();
		if ((*it)->canRemove())
		{
			it = this->balls.erase(it);
		}
		else {
			(*it)->draw();
			++it;
		}
	}

	//-------paddle update&draw-------
	//this->myPaddle->setPosition(this->myShip->getPosition());
	//this->myPaddle->draw();

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
	//ofSetColor(0, 122, 204);
	ofSetColor(0, 0, 0);
	ofDrawRectangle(0, 0, ofGetWidth() / 4, ofGetHeight());
	ofDrawRectangle(ofGetWidth() * 3 / 4, 0, ofGetWidth() / 4, ofGetHeight());

	(this->is_transiting) ? ofSetColor(255, 10, 10) : ofSetColor(255, 255, 255);
	char minute[3];
	sprintf_s(minute, "%02d", finish_time / 3600);
	char second[3];
	sprintf_s(second, "%02d", (finish_time / 60) % 60);
	char m_second[3];
	sprintf_s(m_second, "%02d", (int)ofMap(finish_time % 60, 0, 60, 0, 99));
	verdana->drawString("" + std::string(minute) + ":" + std::string(second) + ":" + std::string(m_second), 50, ofGetHeight() / 4);

	ofSetColor(255, 255, 255);
	if (game_bgm->isPlaying())
	{
		std::ostringstream bgm_param;
		bgm_param << setting_parameter->bgm_volume;
		ofDrawBitmapString("bgm: " + bgm_param.str(), 100, ofGetHeight() / 2);
	}

	std::ostringstream se_param;
	se_param << setting_parameter->se_volume;
	ofDrawBitmapString("se:  " + se_param.str(), 100, ofGetHeight() / 2 + 50);

	//drawHowToPlay(10, 10);

	//-------transition_in------------
	if (counter < 30)
	{
		ofSetColor(255, 255, 255, ofMap(counter, 0, 30, 255, 0));
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	}

	//-------transition_out-----------
	if (this->is_transiting)
	{
		ofSetColor(255, 255, 255, ofMap(transition_counter, 0, transition_time, 0, 255));
		ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
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
	if (key == 'p') {
		//activate_paddle = !activate_paddle;
	}
}

void GameScene::mouseMoved(int x, int y)
{
	//this->myShip->mouseMoved(x, y);
}

void GameScene::b_contactStart(ofxBox2dContactArgs & e)
{
	if (e.a == NULL || e.b == NULL) {
		return;
	}

	GameObjectData* aData = (GameObjectData*)e.a->GetBody()->GetUserData();
	GameObjectData* bData = (GameObjectData*)e.b->GetBody()->GetUserData();

	if (aData == NULL || bData == NULL)
	{
		wall_hit_se->play();
		return;
	}

	if (aData->object_type == GameObjectData::ball && bData->object_type == GameObjectData::brick)
	{
		aData->is_hit = true;
		bData->is_hit = true;
		brick_hit_se->play();
	}
	else if (aData->object_type == GameObjectData::brick && bData->object_type == GameObjectData::ball)
	{
		aData->is_hit = true;
		bData->is_hit = true;
		brick_hit_se->play();
	}
}