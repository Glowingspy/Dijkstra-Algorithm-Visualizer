#include <SDL2/SDL.h>
#include <iostream>

enum CellType {
    EMPTY,
    START,
    END,
    WALL
};

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("Dijkistra Algorithm Pathfinder",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Grid properties
    const int cellSize = 25;  // Size of each cell (25x25 pixels)
    const int gridCols = 20;  // Number of columns
    const int gridRows = 20;  // Number of rows

    // Calculate the total width and height of the grid
    int gridWidth = gridCols * cellSize;
    int gridHeight = gridRows * cellSize;

    // Get the window size
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    // Calculate the starting position to center the grid
    int startX = (windowWidth - gridWidth) / 2; // Center horizontally
    int startY = (windowHeight - gridHeight) / 2; // Center vertically

    bool running = true;
    SDL_Event event;

    // Create a grid to store cell types
    CellType grid[gridRows][gridCols] = { EMPTY };

    // Define current mode (start, end, wall)
    enum Mode {
        SELECT_START,
        SELECT_END,
        SELECT_WALL
    };
    Mode currentMode = SELECT_START;

    bool mousePressed = false;  // Track mouse button state

    // Keep track of whether start and end nodes are set
    bool startSelected = false;
    bool endSelected = false;

    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                // Switch between modes based on key press
                if (event.key.keysym.sym == SDLK_s) {
                    currentMode = SELECT_START;  // 'S' for Start
                } 
                else if (event.key.keysym.sym == SDLK_e) {
                    currentMode = SELECT_END;  // 'E' for End
                }
                else if (event.key.keysym.sym == SDLK_w) {
                    currentMode = SELECT_WALL;  // 'W' for Wall
                }
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
                // Mouse button pressed, begin selecting
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mousePressed = true;
                    // Get mouse position
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);

                    // Calculate which cell was clicked
                    int col = (mouseX - startX) / cellSize;
                    int row = (mouseY - startY) / cellSize;

                    // Make sure the click is inside the grid bounds
                    if (col >= 0 && col < gridCols && row >= 0 && row < gridRows) {
                        if (currentMode == SELECT_START && !startSelected) {
                            // Only allow one start node, prevent replacing with a wall
                            if (grid[row][col] != END && grid[row][col] != WALL) {
                                grid[row][col] = START;
                                startSelected = true; // Mark start as selected
                            }
                        }
                        else if (currentMode == SELECT_END && !endSelected) {
                            // Only allow one end node, prevent replacing with a wall
                            if (grid[row][col] != START && grid[row][col] != WALL) {
                                grid[row][col] = END;
                                endSelected = true; // Mark end as selected
                            }
                        }
                        else if (currentMode == SELECT_WALL) {
                            // Set wall if no start or end node
                            if (grid[row][col] != START && grid[row][col] != END) {
                                grid[row][col] = WALL;
                            }
                        }
                    }
                }
            }
            else if (event.type == SDL_MOUSEBUTTONUP) {
                // Mouse button released, stop selecting
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mousePressed = false;
                }
            }
            else if (event.type == SDL_MOUSEMOTION && mousePressed) {
                // If the mouse is moved while holding the button
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                // Calculate which cell the mouse is over
                int col = (mouseX - startX) / cellSize;
                int row = (mouseY - startY) / cellSize;

                // Make sure the mouse is within the grid bounds
                if (col >= 0 && col < gridCols && row >= 0 && row < gridRows) {
                    // Select the appropriate cell based on current mode
                    if (currentMode == SELECT_START && !startSelected) {
                        // Only allow one start node, prevent replacing with a wall
                        if (grid[row][col] != END && grid[row][col] != WALL) {
                            grid[row][col] = START;
                            startSelected = true; // Mark start as selected
                        }
                    }
                    else if (currentMode == SELECT_END && !endSelected) {
                        // Only allow one end node, prevent replacing with a wall
                        if (grid[row][col] != START && grid[row][col] != WALL) {
                            grid[row][col] = END;
                            endSelected = true; // Mark end as selected
                        }
                    }
                    else if (currentMode == SELECT_WALL) {
                        // Set wall if no start or end node
                        if (grid[row][col] != START && grid[row][col] != END) {
                            grid[row][col] = WALL;
                        }
                    }
                }
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Draw the grid with different colors based on cell type
        for (int row = 0; row < gridRows; ++row) {
            for (int col = 0; col < gridCols; ++col) {
                SDL_Rect cellRect = { startX + col * cellSize, startY + row * cellSize, cellSize, cellSize };

                // Set color based on cell type
                if (grid[row][col] == START) {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green for start
                } 
                else if (grid[row][col] == END) {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red for end
                } 
                else if (grid[row][col] == WALL) {
                    SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);  // Grey for wall
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White for empty cells
                }

                SDL_RenderFillRect(renderer, &cellRect);  // Fill the cell with the selected color
            }
        }

        // Draw grid lines
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // White color for grid lines
        for (int x = 0; x <= gridCols; ++x) {
            int xPos = startX + x * cellSize;
            SDL_RenderDrawLine(renderer, xPos, startY, xPos, startY + gridHeight);
        }
        for (int y = 0; y <= gridRows; ++y) {
            int yPos = startY + y * cellSize;
            SDL_RenderDrawLine(renderer, startX, yPos, startX + gridWidth, yPos);
        }

        // Present the grid
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
