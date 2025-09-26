#include <SDL.h>

#include <iostream>
#include <vector>
#include <random> 
#include <chrono>

const int ROWS = 50;
const int COLS = 40;

int countNeighbor(const std::vector<std::vector<int>>& grid, int row, int col){
    
    int count = 0;
    for(int i = -1; i <= 1; ++i)
        for(int j = -1; j <= 1; ++j){
            if(i == 0 && j == 0) continue;
            if((row+i >= 0) && (col+j) >= 0 && (row+i < ROWS) && (col+j) < COLS) count += grid[row+i][col+j];
        }

    return count;
}


void updateGrid(std::vector<std::vector<int>>& grid){
    std::vector<std::vector<int>> temp(ROWS, std::vector<int>(COLS, 0));
    for(int i = 0; i < ROWS; ++i)
        for(int j = 0; j < COLS; ++j){
            int aliveNeighborNum = countNeighbor(grid, i, j);
            if(grid[i][j]){
                if(aliveNeighborNum < 2) temp[i][j] = 0;
                if(aliveNeighborNum == 2 || aliveNeighborNum == 3) temp[i][j] = 1;
                else temp[i][j] = 0;
        }
        else{
            if(aliveNeighborNum == 3) temp[i][j] = 1;
    } 
    }
    grid = std::move(temp);
}

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 800;

const int cellSize = SCREEN_WIDTH / ROWS;

int main(int argc, char* args[]) {
    SDL_Window* window = NULL;
    SDL_Surface* screenSurface = NULL;

    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // 创建窗口
    window = SDL_CreateWindow("SDL Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool quit = false;
    SDL_Event e;

    // //初始化一个二维vector，使其初始值随机为0或1
    // std::random_device rd;  
    // std::mt19937 gen(rd()); 
    unsigned seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    // 定义一个整数分布，范围是 [0, 1] (闭区间)
    std::uniform_int_distribution<> distrib(0, 1);

    std::vector<std::vector<int>> grid(ROWS, std::vector<int>(COLS));
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            grid[i][j] = distrib(gen);
        }
    }

   while (!quit) {
        // 4. 事件处理
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // --- 游戏逻辑更新 ---
        updateGrid(grid);

        // --- 渲染 ---
        // 5.1 用背景色清空屏幕
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // 白色
        SDL_RenderClear(renderer);

        // 5.2 绘制活细胞
        // 遍历你的游戏世界网格
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                if (grid[i][j]) {
                    SDL_Rect cellRect = { j * cellSize, i * cellSize, cellSize, cellSize };
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 黑色
                    SDL_RenderFillRect(renderer, &cellRect);
                }
            }
        }

        // 6. 更新屏幕
        SDL_RenderPresent(renderer);

        // 7. 控制帧率
        SDL_Delay(100); // 暂停100毫秒
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}