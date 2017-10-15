#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "Player.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	
	LoadLvl(1,true);

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		App->GoSave();
		App->GoSaveAudio();
	}

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		App->GoLoad();
		App->GoLoadAudio();
	}

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		LoadLvl(1,true);
	}


	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		LoadLvl(current_lvl, true);
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		LoadLvl(2,true);
	}
/*
	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y -= -1;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y += -1;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x -= -1;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x += -1;*/
		
	if (App->player->position.x >= 3550)
	{
		if (current_lvl == 1)
			LoadLvl(2, true);

		else if (current_lvl == 2)
			LoadLvl(1, true);
	}

	App->map->Draw();

	// "Map:%dx%d Tiles:%dx%d Tilesets:%d"
	p2SString title("SuperMarioBros");
	p2SString title_tile("Map:%dx%d Tiles:%dx%d Tilesets:%d",
					App->map->map_data.width, App->map->map_data.height,
					App->map->map_data.tile_width, App->map->map_data.tile_height,
					App->map->map_data.tilesets.count());

	App->win->SetTitle(title.GetString());
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
void j1Scene::LoadLvl(int current,bool lvl_start)
{
	if (lvl_start == true)
	{
		App->player->position.x = 86;
		App->player->position.y = 174;
		App->render->camera.x = 0;
		App->render->camera.y = 0;
	}
	if (current == 1)
	{
		App->audio->PlayMusic("audio/music/lvl_1.ogg");
		App->map->Load("lvl1.tmx");
		current_lvl = current;
	}
	else if (current == 2)
	{
		App->audio->PlayMusic("audio/music/lvl_2.ogg");
		App->map->Load("lvl2.tmx");
		current_lvl = current;
	}
	
}

bool j1Scene::Save(pugi::xml_node& node)const
{
	node.append_attribute("current_lvl") = current_lvl;
	return true;
}

bool j1Scene::Load(pugi::xml_node& node)
{
	LoadLvl(node.attribute("current_lvl").as_int(),false);
	return true;
}