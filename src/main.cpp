#include <time.h>
#include "Tracer.h"

int main(int argc, char **argv)
{
    Tracer tracer;
    unsigned t = clock();
    if(!tracer.loadSettings("settings.xml")) {
        cerr << "Bad settings file" << endl;
        return 1;
    }
    tracer.renderImage();

    tracer.saveImage("result.png");
    t = clock() - t;
    cout << (float)t / CLOCKS_PER_SEC << "s" << endl;
    return 0;
}