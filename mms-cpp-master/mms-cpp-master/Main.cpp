#include <iostream>
#include <string>
#include <stack>
#include <utility>
#include "API.h"
using namespace std;


const int MAX_x = 16;
const int MAX_y = 16;



int matrix[MAX_y][MAX_x];
int floods[MAX_y][MAX_x];

//indicate destination block
const int DEST_X = 3;
const int DEST_Y = 3;



//check whether current x and y are valid
bool checkValid(int x,int y);
//check wall around the vehicle
//bool checkWall(int x,int y,int direction);
void checkWall(int x, int y, int direction, bool& left, bool& right, bool& front);


struct Node
{
    int x;
    int y;
    //double f,g,h;
    int wall_w;
    int wall_e;
    int wall_n;
    int wall_s;
    Node* parent;
};
void log(const std::string& text) {
    std::cerr << text << std::endl;
}
void log_num(int x)
{
    std::cerr<<x<<std::endl;
}
void log_bool(bool y)
{
    std::cerr<<y<<std::endl;
}


bool checkValid(int &x, int &y)
{
    if(x >= 0 && x <= MAX_x && y >=0 && y <= MAX_y)
    {
        return true;
    }
    else
    {
        return false;
    }
}
// configure matrix map with walls
void setMatrix(bool left, bool right, bool front, int x,int y, int direction){
    if(direction == 0){
        if(left&&right&&front){
            matrix[x][y] = 1;//world left right front
        } else if(!left&&right&&front) {
            matrix[x][y] = 2;//world right front
        } else if(left&&!right&&front) {
            matrix[x][y] = 3;//world left front
        } else if(left&&right&&!front) {
            matrix[x][y] = 4;//world left right
        } else if(!left&&!right&&front) {
            matrix[x][y] = 5;//world front
        } else if(!left&&right&&!front) {
            matrix[x][y] = 6;//world right
        } else if(left&&!right&&!front) {
            matrix[x][y] = 7;//world left
        } else if(!left&&!right&&!front) {
            matrix[x][y] = 15;//no wall
        }

    }
    if(direction == 1) {
        if (left && right && front) {
            matrix[x][y] = 9;//world front right back
        } else if (!left && right && front) {
            matrix[x][y] = 12;//world back right
        } else if (left && !right && front) {
            matrix[x][y] = 2;//world right front
        } else if (left && right && !front) {
            matrix[x][y] = 13;//world front back
        } else if (!left && !right && front) {
            matrix[x][y] = 6;//world right
        } else if (!left && right && !front) {
            matrix[x][y] = 8;//world back
        } else if (left && !right && !front) {
            matrix[x][y] = 5;//world front
        } else if (!left && !right && !front) {
            matrix[x][y] = 15;//no wall
        }
    }
    if(direction == 2) {
        if (left && right && front) {
            matrix[x][y] = 10;//world left right back
        } else if (!left && right && front) {
            matrix[x][y] = 14;//world back left
        } else if (left && !right && front) {
            matrix[x][y] = 12;//world back right
        } else if (left && right && !front) {
            matrix[x][y] = 4;//world left right
        }else if (!left && !right && front) {
            matrix[x][y] = 8;//world back
        } else if (!left && right && !front) {
            matrix[x][y] = 7;//world left
        } else if (left && !right && !front) {
            matrix[x][y] = 6;//world right
        } else if (!left && !right && !front) {
            matrix[x][y] = 15;//no wall
        }
    }
    if(direction == 3) {
        if (left && right && front) {
            matrix[x][y] = 11;//world front left back
        } else if (!left && right && front) {
            matrix[x][y] = 3;//world left front
        } else if (left && !right && front) {
            matrix[x][y] = 14;//world back left
        } else if (left && right && !front) {
            matrix[x][y] = 13;//world front back
        }  else if (!left && !right && front) {
            matrix[x][y] = 7;//world left
        } else if (!left && right && !front) {
            matrix[x][y] = 5;//world front
        } else if (left && !right && !front) {
            matrix[x][y] = 8;//world back
        } else if (!left && !right && !front) {
            matrix[x][y] = 15;//no wall
        }
    }
}

