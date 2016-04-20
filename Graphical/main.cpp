//
//  main.cpp
//
//  Written by Jackson Kearl on 4/18/16.
//
//  MIT Licensed... https://opensource.org/licenses/MIT
//

#include "ccc_win.h"

const bool comp = true;

int board[3][3];

void make_board() {
    cwin.clear();
    Point p02(0,2);
    Point p04(0,4);
    Point p62(6,2);
    Point p64(6,4);
    Point p20(2,0);
    Point p40(4,0);
    Point p26(2,6);
    Point p46(4,6);
    
    Line l1(p02, p62);
    Line l2(p04, p64);
    Line l3(p20, p26);
    Line l4(p40, p46);
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = 1;
        }
    }
    cwin << l1 << l2 << l3 << l4;
}

int round_to_odd(double val) {
    return ((int)val/2) * 2 + 1;
}

Point get_user_location(string message) {
    Point raw_location = cwin.get_mouse(message);

    double raw_x = raw_location.get_x();
    double raw_y = raw_location.get_y();
    return Point(round_to_odd(raw_x), round_to_odd(raw_y));
}

void draw_object(char to_place, Point place_location) {
    double center_x = place_location.get_x();
    double center_y = place_location.get_y();
    switch (to_place) {
        case 'O':
            cwin << Circle(place_location, 0.75);
            break;
            
        case 'X':
            cwin << Line(Point(center_x-0.75, center_y-0.75),
                         Point(center_x+0.75, center_y+0.75));
            
            cwin << Line(Point(center_x-0.75, center_y+0.75),
                         Point(center_x+0.75, center_y-0.75));
            break;
        case 'Y': //check mark, yes continue
            cwin << Line(Point(center_x-0.75, center_y+0.75),
                         Point(center_x+0.75, center_y-0.75));
            cwin << Line(Point(center_x-0.75, center_y+0.75),
                         Point(center_x-1.25,    center_y));
            break;
        case 'N': //slashed circle, no continue
            cwin << Circle(place_location, .92);
            cwin << Line(Point(center_x+0.65, center_y+0.65),
                         Point(center_x-0.65, center_y-0.65));
            break;
    }
}

bool should_continue() {
    
    cwin.clear();
    draw_object('Y', Point(4.5, 3));
    draw_object('N', Point(1.5, 3));
    Point click = get_user_location("Game over, play again...?");
    return (click.get_x() >= 3);
}

int* point_to_board(Point point) {
    double x = point.get_x();
    double y = point.get_y();
    int col = x/2 ;
    int row = y/2 ;
    return &board[row][col];
}

bool valid_place( Point place_location) {
    int prev = * point_to_board(place_location);
    return prev == 1;
}

void record_move( Point place_location, char to_place) {
    int* loc = point_to_board(place_location);
    if (to_place == 'O')
        *loc = 3;
    if (to_place == 'X')
        *loc = 2;
}

int evaluate_board() {
    int paths[8][3][2] =
    {
        {{0,0},{1,0},{2,0}},
        {{0,1},{1,1},{2,1}},
        {{0,2},{1,2},{2,2}},
        {{0,0},{0,1},{0,2}},
        {{1,0},{1,1},{1,2}},
        {{2,0},{2,1},{2,2}},
        {{0,0},{1,1},{2,2}},
        {{2,0},{1,1},{0,2}}
    };
    for (int path = 0; path < 8; path++) {
        int product = 1;
        for (int cell = 0; cell < 3; cell++) {
            product *= board[paths[path][cell][0]][paths[path][cell][1]];
        }
        if (product == 8)
            return path+1;
        if (product == 27)
            return -(path+1);
    }
    return 0;
}

