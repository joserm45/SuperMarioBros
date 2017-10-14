#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Module.h"
#include "p2List.h"

struct SDL_Texture;


class j1Player : public j1Module
{
public:
	enum mario {IDLE, RIGHT,LEFT,JUMP};

	j1Player();

	// Destructor
	virtual ~j1Player();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	//Save and Load functions
	bool Load(pugi::xml_node &);
	bool Save(pugi::xml_node&)const;
	void Draw();
	// Called before quitting
	bool CleanUp();

private:
	SDL_Texture* text_player = nullptr;
	fPoint position;
	SDL_Rect* sprite_pos;
	p2List<j1Player> animation;

public:


};

#endif // __PLAYER_H__