void checkWall(int x, int y, int direction, bool& left, bool& right, bool& front){
    left = false;
    front = false;
    right = false;
    switch (direction) {
        case 0:
            if (API::wallLeft()) {
                API::setWall(x, y, 'w');
                left = true;
            }
            if (API::wallFront()) {
                API::setWall(x, y, 'n');
                front = true;
            }
            if (API::wallRight()) {
                API::setWall(x, y, 'e');
                right = true;
            }
            break;
        case 1:
            if (API::wallLeft()) {
                API::setWall(x, y, 'n');
                left = true;
            }
            if (API::wallFront()) {
                API::setWall(x, y, 'e');
                front = true;
            }
            if (API::wallRight()) {
                API::setWall(x, y, 's');
                right = true;
            }
            break;
        case 2:
            if (API::wallLeft()) {
                API::setWall(x, y, 'e');
                left = true;
            }
            if (API::wallFront()) {
                API::setWall(x, y, 's');
                front = true;
            }
            if (API::wallRight()) {
                API::setWall(x, y, 'w');
                right = true;
            }
            break;
        case 3:
            if (API::wallLeft()) {
                API::setWall(x, y, 's');
                left = true;
            }
            if (API::wallFront()) {
                API::setWall(x, y, 'w');
                front = true;
            }
            if (API::wallRight()) {
                API::setWall(x, y, 'n');
                right = true;
            }
            break;
    }
}

//void flood(int matrix[MAX_x][MAX_y], int floods[MAX_x][MAX_y], int rowMax, int x, int y, int position, bool update) {
//    if(update){
//        if(floods[x][y] > position) floods[x][y] = position;
//    } else {
//        floods[x][y] = position;
//    }
//
//    if(x > rowMax || x < 0 || y > rowMax || y < 0 || position > 10 ) {
//        return;
//    }
//
//    change[x][y] = true;
//
//    if(matrix[x][y] == 1) {//wall left right front
//        flood(matrix, floods, rowMax, x, y - 1, position + 1,update);
//    } else if(matrix[x][y] == 2) {//wall right front
//        flood(matrix, floods, rowMax, x - 1, y, position + 1,update);
//        flood(matrix, floods, rowMax, x, y - 1, position + 1,update);
//    } else if(matrix[x][y] == 3) {//wall left front
//        flood(matrix, floods, rowMax, x + 1, y, position + 1,update);
//        flood(matrix, floods, rowMax, x, y - 1, position + 1,update);
//    } else if(matrix[x][y] == 4) {//wall left right
//        flood(matrix, floods, rowMax, x, y + 1, position + 1,update);
//        flood(matrix, floods, rowMax, x, y - 1, position + 1,update);
//    } else if(matrix[x][y] == 5) {//wall front
//        flood(matrix, floods, rowMax, x - 1, y, position + 1,update);
//        flood(matrix, floods, rowMax, x + 1, y, position + 1,update);
//        flood(matrix, floods, rowMax, x, y - 1, position + 1,update);
//    } else if(matrix[x][y] == 6) {//wall right
//        flood(matrix, floods, rowMax, x - 1, y, position + 1,update);
//        flood(matrix, floods, rowMax, x, y - 1, position + 1,update);
//        flood(matrix, floods, rowMax, x, y + 1, position + 1,update);
//    } else if(matrix[x][y] == 7) {//wall left
//        flood(matrix, floods, rowMax, x + 1, y, position + 1,update);
//        flood(matrix, floods, rowMax, x, y + 1, position + 1,update);
//        flood(matrix, floods, rowMax, x, y - 1, position + 1,update);
//    } else if(matrix[x][y] == 8) {//wall back
//        flood(matrix, floods, rowMax, x, y + 1, position + 1,update);
//        flood(matrix, floods, rowMax, x - 1, y, position + 1,update);
//        flood(matrix, floods, rowMax, x + 1, y, position + 1,update);
//    } else if(matrix[x][y] == 9) {//wall front right back
//        flood(matrix, floods, rowMax, x - 1, y, position + 1,update);
//        //flood(matrix, flood, rowMax, x - 1, y, position + 1);
//    } else if(matrix[x][y] == 10) {//wall left right back
//        flood(matrix, floods, rowMax, x, y + 1, position + 1,update);
//        //flood(matrix, flood, rowMax, x - 1, y, position + 1);
//    } else if(matrix[x][y] == 11) {//wall front left back
//        flood(matrix, floods, rowMax, x + 1, y, position + 1,update);
//        //flood(matrix, flood, rowMax, x - 1, y, position + 1);
//    } else if(matrix[x][y] == 12) {//wall back right
//        flood(matrix, floods, rowMax, x, y + 1, position + 1,update);
//        flood(matrix, floods, rowMax, x - 1, y, position + 1,update);
//    } else if(matrix[x][y] == 13) {//wall front back
//        flood(matrix, floods, rowMax, x - 1, y, position + 1,update);
//        flood(matrix, floods, rowMax, x + 1, y, position + 1,update);
//    } else if(matrix[x][y] == 14) {//wall back left
//        flood(matrix, floods, rowMax, x, y + 1, position + 1,update);
//        flood(matrix, floods, rowMax, x + 1, y, position + 1,update);
//    } else if(matrix[x][y] == 15) {//wall right
//        flood(matrix, floods, rowMax, x - 1, y, position + 1,update);
//        //flood(matrix, flood, rowMax, x - 1, y, position + 1);
//    } else if (matrix[x][y] == 0) {
//        flood(matrix, floods, rowMax, x - 1, y, position + 1,update);
//        flood(matrix, floods, rowMax, x + 1, y, position + 1,update);
//        flood(matrix, floods, rowMax, x, y - 1, position + 1,update);
//        flood(matrix, floods, rowMax, x, y + 1, position + 1,update);
//    }
//}

