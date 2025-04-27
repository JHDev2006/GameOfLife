#include <iostream>
#include <SDL.h>
#include <algorithm>

const int WORLD_SIZE_X = 960;
const int WORLD_SIZE_Y = 960;
int ZOOM = 1;
int SPEED = 1;

bool world[WORLD_SIZE_X][WORLD_SIZE_Y]{false};
bool last_world[WORLD_SIZE_X][WORLD_SIZE_Y]{true};

SDL_FPoint camera_position = SDL_FPoint{0, 0};

bool process_cell(int pos_x, int pos_y);

void render(SDL_Renderer* renderer);
void draw_world(SDL_Renderer* renderer);

void process_world();

void setup_world();

bool handle_events();

void add_random_cells(int pos_x, int pos_y);
void make_glider(int pos_x, int pos_y);

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Game of Life", WORLD_SIZE_X, WORLD_SIZE_Y, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

	setup_world();

	bool quit = false;

	while (!handle_events())
	{
		process_world();
		render(renderer);
		SDL_Delay(16);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

void render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 51, 57, 65, 255);
	SDL_FRect bg_rect = SDL_FRect{0 - (camera_position.x * ZOOM), 0 - (camera_position.y * ZOOM), float(WORLD_SIZE_X) * ZOOM, float(WORLD_SIZE_Y) * ZOOM};
	SDL_RenderFillRect(renderer, &bg_rect);
	draw_world(renderer);
	SDL_RenderPresent(renderer);
}

bool handle_events()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
		{
			return true;
		}

		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				int mouseX = event.button.x;
				int mouseY = event.button.y;
				int globalMouseX = mouseX - camera_position.x;
				int globalMouseY = mouseY - camera_position.y;
				add_random_cells((mouseX / ZOOM) + camera_position.x, (mouseY / ZOOM) + camera_position.y);
			}
		}
		if (event.type == SDL_EVENT_MOUSE_WHEEL)
		{
			if (event.wheel.y < 0 && ZOOM > 1)
			{
				ZOOM--;
			} else if (event.wheel.y > 0)
			{
				camera_position.x -= camera_position.x / ZOOM + 1;
				ZOOM++;
			}
		}
		if (event.type == SDL_EVENT_MOUSE_MOTION)
		{
			Uint32 mouse_state = SDL_GetGlobalMouseState(NULL, NULL);
			if (mouse_state & SDL_BUTTON_LMASK)
			{
				camera_position.x -= event.motion.xrel / ZOOM;
				camera_position.y -= event.motion.yrel / ZOOM;
			}
		}
	}
	return false;
}

void setup_world() {
    // Clear all
    for (int x = 0; x < WORLD_SIZE_X; x++) {
        for (int y = 0; y < WORLD_SIZE_Y; y++) {
            last_world[x][y] = world[x][y] = false;
        }
    }
}

void add_random_cells(int pos_x, int pos_y)
{
	std::cout << pos_x << " " << pos_y << std::endl;
	for (int x = 0; x < 50; x++)
	{
		for (int y = 0; y < 50; y++)
		{
			if (rand() % 2 == 0)
			{
				world[x + pos_x][y + pos_y] = true;
			}
		}
	}
}

void make_glider(int pos_x, int pos_y)
{
	world[pos_x][pos_y] = true;
	world[pos_x - 1][pos_y] = true;
	world[pos_x + 1][pos_y] = true;
}

void process_world()
{

	for (int x = 0; x < WORLD_SIZE_X; x++)
	{
		for (int y = 0; y < WORLD_SIZE_Y; y++)
		{
			last_world[x][y] = world[x][y];
		}
	}

	for (int x = 0; x < WORLD_SIZE_X; x++)
	{
		for (int y = 0; y < WORLD_SIZE_Y; y++)
		{
			world[x][y] = process_cell(x, y);
		}
	}
}

void draw_world(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for (int x = 0; x < WORLD_SIZE_X; x++)
	{
		for (int y = 0; y < WORLD_SIZE_Y; y++)
		{
			if (world[x][y] == true)
			{
				SDL_FRect rect = SDL_FRect{(float(x) - camera_position.x) * ZOOM, (float(y) - camera_position.y) * ZOOM, ZOOM, ZOOM};
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}
}

bool process_cell(int pos_x, int pos_y) {
	int live_neighbours = 0;

	last_world[pos_x][pos_y] = world[pos_x][pos_y];

	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			// Skip the center cell
			if (x == 0 && y == 0) continue;

			int n_x = pos_x + x;
			int n_y = pos_y + y;

			bool neighbour_cell = last_world[n_x][n_y];
			if (neighbour_cell == true)
			{
				live_neighbours++;
			}
		}
	}

	// Apply Conway's rules
	if (last_world[pos_x][pos_y]) {
		return (live_neighbours == 2 || live_neighbours == 3); // Survival
	} else {
		return (live_neighbours == 3); // Birth
	}
}



