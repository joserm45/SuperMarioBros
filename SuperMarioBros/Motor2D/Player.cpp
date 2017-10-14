#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "Player.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"

j1Player::j1Player() : j1Module()
{
	name.create("player");

	
}

// Destructor
j1Player::~j1Player()
{}

// Called before render is available
bool j1Player::Awake(pugi::xml_node& node)
{
	LOG("Loading Player");
	bool ret = true;

	position.create(96, 64);

	return ret;
}

// Called before the first frame
bool j1Player::Start()
{
	text_player = App->tex->Load("textures/mario_sprite.png");

	return true;
}

// Called each loop iteration
bool j1Player::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Player::Update(float dt)
{
	Draw();
	return true;
}

// Called each loop iteration
bool j1Player::PostUpdate()
{
	bool ret = true;

	

	return ret;
}

bool j1Player::Save(pugi::xml_node&)const
{

	return true;
}

bool j1Player::Load(pugi::xml_node &)
{

	return true;
}
void j1Player::Draw()
{

	App->render->Blit(text_player, position.x, position.y, sprite_pos);
}

// Called before quitting
bool j1Player::CleanUp()
{
	LOG("Freeing player");

	return true;
}