//void renewFloodMap() {
//    for (int a = 0; a < MAX_x; a++) {
//        for (int b = 0; b < MAX_y; b++){
//            change[a][b] = false;
//            floods[a][b] = 999;
//        }
//    }
//
//    int rowMax = MAX_x;
//    int begin_point1 = 2;
//    int begin_point2 = 3;
//
//    flood(matrix, floods, MAX_x, begin_point1, begin_point1, 0, true);
//    for (int a = 0; a < MAX_x; a++) {
//        for (int b = 0; b < MAX_y; b++){
//            change[a][b] = false;
//        }
//    }
//    flood(matrix, floods, MAX_x, begin_point2, begin_point2, 0, true);
//    for (int a = 0; a < MAX_x; a++) {
//        for (int b = 0; b < MAX_y; b++){
//            change[a][b] = false;
//        }
//    }
//    flood(matrix, floods, MAX_x, begin_point2, begin_point1, 0, true);
//    for (int a = 0; a < MAX_x; a++) {
//        for (int b = 0; b < MAX_y; b++){
//            change[a][b] = false;
//        }
//    }
//    flood(matrix, floods, MAX_x, begin_point1, begin_point2, 0, true);
//
////................................................................................
//}

void renewFloodMap(){
    bool change[MAX_y+2][MAX_x+2];
    bool change1[MAX_y+2][MAX_x+2];
    for (int a = 0; a < MAX_x+2; a++) {
        for (int b = 0; b < MAX_y+2; b++){
            change1[a][b] = false;
            change[a][b] = false;
            floods[a][b] = 999;
        }
    }

    change[MAX_x/2 + 1][MAX_x/2] = true;
    change[MAX_x/2 + 1][MAX_x/2 + 1] = true;
    change[MAX_x/2][MAX_x/2 + 1] = true;
    change[MAX_x/2][MAX_x/2] = true;

    int counter = 0;
    int layer = 0;

    floods[MAX_x/2][MAX_x/2] = layer;
    floods[MAX_x/2][MAX_x/2 - 1] = layer;
    floods[MAX_x/2 - 1][MAX_x/2] = layer;
    floods[MAX_x/2 - 1][MAX_x/2 - 1] = layer;

    while (counter <= MAX_y*MAX_y - 1) {
        for (int a = 0; a < MAX_x+2; a++) {
            for (int b = 0; b < MAX_y+2; b++){
                change1[a][b] = change[a][b];
            }
        }
        counter = 0;
        layer++;
        for (int a = 0; a < MAX_x; a++) {
            for (int b = 0; b < MAX_y; b++){
                if(change1[a+1][b+1] == false){

                    if(matrix[a][b] == 1) {
                        if(change1[a + 1][b]){
                            change[a+1][b+1] = true;
                            floods[a][b] = layer;
                        }
                    }

                    if(matrix[a][b] == 2) {
                        if(change1[a + 1][b] || change1[a][b+1]){
                            change[a+1][b+1] = true;
                            floods[a][b] = layer;
                        }
                    }

                    if(matrix[a][b] == 3) {
                        if(change1[a+1][b] || change1[a + 2][b+1]){
                            change[a+1][b+1] = true;
                            floods[a][b] = layer;
                        }
                    }

                    if(matrix[a][b] == 4) {
                        if(change1[a+1][b] || change1[a+1][b + 2]){
                            change[a+1][b+1] = true;
                            floods[a][b] = layer;
                        }
                    }

                    if(matrix[a][b] == 5) {
                        if(change1[a+1][b] || change1[a][b+1] || change1[a + 2][b+1]){
                            change[a+1][b+1] = true;
                            floods[a][b] = layer;
                        }
                    }

                    if(matrix[a][b] == 6) {
                        if(change1[a+1][b] || change1[a][b+1] || change1[a+1][b + 2]){
                            change[a+1][b+1] = true;
                            floods[a][b] = layer;
                        }
                    }

                    if(matrix[a][b] == 7) {
                        if(change1[a+1][b] || change1[a + 2][b+1] || change1[a+1][b + 2]){
                            change[a+1][b+1] = true;
                            floods[a][b] = layer;
                        }
                    }

                    if(matrix[a][b] == 8) {
                        if(change1[a+1][b  +2] || change1[a][b+1] || change1[a + 2][b+1]){
                            change[a+1][b+1] = true;
                            floods[a][b] = layer;
                        }
                    }

                    if(matrix[a][b] == 9) {
                        if(change1[a][b+1]){
                            change[a+1][b+1] = true;
                            floods[a][b] = layer;
                        }
                    }

                    if(matrix[a][b] == 10) {
                        if(change1[a+1][b + 2]){
                            change[a+1][b+1] = true;
                            floods[a][b] = layer;
                        }
                    }

                    if(matrix[a][b] == 11) {
                        if(change1[a + 2][b+1]){
                            change[a+1][b+1] = true;
                            floods[a][b] = layer;
                        }
                    }

                    if(matrix[a][b] == 12) {
                        if(change1[a+1][b + 2] || change1[a][b+1]){
                            change[a+1][b+1] = true;
                            floods[a][b] = layer;
                        }
                    }

                    if(matrix[a][b] == 13) {
                        if(change1[a][b+1] || change1[a + 2][b+1]){
                            change[a+1][b+1] = true;
                            floods[a][b] = layer;
                        }
                    }

                    if(matrix[a][b] == 14) {
                        if(change1[a+1][b + 2] || change1[a + 2][b+1]){
                            change[a+1][b+1] = true;
                            floods[a][b] = layer;
                        }
                    }

//                    if(matrix[a][b] == 15) {
//                        if(change1[a][b+1]){
//                            change[a+1][b+1] = true;
//                            floods[a][b] = layer;
//                        }
//                    }

                    if(matrix[a][b] == 0 || matrix[a][b] == 15) {
                        if(change1[a][b+1] || change1[a + 2][b+1] || change1[a+1][b]  || change1[a+1][b + 2]){
                            change[a+1][b+1] = true;
                            floods[a][b] = layer;
                        }
                    }
                } else {
                    counter++;
                    change[a+1][b+1] = true;
                }
            }
        }
    }
}

