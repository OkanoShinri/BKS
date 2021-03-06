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
	void draw();
	void drawHowToPlay(float _x, float _y);
	void feadout(int mun) {};
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void update();

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
	std::unique_ptr<SettingParameter> setting_parameter;
	std::unique_ptr<BackGroundImage> back_ground;
	std::shared_ptr<MyShip> myShip;
	std::unique_ptr<ofSoundPlayer> game_bgm, shot_se;
	SceneIdx next_scene = title_scene;
	std::unique_ptr<ofTrueTypeFont> verdana;
};
