#include <iostream>
#include <SDL/SDL.h>

using namespace std;

int main()
{
    int res;
    res = SDL_Init(SDL_INIT_EVENTTHREAD);
    if(res == 0){
        cout << "SDL init success!" << endl;
    }else{
        cout << "SDL init fail!" << endl;
    }
    return 0;
}
