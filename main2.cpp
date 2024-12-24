#include <SDL2/SDL.h>
#include <iostream>
#include <queue>
#include <vector>
#include <climits>
#include <stack>

using namespace std;

enum CellType {
    EMPTY,
    START,
    END,
    WALL,
    PATH
};

struct Node {
    int x, y;
    Node* parent; // To track the parent node for path tracing
    Node(int _x, int _y) : x(_x), y(_y), parent(nullptr) {}
};

// Direction vectors for 8 possible movements (up, down, left, right, diagonals)
int dx[] = {0, 0, -1, 1, -1, 1, -1, 1};
int dy[] = {-1, 1, 0, 0, -1, -1, 1, 1};
int cost[] = {1, 1, 1, 1, 2, 2, 2, 2}; // Costs for each move

// Dijkstra's algorithm to find the shortest path
void dijkstra(CellType grid[20][20], Node& startNode, Node& endNode, vector<pair<int, int>>& path) {
    const int gridCols = 20;
    const int gridRows = 20;

    // Distance matrix
    vector<vector<int>> distance(gridRows, vector<int>(gridCols, INT_MAX));
    distance[startNode.y][startNode.x] = 0;

    // Priority queue for Dijkstra's algorithm
    auto cmp = [](pair<int, Node*> a, pair<int, Node*> b) { return a.first > b.first; };
    priority_queue<pair<int, Node*>, vector<pair<int, Node*>>, decltype(cmp)> pq(cmp);
    pq.push({0, &startNode});

    // Map to store parent nodes for path reconstruction
    Node* parent[20][20] = {nullptr};

    while (!pq.empty()) {
        pair<int, Node*> top = pq.top();
        int dist = top.first;
        Node* current = top.second;
        pq.pop();

        // If we reach the end node, stop
        if (current->x == endNode.x && current->y == endNode.y) {
            break;
        }

        // Explore neighbors
        for (int i = 0; i < 8; ++i) {
            int newX = current->x + dx[i];
            int newY = current->y + dy[i];
            int moveCost = cost[i];

            if (newX >= 0 && newY >= 0 && newX < gridCols && newY < gridRows &&
                grid[newY][newX] != WALL) {

                int newDist = dist + moveCost;
                if (newDist < distance[newY][newX]) {
                    distance[newY][newX] = newDist;
                    Node* neighbor = new Node(newX, newY);
                    parent[newY][newX] = current;
                    pq.push({newDist, neighbor});
                }
            }
        }
    }

    // Trace back the path from end to start
    Node* current = &endNode;
    stack<pair<int, int>> pathStack;

    while (current != nullptr) {
        pathStack.push({current->x, current->y});
        current = parent[current->y][current->x];
    }

    // Push the path nodes to the path vector
    while (!pathStack.empty()) {
        path.push_back(pathStack.top());
        pathStack.pop();
    }
}


int main(int argc, char** argv) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Dijkstra's Algorithm", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    const int cellSize = 25;
    const int gridCols = 20;
    const int gridRows = 20;
    int gridWidth = gridCols * cellSize;
    int gridHeight = gridRows * cellSize;
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    int startX = (windowWidth - gridWidth) / 2;
    int startY = (windowHeight - gridHeight) / 2;

    bool running = true;
    SDL_Event event;

    // Initialize grid with all cells as EMPTY
    CellType grid[gridRows][gridCols] = { EMPTY };

    enum Mode {
        SELECT_START,
        SELECT_END,
        SELECT_WALL,
        SELECT_PATH
    };
    Mode currentMode = SELECT_START;

    bool mousePressed = false;

    bool startSelected = false;
    bool endSelected = false;

    Node* startNode = nullptr;
    Node* endNode = nullptr;

    vector<pair<int, int>> path;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_s) {
                    currentMode = SELECT_START;
                } else if (event.key.keysym.sym == SDLK_e) {
                    currentMode = SELECT_END;
                } else if (event.key.keysym.sym == SDLK_w) {
                    currentMode = SELECT_WALL;
                } else if (event.key.keysym.sym == SDLK_d) {
                    if (startNode && endNode) {
                        path.clear();
                        dijkstra(grid, *startNode, *endNode, path); // Run Dijkstra when 'D' is pressed
                    }
                }
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mousePressed = true;

                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    int col = (mouseX - startX) / cellSize;
                    int row = (mouseY - startY) / cellSize;

                    if (col >= 0 && col < gridCols && row >= 0 && row < gridRows) {
                        if (currentMode == SELECT_START && !startSelected) {
                            grid[row][col] = START;
                            startNode = new Node(col, row); // Set start node
                            startSelected = true;
                        } else if (currentMode == SELECT_END && !endSelected) {
                            grid[row][col] = END;
                            endNode = new Node(col, row); // Set end node
                            endSelected = true;
                        } else if (currentMode == SELECT_WALL) {
                            grid[row][col] = WALL;
                        }
                    }
                }
            }

            else if(event.type == SDL_MOUSEBUTTONUP){
                if(event.button.button == SDL_BUTTON_LEFT) {
                    mousePressed = false;
                }
            }
            else if (event.type == SDL_MOUSEMOTION && mousePressed){
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);

                int col = (mouseX - startX) / cellSize;
                int row = (mouseY - startY) / cellSize;

                if(col >= 0 && col < gridCols && row >= 0 && row < gridRows) {
                    if(currentMode == SELECT_START && !startSelected) {
                        if(grid[row][col] != END && grid[row][col] != WALL) {
                            grid[row][col] = START;
                            startSelected = true;
                        }
                    }
                    else if (currentMode == SELECT_END && !endSelected) {
                        if(grid[row][col] != START && grid[row][col] != WALL) {
                            grid[row][col] = END;
                            endSelected = true;
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

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Render the grid
        for (int row = 0; row < gridRows; ++row) {
            for (int col = 0; col < gridCols; ++col) {
                SDL_Rect cellRect = { startX + col * cellSize, startY + row * cellSize, cellSize, cellSize };

                if (grid[row][col] == START) {
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
                } else if (grid[row][col] == END) {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
                } else if (grid[row][col] == WALL) {
                    SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255); // Gray
                } else {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White for empty
                }

                SDL_RenderFillRect(renderer, &cellRect);
            }
        }

        // Draw grid lines
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // Light gray grid lines
        for (int x = 0; x <= gridCols; ++x) {
            int xPos = startX + x * cellSize;
            SDL_RenderDrawLine(renderer, xPos, startY, xPos, startY + gridHeight);
        }
        for (int y = 0; y <= gridRows; ++y) {
            int yPos = startY + y * cellSize;
            SDL_RenderDrawLine(renderer, startX, yPos, startX + gridWidth, yPos);
        }

        // Draw the shortest path
        if (!path.empty()) {
            SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255); // Purple for the path

            for (size_t i = 1; i < path.size(); ++i) {
                int x1 = startX + path[i - 1].first * cellSize + cellSize / 2;
                int y1 = startY + path[i - 1].second * cellSize + cellSize / 2;
                int x2 = startX + path[i].first * cellSize + cellSize / 2;
                int y2 = startY + path[i].second * cellSize + cellSize / 2;
                SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
            }
        }

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
