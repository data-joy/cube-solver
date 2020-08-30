// All rights reserved.
// Wilson So <103joy@gmail.com>

#include <stdio.h>
#include <stdbool.h>
#include <memory.h>
#include <stdlib.h>

typedef struct point {
  int x;
  int y;
  int z;
} point;

#define LEN (3*3*3)
typedef bool cube_input[LEN];

// solution[i] = {x, y, z} of position i of snake
typedef point cube_solution[LEN];

typedef bool filled[3][3][3];


void init_filled(filled cube) {
  memset(cube, 0, sizeof(bool)*3*3*3);
}

void print_filled(filled cube, int verbose) {
  int x, y, z;
  if (verbose == 0) {
    return;
  }

  for (x=0; x<3; x++) {
    for (y=0; y<3; y++) {
      for (z=0; z<3; z++) {
        printf("(%d,%d,%d) = %c ", x, y, z, (cube[x][y][z]? 'x' : ' '));
      }
      printf("\n");
    }
  }
}
// print_partial_solution prints solution up to position pos - 1
void print_partial_solution(cube_solution solution, int pos, filled cube, int verbose) {
  int i;
  if (verbose == 0) {
    return;
  }

  printf("--- POSITIONS ---\n");
  for (i=0; i<LEN && i<pos; i++) {
    point pt = solution[i];
    printf("(%u,%u,%u), ", pt.x, pt.y, pt.z);
    if ((i+1)%3 == 0) {
      printf("\n");
    }
  }
  printf("\n");

  print_filled(cube, verbose);
}

// print input
void print_input(cube_input input, int verbose) {
  int i;
  if (verbose == 0) {
    return;
  }

  printf("--- INPUT ---\n");
  for (i=0; i<LEN; i++) {
    printf("%d,", input[i]);
    if ((i+1)%9 == 0) {
      printf("\n");
    }
  }
  printf("\n");

}
// print_solution prints the input and output.
void print_solution(cube_input input, cube_solution solution, filled cube, int verbose) {
  if (verbose == 0) {
    return;
  }

  print_input(input, verbose);
  print_partial_solution(solution, LEN, cube, verbose);
}

bool valid_pos(int x, int y, int z) {
  return (x>=0 && x<=2 && y>=0 && y<=2 && z>=0 && z<=2);
}

// solve solves puzzle given input and direction dir, assuming positions 0...pos-1
// have been solved, and the partial soution is already in solution.
// pos >= 1
bool solve(cube_input input, point dir, int pos, cube_solution solution, filled cube, int verbose) {

  bool solvable;

  if (verbose != 0) {
    printf("Solve called with pos %d.   Dir = (%d, %d, %d)\n", pos, dir.x, dir.y, dir.z);
    print_partial_solution(solution, pos, cube, verbose);
  }

  if (pos >= LEN) {
    // This has reached the end.
    print_solution(input, solution, cube, verbose);
    return true;
  }

  // if this position is fixed (i.e., next block continues the direction),
  // the solution simply extends the current direciton by 1 block.
  // Check if this is feasible.  If so, continue, else return false.

  // if this position is movable, iterate over 4 directions.
  // If feasilble, continue, else return false.

  if ( !input[pos] ) {
    // position is fixed
    // extend current direction by 1
    int new_x = solution[pos-1].x + dir.x;
    int new_y = solution[pos-1].y + dir.y;
    int new_z = solution[pos-1].z + dir.z;
    // check if feaislble
    if (valid_pos(new_x, new_y, new_z) && !cube[new_x][new_y][new_z]) {
      solution[pos].x = new_x;
      solution[pos].y = new_y;
      solution[pos].z = new_z;
      cube[new_x][new_y][new_z] = true;
      solvable = solve(input, dir, pos + 1, solution, cube, verbose);
      if (!solvable) {
        cube[new_x][new_y][new_z] = false;
      }
      return solvable;
    } else {
      return false;
    }
  } else {
    // position is movable
    point new_dirs[4];
    int i;
    if (dir.x != 0) {
      new_dirs[0] = (point){0, 1, 0};
      new_dirs[1] = (point){0, -1, 0};
      new_dirs[2] = (point){0, 0, 1};
      new_dirs[3] = (point){0, 0, -1};
    } else if (dir.y != 0) {
      // {1, 0, 0};
      // {-1, 0, 0};, {0, 0, 1}, {0, 0, -1}];
      new_dirs[0] = (point){1, 0, 0};
      new_dirs[1] = (point){-1, 0, 0};
      new_dirs[2] = (point){0, 0, 1};
      new_dirs[3] = (point){0, 0, -1};
    } else {
      // new_dirs = [{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}];
      new_dirs[0] = (point){1, 0, 0};
      new_dirs[1] = (point){-1, 0, 0};
      new_dirs[2] = (point){0, 1, 0};
      new_dirs[3] = (point){0, -1, 0};
    }
    // interate over 4 directions
    for (i=0; i<4; i++) {
      // check each direction
      // if feasible, recurse, otherwise return false.
      int new_x = solution[pos-1].x + new_dirs[i].x;
      int new_y = solution[pos-1].y + new_dirs[i].y;
      int new_z = solution[pos-1].z + new_dirs[i].z;
      // check if feaislble
      if (valid_pos(new_x, new_y, new_z)  && !cube[new_x][new_y][new_z]) {
        solution[pos].x = new_x;
        solution[pos].y = new_y;
        solution[pos].z = new_z;

        cube[new_x][new_y][new_z] = true;
        solvable = solve(input, new_dirs[i], pos + 1, solution, cube, verbose);
        if (solvable) {
          return true;
        } else {
          cube[new_x][new_y][new_z] = false;
        }
      }
    }
  }
  return false;
}

int main(int argc, char **argv) {

  // bool input[LEN] =
  // { 0, 1, 1, 1, 1, 1, 1, 1, 1,
  //   1, 1, 1, 1, 1, 1, 1, 1, 1,
  //   1, 1, 1, 1, 1, 1, 1, 1, 1};
  bool input[LEN] =
  { 0,
    0, 0, 1,
    1, 1, 0,
    1, 1, 0,
    1, 1, 1,
    0, 1, 0,
    1, 1, 1,
    1, 0, 1,
    0, 1, 0,
    1, 0};

  point init_dir = {1, 0, 0};
  point solution[LEN];
  filled cube;
  int new_x, new_y, new_z;
  int pos = 0;

  init_filled(cube);
  // print_filled(cube);

  // WLOG, first block is at (0,0,0), dir = (1,0,0)
  new_x = 0;
  new_y = 0;
  new_z = 0;
  solution[0].x = new_x;
  solution[0].y = new_y;
  solution[0].z = new_z;
  cube[new_x][new_y][new_z] = true;
  pos ++;
  // print_filled(cube);

  // printf("Hello world!\n");
  // print_partial_solution(solution, pos, cube);


  if (solve(input, init_dir, 1, solution, cube, 1 /* verbose */)) {
    printf("Solved!\n");
    print_solution(input, solution, cube, 1);
  } else {
    cube[new_x][new_y][new_z] = false;
    printf("No solution!\n");
  }
  return 0;

}