//void renewFloodMap(){
//    bool change[MAX_y][MAX_x];
//    bool change1[MAX_y][MAX_x];
//    for (int a = 0; a < MAX_x; a++) {
//        for (int b = 0; b < MAX_y; b++){
//            change1[a][b] = false;
//            change[a][b] = false;
//            floods[a][b] = 999;
//        }
//    }
//
//    change[MAX_x/2 - 1][MAX_x/2] = true;
//    change[MAX_x/2 - 1][MAX_x/2 - 1] = true;
//    change[MAX_x/2][MAX_x/2 - 1] = true;
//    change[MAX_x/2][MAX_x/2] = true;
//
//    int counter = 0;
//    int layer = 0;
//
//    floods[MAX_x/2][MAX_x/2] = layer;
//    floods[MAX_x/2][MAX_x/2 - 1] = layer;
//    floods[MAX_x/2 - 1][MAX_x/2] = layer;
//    floods[MAX_x/2 - 1][MAX_x/2 - 1] = layer;
//
//    while (counter < MAX_x*MAX_y-1) {
//        for (int a = 0; a < MAX_x; a++) {
//            for (int b = 0; b < MAX_y; b++){
//                change1[a][b] = change[a][b];
//            }
//        }
//        counter = 0;
//        layer++;
//        for (int a = 0; a < MAX_x; a++) {
//            for (int b = 0; b < MAX_y; b++){
//                if(change1[a][b] == false){
//
//                    if(matrix[a][b] == 1) {
//                        if(change1[a][b - 1]){
//                            change[a][b] = true;
//                            floods[a][b] = layer;
//                        }
//                    }
//
//                    if(matrix[a][b] == 2) {
//                        if(change1[a][b - 1] || change1[a - 1][b]){
//                            change[a][b] = true;
//                            floods[a][b] = layer;
//                        }
//                    }
//
//                    if(matrix[a][b] == 3) {
//                        if(change1[a][b - 1] || change1[a + 1][b]){
//                            change[a][b] = true;
//                            floods[a][b] = layer;
//                        }
//                    }
//
//                    if(matrix[a][b] == 4) {
//                        if(change1[a][b - 1] || change1[a][b + 1]){
//                            change[a][b] = true;
//                            floods[a][b] = layer;
//                        }
//                    }
//
//                    if(matrix[a][b] == 5) {
//                        if(change1[a][b - 1] || change1[a - 1][b] || change1[a + 1][b]){
//                            change[a][b] = true;
//                            floods[a][b] = layer;
//                        }
//                    }
//
//                    if(matrix[a][b] == 6) {
//                        if(change1[a][b - 1] || change1[a - 1][b] || change1[a][b + 1]){
//                            change[a][b] = true;
//                            floods[a][b] = layer;
//                        }
//                    }
//
//                    if(matrix[a][b] == 7) {
//                        if(change1[a][b - 1] || change1[a + 1][b] || change1[a][b + 1]){
//                            change[a][b] = true;
//                            floods[a][b] = layer;
//                        }
//                    }
//
//                    if(matrix[a][b] == 8) {
//                        if(change1[a][b  +1] || change1[a - 1][b] || change1[a + 1][b]){
//                            change[a][b] = true;
//                            floods[a][b] = layer;
//                        }
//                    }
//
//                    if(matrix[a][b] == 9) {
//                        if(change1[a - 1][b]){
//                            change[a][b] = true;
//                            floods[a][b] = layer;
//                        }
//                    }
//
//                    if(matrix[a][b] == 10) {
//                        if(change1[a][b + 1]){
//                            change[a][b] = true;
//                            floods[a][b] = layer;
//                        }
//                    }
//
//                    if(matrix[a][b] == 11) {
//                        if(change1[a + 1][b]){
//                            change[a][b] = true;
//                            floods[a][b] = layer;
//                        }
//                    }
//
//                    if(matrix[a][b] == 12) {
//                        if(change1[a][b + 1] || change1[a - 1][b]){
//                            change[a][b] = true;
//                            floods[a][b] = layer;
//                        }
//                    }
//
//                    if(matrix[a][b] == 13) {
//                        if(change1[a - 1][b] || change1[a + 1][b]){
//                            change[a][b] = true;
//                            floods[a][b] = layer;
//                        }
//                    }
//
//                    if(matrix[a][b] == 14) {
//                        if(change1[a][b + 1] || change1[a + 1][b]){
//                            change[a][b] = true;
//                            floods[a][b] = layer;
//                        }
//                    }
//
//                    if(matrix[a][b] == 15) {
//                        if(change1[a - 1][b]){
//                            change[a][b] = true;
//                            floods[a][b] = layer;
//                        }
//                    }
//
//                    if(matrix[a][b] == 0) {
//                        if(change1[a - 1][b] || change1[a + 1][b] || change1[a][b - 1]  || change1[a][b + 1]){
//                            change[a][b] = true;
//                            floods[a][b] = layer;
//                        }
//                    }
//                } else {
//                    counter++;
//                    change[a][b] = true;
//                }
//            }
//        }
//    }
//}


