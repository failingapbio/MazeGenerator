#include <iostream>
#include <limits>
#include <random>
#include <thread>
#include <cmath>
using namespace std;

constexpr int base = static_cast<int>(2);
constexpr int cell_width = static_cast<int>(5);
constexpr int cell_height = static_cast<int>(2);
void tessellate_and_print_maze(const int total_iterations) {
    //storing important variables
    const int x_dimension = static_cast<int>(cell_width * pow(base, total_iterations) + 1);
    const int y_dimension = static_cast<int>(cell_height * pow(base, total_iterations) + 1);

    bool maze[x_dimension][y_dimension];
    //make the unit square first
    for(int y = 0; y < 3; y++) {
        for(int x = 0; x < 6; x++) {
            if(y % cell_height == 1) {
                if ((x+1) % cell_width == 1) {
                    maze[x][y] = true;
                }
                else {
                    maze[x][y] = false;
                }
            }
            else {
                maze[x][y] = true;
            }
        }
    }
    //Declare variables needed for expand/puncture
    int previous_length=cell_width, previous_height=cell_height;
    int current_length=10, current_height=4;
    int x,y;
    //Seed random numbers
    random_device rd;
    uniform_int_distribution<int> gen1(1,4);
    //Expand and puncture grid
    for(int current_iteration = 1; current_iteration <= total_iterations; current_iteration++) {
        //re-initialize puncture location per iteration
        uniform_int_distribution<int> gen2(0,current_iteration-1);
        //begin the expansion
        for(y = 0; y <= previous_height; y++) {
            for(x = previous_length+1; x <= current_length; x++) {
                maze[x][y] = maze[x-previous_length][y];
            }
        }
        for(y = previous_height+1; y <= current_height; y++) {
            for(x = 0; x <= current_length; x++) {
                maze[x][y] = maze[x][y-previous_height];
            }
        }
        //Which size to not puncture (3/4 sides need a puncture)
        int no_puncture_direction = gen1(rd);
        int puncture_location_1 = gen2(rd);
        int puncture_location_2 = gen2(rd);
        int puncture_location_3 = gen2(rd);
        int puncture_location_4 = gen2(rd);

        if(no_puncture_direction != 1) {
            for(int o = 0; o < cell_width-1; o++) {
                maze[1+cell_width*puncture_location_1 + o][previous_height] = false;
            }
        }

        if(no_puncture_direction != cell_height) {
            for(int o = 0; o < cell_width-1; o++) {
                maze[(previous_length + 1 + cell_width * puncture_location_2 + o)][previous_height] = false;
            }
        }

        if(no_puncture_direction != 3) {
                maze[previous_length][1+cell_height*puncture_location_3] = false;
        }

        if(no_puncture_direction != 4) {
                maze[previous_length][previous_height+1+cell_height*puncture_location_4] = false;
        }
        previous_length=current_length, previous_height=current_height;
        current_length=current_length*cell_height, current_height=current_height*cell_height;
    }
    // final puncture at edges
    uniform_int_distribution<int> gen3(0, 1);
    uniform_int_distribution<int> gen2(0,pow(2,total_iterations));
    bool maze_vertical = gen3(rd);
    if (maze_vertical == true)
    {
        int puncture_location_3 = gen2(rd);
        int puncture_location_4 = gen2(rd);
        maze[0][1+cell_height*puncture_location_3] = false;
        maze[y_dimension][1+cell_height*puncture_location_4] = false;
    }
    else
    {
        int puncture_location_1 = gen2(rd);
        int puncture_location_2 = gen2(rd);
        for(int o = 0; o < cell_width-1; o++) {
            maze[1+cell_width*puncture_location_1 + o][0] = false;
            maze[1 + cell_width * puncture_location_2 + o][x_dimension] = false;
        }
    }
    // printing maze

    for(int y_print = 0; y_print < y_dimension; y_print++) {
        for(int x_print = 0; x_print < x_dimension; x_print++) {
            if (maze[x_print][y_print] == false) {
                cout << " ";
            }
            else {
                cout << "#";
            }
        }
        cout << endl;
    }
}
int main()
    {
        cout << "Hello!" << endl;
        this_thread::sleep_for(chrono::milliseconds(750));
        cout << "This program generates a maze randomly using tessellation." << endl;
        this_thread::sleep_for(chrono::milliseconds(1250));
        cout << "Please specify the size of the maze, with 1 being the smallest and 5 being the largest." << endl;
        this_thread::sleep_for(chrono::milliseconds(1250));
        cout << "Enjoy!" << endl;
        this_thread::sleep_for(chrono::milliseconds(1250));
        cout << "Size of maze:" << endl;
        int number_of_iterations;

        while (!(cin >> number_of_iterations)|| number_of_iterations < 1 || number_of_iterations > 5) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout << "Please enter a whole number between 1 and 5:" << endl;
        }
        const int size_in_units = static_cast<int>(pow(base, number_of_iterations));
        cout << "Height of maze:" << endl;
        cout << "Your maze will be " << size_in_units << " units long and " << size_in_units << " units wide" << endl <<endl;
        tessellate_and_print_maze(number_of_iterations);
        return 0;
    }