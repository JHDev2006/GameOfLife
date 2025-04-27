#include <iostream>
#include <SDL.h>
#include <algorithm>

const int WORLD_SIZE_X = 1920;
const int WORLD_SIZE_Y = 1080;
const int SPEED = 1;

bool world[WORLD_SIZE_X][WORLD_SIZE_Y]{false};
bool last_world[WORLD_SIZE_X][WORLD_SIZE_Y]{true};

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

		for (int i = 0; i < SPEED; i++)
		{
			process_world();
		}
		render(renderer);
		SDL_Delay(16);
		std::cout << "Tick" << std::endl;
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

void render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 51, 57, 65, 255);
	SDL_RenderClear(renderer);
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
			int mouseX = event.button.x;
			int mouseY = event.button.y;
			if (mouseX >= 0 && mouseX < WORLD_SIZE_X && mouseY >= 0 && mouseY < WORLD_SIZE_Y) {
				add_random_cells(mouseX, mouseY);
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

    // Sync to world
    for (int x = 0; x < WORLD_SIZE_X; x++) {
        for (int y = 0; y < WORLD_SIZE_Y; y++) {
            world[x][y] = last_world[x][y];
        }
    }
}

void add_random_cells(int pos_x, int pos_y)
{
	std::cout << pos_x << " " << pos_y << std::endl;
	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
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
	world[pos_x][pos_y - 1] = true;
	world[pos_x + 1][pos_y] = true;
	world[pos_x - 1][pos_y + 1] = true;
	world[pos_x][pos_y + 1] = true;
	world[pos_x + 1][pos_y + 1] = true;
	process_world();
}

void process_world()
{
	for (int x = 0; x < WORLD_SIZE_X; x++)
	{
		for (int y = 0; y < WORLD_SIZE_Y; y++)
		{
			last_world[x][y] = world[x][y];
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
			bool cell = world[x][y];
			if (cell == true)
			{
				SDL_RenderPoint(renderer, x, y);
			}
		}
	}
}

bool process_cell(int pos_x, int pos_y) {
	int live_neighbours = 0;

	for (int x = -1; x <= 1; x++) {
		for (int y = -1; y <= 1; y++) {
			// Skip the center cell
			if (x == 0 && y == 0) continue;

			int n_x = pos_x + x;
			int n_y = pos_y + y;

			// Check bounds
			if (n_x >= 0 && n_x < WORLD_SIZE_X && n_y >= 0 && n_y < WORLD_SIZE_Y) {
				if (last_world[n_x][n_y] == true) {
					live_neighbours++;
				}
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



