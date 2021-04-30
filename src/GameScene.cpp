#include "GameScene.h"

GameScene::GameScene()
{
	this->box2d_for_breakout = std::make_shared<ofxBox2d>();
	this->box2d_for_breakout->init();
	this->box2d_for_breakout->enableEvents();
	this->box2d_for_breakout->setGravity(0, 0);
	this->box2d_for_breakout->createBounds(ofGetWidth() / 4, 0, ofGetWidth() / 2, ofGetHeight());
	this->box2d_for_breakout->setFPS(30);

	ofAddListener(box2d_for_breakout->contactStartEvents, this, &GameScene::b_contactStart);

	this->can_change_scene = false;
	this->is_transiting = false;
	this->nextScene = Scene::title_scene;
	this->transition_counter = 0;
	this->transition_time = 180;

	this->myShip = std::make_unique<MyShip>();

	if (activate_paddle) {
		this->myPaddle = std::make_unique<ofxBox2dRect>();
		this->myPaddle->setPhysics(1.0, 1.0, 0.0);
		this->myPaddle->setup(this->box2d_for_breakout->getWorld(), myShip->getPosition().x - 100, myShip->getPosition().y - 25, 200, 50);
		this->myPaddle->setFixedRotation(true);
	}

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

	this->wall_hit_se = std::make_unique<ofSoundPlayer>();
	wall_hit_se->load("se01.mp3");
	this->wall_hit_se->setMultiPlay(true);

	this->brick_hit_se = std::make_unique<ofSoundPlayer>();
	brick_hit_se->load("se02.mp3");
	this->brick_hit_se->setMultiPlay(true);

	this->shot_se = std::make_unique<ofSoundPlayer>();
	shot_se->load("shotse01.mp3");
	this->shot_se->setMultiPlay(true);

	this->game_bgm = std::make_unique<ofSoundPlayer>();
	std::string bgms[] = { "Ž‡‰‘_-’Ç‰¯-_2.mp3","ƒ€ƒXƒJƒŠ‚Ì‰Ô.mp3" };
	int idx = std::rand() % 2;
	game_bgm->load(bgms[idx]);
	game_bgm->setLoop(true);
	game_bgm->play();
}

GameScene::~GameScene()
{
	std::cout << "Remove: GameScene" << std::endl;
}

std::unique_ptr<Brick> GameScene::brickFactory(int _id, float _v_y = 0.5)
{
	float init_y;
	if (_v_y<0) {
		init_y = ofGetHeight() + 50;
	}
	else {
		init_y = -50;
	}

	switch (_id)
	{
	case 0:
	{
		return std::make_unique<Brick>(this->box2d_for_breakout->getWorld(), ofRandom(ofGetWidth() / 3, ofGetWidth() * 2 / 3), init_y, 30, 30, _v_y);
	}
		break;
	
	case 1:
	{
		std::unique_ptr<NWayBrick_1> b = std::make_unique<NWayBrick_1>(this->box2d_for_breakout->getWorld(), ofRandom(ofGetWidth() / 3, ofGetWidth() * 2 / 3), init_y, 30, 30, _v_y);
		b->setNWay(16);
		return b;
	}
		break;
	
	case 2:
	{
		std::unique_ptr<NWayBrick_2> b = std::make_unique<NWayBrick_2>(this->box2d_for_breakout->getWorld(), ofRandom(ofGetWidth() / 3, ofGetWidth() * 2 / 3), init_y, 30, 30, _v_y);
		b->setNWay(12);
		return b;
	}
		break;

	case 3:
	{
		std::unique_ptr<NWayBrick_2> b = std::make_unique<NWayBrick_2>(this->box2d_for_breakout->getWorld(), ofRandom(ofGetWidth() / 3, ofGetWidth() * 2 / 3), init_y, 30, 30, _v_y);
		b->setNWay(12);
		return b;
	}
		break;
	
	case 4:
	{
		return std::make_unique<Jikinerai_Brick>(this->box2d_for_breakout->getWorld(), ofRandom(ofGetWidth() / 3, ofGetWidth() * 2 / 3), init_y, 30, 30, _v_y);
	}
		break;
	default:
		return std::make_unique<Brick>(this->box2d_for_breakout->getWorld(), ofRandom(ofGetWidth() / 3, ofGetWidth() * 2 / 3), init_y, 200, 50, _v_y);
		break;
	}
}

void GameScene::update()
{
	if (is_transiting) {
		game_bgm->setVolume(ofMap(transition_counter, 0, transition_time, 1.0, 0.0));

		if (transition_counter < transition_time) {
			transition_counter++;
		}
		else {
			can_change_scene = true;
		}
		return;
	}

	//-------add brick----------------

	if (counter % 600 == 0)
	{
		int x = (int)ofRandom(200);
		for (int i = 0; i < 10; i++)
		{
			this->bricks.emplace_front(std::make_unique<Jikinerai_Brick>(this->box2d_for_breakout->getWorld(), ofGetWidth() / 3 + x + 30 * i, -50, 30, 30, 0.5));
		}
	}
	else if (counter % 120 == 0)
	{
		this->bricks.emplace_front(std::move(brickFactory((int)ofRandom(5))));
		//this->bricks.emplace_front(std::move(brickFactory(4)));
	}
	/*
	if (counter > 1200 && ((counter % 360) == 0)) {
		this->bricks.emplace_back(std::move(brickFactory((int)ofRandom(3), true)));
	}*/

	//-------box2d update-------------
	this->box2d_for_breakout->update();

	//-------myship update------------
	this->myShip->update();
	if (this->myShip->canRemove()) {
		this->can_change_scene = true;
	}

	this->counter++;
}

void GameScene::draw()
{
	//-------back ground---------------
	ofSetColor(255, 255, 255);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	//-------bricks update&draw--------
	ofSetColor(255, 100, 100);
	for (auto it = this->bricks.begin(); it != this->bricks.end();)
	{
		(*it)->update();
		if ((*it)->canRemove()) {
			// destroy brick(s) and make bullets
			for (auto &_bullet_data : (*it)->getBullet()) {
				active_bullets.push_back(non_active_bullets.back());
				non_active_bullets.pop_back();
				if (_bullet_data->vec.x == NULL) {
					_bullet_data->vec = ofVec2f(6.0*(myShip->getPosition() - _bullet_data->pos).normalize());
					_bullet_data->is_homing = true;
				}
				active_bullets.back()->set(_bullet_data);
			}
			it = this->bricks.erase(it);
		}
		else {
			(*it)->draw();
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
	if (activate_paddle) {
		this->myPaddle->setPosition(this->myShip->getPosition());
	}

	if (activate_paddle) {
		this->myPaddle->draw();
	}

	//-------bullets update&draw------
	bool is_play_shotse = false;
	ofSetColor(10, 10, 10);
	for (auto it = this->active_bullets.begin(); it != this->active_bullets.end();)
	{
		(*it)->update();

		if ((*it)->play_shotse && !is_play_shotse)
		{
			shot_se->play();
			is_play_shotse = true;
		}

		float x1 = (*it)->getPosition().x;
		float x2 = myShip->getPosition().x;
		float y1 = (*it)->getPosition().y;
		float y2 = myShip->getPosition().y;
		float r = (*it)->getRadius();
		if ((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) < r*r) {
			is_transiting = true;
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

	ofSetColor(255, 255, 255);
	drawHowToPlay(10, 10);

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
		activate_paddle = !activate_paddle;
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