int find_Min_index(int four_cell[]){
    int index_min;
    int minimum;
    for(int i =0;i<4;i++)
    {
        if(four_cell[i] < minimum)
        {
            minimum = four_cell[i];
            index_min = i;
        }
    }
    return index_min;
}
int find_Max_index(int four_cell[]){
    int index_max;
    int max;
    for(int i = 0;i<4;i++)
    {
        if(four_cell[i] > max)
        {
            max = four_cell[i];
            index_max = i;
        }
    }
    return index_max;
}

bool checkEqual(int four_cell[])
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
void moveToGoal(int &x, int &y, int &direction, int floods[MAX_x][MAX_y],bool left, bool right, bool front)
{
    pair <int,int> pairs;

    if(direction == 0) {
        int index_min;
        int four_cell[4];
        for(int i =0;i<4;i++){
            four_cell[i] = 999;
        }
        if(y+1 < MAX_y && !front){
            four_cell[0] = floods[x][y+1];
        }

        if(x+1 < MAX_x && !right){
            four_cell[1] = floods[x+1][y];
        }
        if((y-1 >=0 && left && right && front)||(y-1 >=0 && !left && !right && !front)){
            four_cell[2] = floods[x][y-1];
        }
        if(x-1 >=0 && !left){
            four_cell[3] = floods[x-1][y];
        }

        //////////////////////////////////////
        index_min = find_Min_index(four_cell);

        switch (index_min)
        {
            case 0:
                API::moveForward();
                y++;
                direction = 0;
                break;
            case 1:
                API::turnRight();
                API::moveForward();
                x++;
                direction = 1;
                break;
            case 2:
                API::turnRight();
                API::turnRight();
                API::moveForward();
                y--;
                direction = 2;
                break;
            case 3:
                API::turnLeft();
                API::moveForward();
                x--;
                direction = 3;
                break;
        }

    }
    else if(direction == 1){
        int index_min;
        int four_cell[4];
        for(int i =0;i<4;i++){
            four_cell[i] = 999;
        }
        if(y+1 < MAX_y && !left){
            four_cell[0] = floods[x][y+1];
        }

        if(x+1 < MAX_x && !front){
            four_cell[1] = floods[x+1][y];
        }
        if(y-1 >=0 && !right){
            four_cell[2] = floods[x][y-1];
        }
        if((x-1 >=0 && left && right && front)||(x-1 >=0 && !left && !right && front)){
            four_cell[3] = floods[x-1][y];
        }

        index_min = find_Min_index(four_cell);
        switch (index_min)
        {
            case 0:
                API::turnLeft();
                API::moveForward();
                y++;
                direction = 0;
                break;
            case 1:
                //API::turnRight();
                API::moveForward();
                x++;
                break;
            case 2:
                API::turnRight();
                //API::turnRight();
                API::moveForward();
                y--;
                direction = 2;
                break;
            case 3:
                API::turnLeft();
                API::turnLeft();
                API::moveForward();
                x--;
                direction = 3;
                break;
        }

    }
    else if(direction == 2){
        int index_min;
        int four_cell[4];
        for(int i =0;i<4;i++){
            four_cell[i] = 999;
        }
        if((y+1 < MAX_y && front && left && right)||(y+1 < MAX_y && !front && !left && ! right)){
            four_cell[0] = floods[x][y+1];
        }

        if(x+1 < MAX_x && !left){
            four_cell[1] = floods[x+1][y];
        }
        if(y-1 >=0 && !front){
            four_cell[2] = floods[x][y-1];
        }
        if(x-1 >=0 && !right){
            four_cell[3] = floods[x-1][y];
        }
        //////////////////////////////////////
        index_min = find_Min_index(four_cell);
        switch (index_min)
        {
            case 0:
                API::turnLeft();
                API::turnLeft();
                API::moveForward();
                y++;
                direction = 0;
                break;
            case 1:
                API::turnLeft();
                API::moveForward();
                x++;
                direction = 1;
                break;
            case 2:
                API::moveForward();
                y--;
                direction = 2;
                break;
            case 3:
                API::turnRight();
                API::moveForward();
                x--;
                direction = 3;
                break;
        }
    }
    else if(direction == 3){
        int index_min;
        int four_cell[4];
        for(int i =0;i<4;i++){
            four_cell[i] = 999;
        }
        if(y+1 < MAX_y && !right){
            four_cell[0] = floods[x][y+1];
        }

        if((x+1 < MAX_x && left && right && front)||(x+1 < MAX_x && !left && !right && !front)){
            four_cell[1] = floods[x+1][y];
        }
        if(y-1 >=0 && !left){
            four_cell[2] = floods[x][y-1];
        }
        if(x-1 >=0 && !front){
            four_cell[3] = floods[x-1][y];
        }

        index_min = find_Min_index(four_cell);
        switch (index_min)
        {
            case 0:
                API::turnRight();
                API::moveForward();
                y++;
                direction = 0;
                break;
            case 1:
                API::turnRight();
                API::turnRight();
                API::moveForward();
                x++;
                direction = 1;
                break;
            case 2:
                //API::turnRight();
                API::turnLeft();
                API::moveForward();
                y--;
                direction = 2;
                break;
            case 3:
                //API::turnLeft();
                API::moveForward();
                x--;
                direction = 3;
                break;
        }
    }

}




