/*
  Rudimentary Text Based Game Engine RudiE
  ~~~~                        ~

  Daniel Grinshpon
*/
#include <iostream>
using namespace std;

extern void Start();
extern void Update();

int main()
{
    Start();
    for(;;){
        Update();
    }
    cerr << "Error: exited Update loop";
    return EXIT_FAILURE;
}
