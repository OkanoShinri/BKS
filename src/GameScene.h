#pragma once

#include "Scene.h"
#include "GameObject.h"
#include "BackGroundImage.h"

constexpr auto MAX_BULLETS_NUM = 3000;

class GameScene :
	public Scene
{
public:
	GameScene(std::unique_ptr<SettingParameter>&& _setting_parameter);
	~GameScene();
	std::unique_ptr<SettingParameter> getSettingParameter() {
		return std::move(setting_parameter);
	}
	SceneIdx getNextScene() {
		return next_scene;
	}
	bool canChangeScene() {
		return can_change_scene;
	}
private:
	void addBrick(int _id, float _v_y);
	void b_contactStart(ofxBox2dContactArgs &e);
	void draw();
	void drawHowToPlay(float _x, float _y);
	void feadout(int mun) {};
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void update();

	//bool activate_paddle = false;
	int counter = 0;
	int finish_time = 0;
	int transition_counter = 0;
	int transition_time = 0;
	bool can_change_scene = false;
	bool is_transiting = false;
	std::list< std::shared_ptr<Bullet> > active_bullets;
	std::list< std::shared_ptr<Bullet> > non_active_bullets;
	std::list< std::unique_ptr<Ball> > balls;
	std::list< std::unique_ptr<Brick> > bricks;
	std::shared_ptr<ofxBox2d> box2d_for_breakout;
	std::shared_ptr<ofxBox2d> box2d_for_shooting;
	std::unique_ptr<SettingParameter> setting_parameter;
	std::unique_ptr<BackGroundImage> back_ground;
	std::shared_ptr<MyShip> myShip;
	std::unique_ptr<ofSoundPlayer> game_bgm, wall_hit_se, brick_hit_se, shot_se;
	std::unique_ptr<ofxBox2dRect> myPaddle;
	SceneIdx next_scene = title_scene;
	std::unique_ptr<ofTrueTypeFont> verdana;
};
