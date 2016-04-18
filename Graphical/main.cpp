//
//  main.cpp
//  Graphical
//
//  XCode project supporting Horstaman Graphics Library
//
//  Modified for OSX/Xcode by Jackson Kearl on 4/18/16.
//
//  MIT Licensed... https://opensource.org/licenses/MIT
//

#include "ccc_win.h"

int ccc_win_main(){
    //circle.cpp
    Point p(0,0);
    cwin << p << Circle(p,2.5);
    return 0;

}