void moveToBegin(int &x, int &y, int &direction, stack<pair<int,int>> node)
{
    pair<int,int> pairs;
    pairs = node.top();
    int xx = pairs.first;
    int yy = pairs.second;

    switch(direction)
    {
        case 0:
            if(xx-x == 1 && yy - y == 0)
            {
                API::turnRight();
                API::moveForward();
                x++;
                direction = 1;
            }
            else if(xx - x == -1 && yy - y == 0)
            {
                API::turnLeft();
                API::moveForward();
                x--;
                direction = 3;
            }
            else if(xx - x == 0 && yy - y == 1)
            {
                API::moveForward();
                y++;
                direction = 0;
            }
            else if(xx - x == 0 && yy - y == -1)
            {
                API::turnRight();
                API::turnRight();
                API::moveForward();
                y--;
                direction = 2;
            }
            break;
        case 1:
            if(xx-x == 1 && yy - y == 0)
            {
                //API::turnRight();
                API::moveForward();
                x++;
                direction = 1;
            }
            else if(xx - x == -1 && yy - y == 0)
            {
                API::turnLeft();
                API::turnLeft();
                API::moveForward();
                x--;
                direction = 3;
            }
            else if(xx - x == 0 && yy - y == 1)
            {
                API::turnLeft();
                API::moveForward();
                y++;
                direction = 0;
            }
            else if(xx - x == 0 && yy - y == -1)
            {
                API::turnRight();
                API::moveForward();
                y--;
                direction = 2;
            }
            break;
        case 2:
            if(xx-x == 1 && yy - y == 0)
            {
                API::turnLeft();
                API::moveForward();
                x++;
                direction = 1;
            }
            else if(xx - x == -1 && yy - y == 0)
            {
                API::turnRight();
                API::moveForward();
                x--;
                direction = 3;
            }
            else if(xx - x == 0 && yy - y == 1)
            {
                API::turnLeft();
                API::turnLeft();
                API::moveForward();
                y++;
                direction = 0;
            }
            else if(xx - x == 0 && yy - y == -1)
            {
                API::moveForward();
                y--;
                direction = 2;
            }
            break;
        case 3:
            if(xx-x == 1 && yy - y == 0)
            {
                API::turnRight();
                API::turnRight();
                API::moveForward();
                x++;
                direction = 1;
            }
            else if(xx - x == -1 && yy - y == 0)
            {
                //API::turnLeft();
                API::moveForward();
                x--;
                direction = 3;
            }
            else if(xx - x == 0 && yy - y == 1)
            {
                API::turnRight();
                API::moveForward();
                y++;
                direction = 0;
            }
            else if(xx - x == 0 && yy - y == -1)
            {
                API::turnLeft();
                API::moveForward();
                y--;
                direction = 2;
            }
            break;

    }



}