Point get_best_move() {
    Point best(-1,-1);
    int paths[8][3][2] =
    {
        {{0,0},{1,0},{2,0}},
        {{0,1},{1,1},{2,1}},
        {{0,2},{1,2},{2,2}},
        {{0,0},{0,1},{0,2}},
        {{1,0},{1,1},{1,2}},
        {{2,0},{2,1},{2,2}},
        {{0,0},{1,1},{2,2}},
        {{2,0},{1,1},{0,2}}
    };
    for (int path = 0; path < 8; path++) {
        int product = 1;
        for (int cell = 0; cell < 3; cell++) {
            product *= board[paths[path][cell][0]][paths[path][cell][1]];
        }
        if (product == 4) {
            for (int cell = 0; cell < 3; cell++) {
                if ( board[paths[path][cell][0]][paths[path][cell][1]] == 1){
                    double x = paths[path][cell][0] * 2 + 1;
                    double y = paths[path][cell][1] * 2 + 1;
                    best = Point(y, x);
                }
            }
        }
        if (product == 9) {
            for (int cell = 0; cell < 3; cell++) {
                if ( board[paths[path][cell][0]][paths[path][cell][1]] == 1){
                    double x = paths[path][cell][0] * 2 + 1;
                    double y = paths[path][cell][1] * 2 + 1;
                    return Point(y, x);
                }
            }
        }
    }
    
    if (best.get_x() == -1) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == 1) {
                    return Point(j*2 + 1, i*2 + 1);
                }
            }
        }
    }
    return best;
}

void draw_line(int pos) {
    double x_start = 0;
    double x_end = 0;
    double y_start = 0;
    double y_end = 0;
    switch (pos) {
        case 1:
            x_start = 1;
            x_end = 1;
            y_start = 0.25;
            y_end = 5.75;
            break;
        case 2:
            x_start = 3;
            x_end = 3;
            y_start = 0.25;
            y_end = 5.75;
            break;
        case 3:
            x_start = 5;
            x_end = 5;
            y_start = 0.25;
            y_end = 5.75;
            break;
        case 4:
            x_start = 0.25;
            x_end = 5.75;
            y_start = 1;
            y_end = 1;
            break;
        case 5:
            x_start = 0.25;
            x_end = 5.75;
            y_start = 3;
            y_end = 3;
            break;
        case 6:
            x_start = 0.25;
            x_end = 5.75;
            y_start = 5;
            y_end = 5;
            break;
        case 7:
            x_start = 0.3;
            x_end = 5.7;
            y_start = 0.2;
            y_end = 5.8;
            break;
        case 8:
            x_start = 5.7;
            x_end = 0.3;
            y_start = 0.2;
            y_end = 5.8;
            break;
    }
    cwin << Line(Point(x_start, y_start), Point(x_end, y_end));
}

bool has_ended(int num_turns) {
    int victor = evaluate_board();
    if (victor < 0) {
        draw_line(-victor);
        cwin.get_mouse("Player 2 Wins! Click to continue.");
        return true;
    }
    if (victor > 0) {
        draw_line(victor);
        cwin.get_mouse("Player 1 Wins! Click to continue.");
        return true;
    }
    if (num_turns == 9) {
        cwin.get_mouse("Cats Game! Click to continue.");
        return true;
    }
    return false;
}

int ccc_win_main(){
    cwin.coord(0, 0, 6, 6);
    
    do {
        make_board();
        for (int num_turns = 0; !has_ended(num_turns); num_turns++) {
            char to_place = '\0';
            Point place_location;
            bool was_valid = true;
            do {
                if (!was_valid) {
                    place_location = get_user_location("Oops! Someone already played there... try agian. ");

                } else if (num_turns % 2 == 0) {
                    to_place = 'X';
                    place_location = get_user_location("Player 1, choose a location");

                } else {
                    to_place = 'O';
                    if (comp) {
                        place_location = get_best_move();
                    }
                    else {
                        place_location = get_user_location("Player 2, choose a location");
                    }
                }
                was_valid = false;
            
            } while( !valid_place(place_location));
            
            record_move( place_location, to_place );
            draw_object(to_place, place_location);
        }
    } while( should_continue() );
    
    cwin.clear();
    cwin << Message(Point(2.5,3), "Goodbye :)");
    return 0;
}