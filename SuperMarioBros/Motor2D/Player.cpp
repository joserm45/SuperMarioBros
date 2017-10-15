#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "Player.h"
#include "j1App.h"
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
	//little mario
	//sprite_idle = { 277,44,12,16 };

	sprite_idle.PushBack( { 258,1,16,32 });

	//Walk right
	move_right.PushBack({ 296,3,16,30 });
	move_right.PushBack({ 315,2,14,31 });
	move_right.PushBack({ 331,1,16,32 });
	move_right.speed = ANIMATION_SPEED;
	/*Litlle Mario
	move_right.PushBack({54,181,27,32});
	move_right.PushBack({ 93,181,27,32 });
	move_right.PushBack({ 11,220,27,32 });
	move_right.PushBack({ 54,218,27,32 });
	move_right.PushBack({ 91,219,27,32 });
	*/

	//Walk left
	//SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL;

	//SDL_RenderCopyEx(renderer, texture, &srcrect, &dstrect, angle, &center, flip);

	//Jump 
	jump.PushBack({369,2,16,32});

	//little mario
	//jump = { 126,220,34,32 };

	//Die 
	die.PushBack({ 277, 11, 16, 22 });

	//Litlle mario
	//die = { 127,263,32,28 };
}

// Destructor
j1Player::~j1Player()
{}

// Called before render is available
bool j1Player::Awake(pugi::xml_node& node)
{
	LOG("Loading Player");
	bool ret = true;

	//position.create(86, 174);

	return ret;
}

// Called before the first frame
bool j1Player::Start()
{
	text_player = App->tex->Load("textures/characters.png");

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
	vector_x = 0;
	vector_y = 0;
	
	bool moving = false;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		moving = true;
		vector_x -= VELOCITY;
		stage = LEFT;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		moving = true;
		vector_x += VELOCITY;
		stage = RIGHT;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		moving = true;
		jumping = true;
		stage = JUMP;
		frame_counter = 0;

		//max jump (if)
	}
	if (jumping == true)
	{
		vector_y -= VELOCITY * 1.5f;
		frame_counter++;
		if (frame_counter > 120)
		{
			jumping = false;
			frame_counter = 0;
		}
	}

	vector_y += GRAVITY;
	CheckCollisions();
	position.x += vector_x;
	position.y += vector_y;
	MoveCamera();

	if (position.y > 240)
	{
		App->scene->LoadLvl(App->scene->current_lvl, true);
	}
	
	if (moving == false)
	{
		stage = IDLE;
	}
	

	Draw();

	App->render->Blit(text_player, position.x, position.y, &(sprite->GetCurrentFrame()));
	return true;
}

// Called each loop iteration
bool j1Player::PostUpdate()
{
	bool ret = true;

	

	return ret;
}

void j1Player::CheckCollisions()
{
	iPoint current_tile = App->map->WorldToMap(position.x, position.y);
	if (vector_x > 0)
	{
		current_tile.x++;
	}
	if (vector_x < 0)
	{
		current_tile.x--;
	}
	Layer* collision_layer = App->map->GetCollisionLayer();
	uint tile_id = collision_layer->Get(current_tile.x, current_tile.y);

	if (App->map->map_data.tilesets[0]->GetTileCollision(tile_id))
	{
		vector_x = 0;
	}
	tile_id = collision_layer->Get(current_tile.x, current_tile.y+1);

   	if (App->map->map_data.tilesets[0]->GetTileCollision(tile_id))
	{
		vector_x = 0;
	}

	// Y axis
	current_tile = App->map->WorldToMap(position.x, position.y);
	if (vector_y > 0)
	{
		current_tile.y = current_tile.y + 2;
	}
	if (vector_y < 0)
	{
		current_tile.y --;
	}

	tile_id = collision_layer->Get(current_tile.x, current_tile.y );

	if (App->map->map_data.tilesets[0]->GetTileCollision(tile_id))
	{
		vector_y = 0;
	}


}
bool j1Player::Save(pugi::xml_node& node)const
{
	pugi::xml_node root = node.append_child("position");
	root.append_attribute("x") = position.x;
	root.append_attribute("y") = position.y;

	return true;
}

bool j1Player::Load(pugi::xml_node &node)
{
	pugi::xml_node root = node.child("position");
	position.x = root.attribute("x").as_int();
	position.y = root.attribute("y").as_int();
	return true;
}
void j1Player::Draw()
{
	switch (stage)
	{
		case j1Player::IDLE:
		{
			sprite = &sprite_idle;
			break;
		}
		case j1Player::RIGHT: 
		{
			sprite = &move_right;
			break;
		}
		
		case j1Player::LEFT:
		{
			sprite = &move_left;
			break;
		}
		case j1Player::JUMP:
		{
			sprite = &jump;
			break;
		}
		default:

			break;
	}
	
}

void j1Player::MoveCamera()
{
	int relative_pos_player = position.x + App->render->camera.x;

	if (relative_pos_player < App->render->border)
	{
		App->render->camera.x = - (position.x - App->render->border);
	}
	if (relative_pos_player > App->render->camera.w - App->render->border )
	{
		App->render->camera.x = -(position.x - (App->render->camera.w - App->render->border));
	}
	if (App->render->camera.x > 0)
	{
		App->render->camera.x = 0;
	}
	if (App->render->camera.x < -2816)
	{
		App->render->camera.x = -2816;
	}
}

// Called before quitting
bool j1Player::CleanUp()
{
	LOG("Freeing player");

	return true;
}