int main(int argc, char* argv[]) {

    stack<pair<int,int>> node;
    bool finish = false;
    int c = 0;
    int y = 0;
    int x = 0;
    int direction = 0;
    int begin_flood_num = 0;

    for (int a = 0 ; a < 6; a++){
        for (int b = 0; b < 6; b++){
            matrix[a][b] = 0;
        }
    }
    log("Running...");
    API::setColor(0, 0, 'G');


    bool beginToGoal = false;
    bool left,right,front = false;
    while (c < 500) {

        checkWall(x, y, direction, left, right, front);
        setMatrix(left, right, front, x, y, direction);
        renewFloodMap();
        for (int i = 0; i < MAX_x; i++) {
            for (int j = 0; j < MAX_y; j++) {
                int num = floods[i][j];
                string k = to_string(num);
                API::setText(i, j, k);
            }
        }
        log_num(x);
        log_num(y);
        log_num(direction);
        log("------------------");
        if(floods[x][y] == 0)
        {
            beginToGoal = true;
        }

        if(!beginToGoal) {
            pair <int,int> pairs;
            pairs.first = x;
            pairs.second = y;
            node.push(pairs);
            moveToGoal(x, y, direction, floods, left, right, front);
        }

        else if (beginToGoal == true && finish == false)
        {
            if(x == 0 && y == 0)
            {
                finish = true;
            }
            log("change");
            moveToBegin(x, y, direction,node);
            node.pop();
        }
        c++;
    }

    while(!node.empty())
    {
        pair <int,int> pairs;
        pairs = node.top();
        node.pop();
        log_num(pairs.first);
        log_num(pairs.second);
        log(">>>>>>>>>>>>>>>>>>>");
    }


}
