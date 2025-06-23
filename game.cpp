#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH  = 60;
const int HEIGHT = 10;
const int FPS    = 60;   
const int FRAME_TIME = 1000 / FPS;

struct Obstacle { int x, y; };

int main() {
    srand((unsigned)time(nullptr));
    bool   running = true;
    int    dinoY   = HEIGHT-1;
    int    velY    = 0;
    vector<Obstacle> obs;
    int    score   = 0;
    int    spawnTimer = 0;

    while (running) {
        DWORD start = GetTickCount();

        // Input
        if (kbhit()) {
            char c = getch();
            if ((c==' ' || c=='w' || c=='W') && dinoY == HEIGHT-1) {
                velY = -12;  // jump impulse
            }
            if (c=='q' || c=='Q') {
                running = false;
            }
        }

        // Physics
        dinoY += velY;
        velY  += 1;               // gravity
        if (dinoY > HEIGHT-1) {
            dinoY = HEIGHT-1;
            velY  = 0;
        }

        // Spawn obstacles
        if (++spawnTimer > FPS/2) {
            spawnTimer = 0;
            obs.push_back({ WIDTH-1, HEIGHT-1 });
        }

        // Move obstacles & collision
        for (auto &o : obs) {
            o.x--;
            if (o.x == 5 && o.y == dinoY) {
                running = false;
            }
        }
        while (!obs.empty() && obs.front().x < 0)
            obs.erase(obs.begin());

        // Render
        system("cls");
        // empty rows
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                bool drew = false;
                // Dino
                if (x==5 && y==dinoY) {
                    cout << "D";
                    drew = true;
                }
                // Obstacles
                for (auto &o : obs) {
                    if (o.x==x && o.y==y) {
                        cout << "|";
                        drew = true;
                    }
                }
                if (!drew) {
                    if (y == HEIGHT-1) cout << "_";
                    else               cout << " ";
                }
            }
            cout << "\n";
        }

        // Score
        cout << "\n Score: " << score++;

        // Frame cap
        DWORD delta = GetTickCount() - start;
        if (delta < FRAME_TIME)
            Sleep(FRAME_TIME - delta);
    }

    cout << "\n\n  Game Over! Final Score: " << score << "\n";
    return 0;
}
