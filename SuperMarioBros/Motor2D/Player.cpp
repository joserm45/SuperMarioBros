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

	position.create(86, 174);

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
	

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= VELOCITY;
		App->render->camera.x -= -0.1f;
		stage = LEFT;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += VELOCITY;
		App->render->camera.x += -0.1f;
		stage = RIGHT;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		int gravedad = GRAVITY + dt;
		position.y -= VELOCITY + gravedad;
		stage = JUMP;

		//max jump (if)
	}

	/*if ((App->input->GetKey(SDL_SCANCODE_SPACE) == NULL))
	{
		stage = IDLE;
	}
	*/
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

// Called before quitting
bool j1Player::CleanUp()
{
	LOG("Freeing player");

	return true;
}
