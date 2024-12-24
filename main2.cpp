#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

enum CellType {
    EMPTY,
    START,
    END,
    WALL,
    PATH
};

int main(int argc, char** argv){
    //Initailize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return 1;
        //Return that it didnt get the correct return type
    }

    /* WINDOW ATTRIBUTES*/
    //Create the window
    SDL_Window* window = SDL_CreateWindow("Breadth First Search", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

    //If it fails to create a window
    if (!window) {
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }



    /*RENDERER ATTRIBUTES*/
    //Creates a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //If it fails to create a renderer
    if(!renderer) {
        cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }


    //Grid Properties
    const int cellSize = 25; // Size of each cell (25x25 pixels)
    const int gridCols = 20; // Number of columns
    const int gridRows = 20; // Number of rows


    //WIDTH AND HEIGHT OF THE GRID
    int gridWidth = gridCols * cellSize;
    int gridHeight = gridRows * cellSize;

    //GET THE WINDOW SIZE
    int windowWidth, windowHeight;
    //Gives the value of the window width and window height depending on the specified moniter wo window width and window height
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);


    //Calculate the starting position to center the grid
    int startX = (windowWidth - gridWidth) / 2; // Center Horizontally
    int startY = (windowHeight - gridHeight) / 2; // Center Vertically


    bool running = true;
    SDL_Event event;

    //Empty is 0 remember !!
    //An array of type CellType
    //All initialized to EMPTY
    CellType grid[gridRows][gridCols] = { EMPTY };


    //Define current mode (start, end , wall, path)
    enum Mode { 
        SELECT_START, 
        SELECT_END,
        SELECT_WALL,
        SELECT_PATH,
    };

    //currentMode is of type Mode with value of SELECT_START which is 0
    Mode currentMode = SELECT_START;

    bool mousePressed = false;

    //Keep track whather the start and end nodes are set
    bool startSelected = false;
    bool endSelected = false;


    while(running){

        while(SDL_PollEvent(&event)) { 
        
            if(event.type == SDL_QUIT) {
                //This stops the program from running when exit (X) is entered
                running = false;
            }
            //Checks when there is an input from the keyboard
            else if (event.type == SDL_KEYDOWN) {
                //Switch between modes based on key presses
                
                if(event.key.keysym.sym == SDLK_s) {
                    currentMode = SELECT_START; // "S" for Start
                }
                
                else if(event.key.keysym.sym == SDLK_e) {
                    currentMode = SELECT_END; // "E" for end is selected
                }

                else if(event.key.keysym.sym == SDLK_w) {
                    currentMode = SELECT_WALL; // "W" for wall
                }
                else if (event.key.keysym.sym == SDLK_d){
                    currentMode = SELECT_PATH; // "D" for path
                }

            }
            //The input comes from the mouse
            else if(event.type == SDL_MOUSEBUTTONDOWN) {
                if(event.button.button == SDL_BUTTON_LEFT) {
                    mousePressed = true;

                    //Get Mouse position
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);

                    //Calculate which cell was clicked
                    int col = (mouseX - startX) / cellSize;
                    int row = (mouseY - startY) / cellSize;

                    //Make sure the click is inside the grid bounds
                    if(col >= 0 && col < gridCols && row >= 0 && row < gridRows) {

                        if(currentMode == SELECT_START && !startSelected){
                            //start selected only allows one start node
                            if(grid[row][col] != END && grid[row][col] != WALL) {
                                grid[row][col] = START;
                                startSelected = true; // Mark it as selected
                            }

                        }
                        else if (currentMode == SELECT_END && !endSelected){
                            //Only allows one end node
                            if(grid[row][col] != START && grid[row][col] != WALL) {
                                grid[row][col] = END;
                                endSelected = true; // Mark end as selected
                            }
                        }
                        else if (currentMode == SELECT_WALL) {
                            if(grid[row][col] != START && grid[row][col] != END) {
                                grid[row][col] = WALL;
                            }
                        }
                    }
                }

            }
            else if(event.type == SDL_MOUSEBUTTONUP) {
                //If mouse button released, stop selecting
                if(event.button.button == SDL_BUTTON_LEFT) {
                    mousePressed = false;
                }
            }
            else if (event.type == SDL_MOUSEMOTION && mousePressed){
                //If the mouse is moved while holding the button
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                //Calculate which cell the mouse is over
                int col = (mouseX - startX) / cellSize;
                int row = (mouseY - startY) / cellSize;

                if(col >= 0 && col < gridCols && row >= 0 && row < gridRows) {
                    //Select the appropirate cell based on current mode
                    if(currentMode == SELECT_START && !startSelected) {
                        if(grid[row][col] != END && grid[row][col] != WALL) {
                            grid[row][col] = START;
                            startSelected = true;
                        }
                    }
                    else if(currentMode == SELECT_END && !endSelected) {
                        // Only allow one end node
                        if(grid[row][col] != START && grid[row][col] != WALL) {
                            grid[row][col] = END;
                            endSelected = true;
                        }
                    }
                    else if (currentMode == SELECT_WALL) {
                        //Set Wall if it isnt a start or an end node
                        if(grid[row][col] != START && grid[row][col] != END) {
                            grid[row][col] = WALL;
                        }
                    }
                }
            }
        }
        //Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //THE two for loops are bounding the cell
        for(int row = 0; row < gridRows; ++row) {
            for(int col = 0; col < gridCols; ++col) {
                SDL_Rect cellRect = { startX + col * cellSize, startY + row * cellSize, cellSize, cellSize};

                //Set color based on cell type
                if(grid[row][col] == START) {
                    SDL_SetRenderDrawColor(renderer, 0, 255 , 0, 255);//Green
                }
                else if(grid[row][col] == END) {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);//Red
                }
                else if(grid[row][col] == WALL) {
                    SDL_SetRenderDrawColor(renderer, 169,169,169,255);//Grey
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 255,255,255,255);//White
                }
                SDL_RenderFillRect(renderer, &cellRect); // Fill the cell with the designated color

            }
        }

        //Draw grid lines
        SDL_SetRenderDrawColor(renderer, 100, 100,100,255);//White color for grid lines

        for(int x = 0; x <= gridCols; ++x){
            int xPos = startX + x * cellSize;
            SDL_RenderDrawLine(renderer, xPos, startY, xPos, startY + gridHeight);
        }

        for(int y = 0; y < gridRows; ++y) {
            int yPos = startY + y *cellSize;
            SDL_RenderDrawLine(renderer, startX, yPos, startX + gridWidth, yPos);
        } 


        SDL_RenderPresent(renderer);
    }

    //Clean Up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}