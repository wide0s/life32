#include <stdlib.h>
#include <stdio.h>
#include <string.h> /* memset */
#include <unistd.h> /* sleep */

#define BIT(n)  (1U << (n))

struct world {
    size_t round;
    unsigned int cells2[2][32];
};

static void life_init(struct world *w, unsigned population[32]) {
    int i;

    w->round = 0;

    for (i = 0; i < 32; ++i) {
        w->cells2[0][i] = population[i];
        w->cells2[1][i] = 0;
    }
}

static int get_state(struct world *w, unsigned frame, unsigned i, unsigned j) {
    return !!(w->cells2[frame][j] & BIT(i));
}

static int is_occupied(struct world *w, unsigned frame, unsigned i, unsigned j) {
    return get_state(w, frame, i, j);
}

static int get_neighbors(struct world *w, unsigned frame, unsigned i, unsigned j) {
    int n = 0;
    int x, y, f = frame;

    x = i, y = j;
    n += get_state(w, f, (x + 1)%32, y) + get_state(w, f, 31 - (32 - x)%32, y);

    y = 31 - (32 - j)%32;
    n += get_state(w, f, (x + 1)%32, y) + get_state(w, f, x, y) + get_state(w, f, 31 - (32 - x)%32, y);

    y = (j + 1)%32;
    n += get_state(w, f, (x + 1)%32, y) + get_state(w, f, x, y) + get_state(w, f, 31 - (32 - x)%32, y);

    return n;
}

static void life_seed(struct world *w, unsigned frame, unsigned i, unsigned j) {
    w->cells2[frame][j] |= BIT(i);
}

static void life_kill(struct world *w, unsigned frame, unsigned i, unsigned j) {
    w->cells2[frame][j] &= ~BIT(i);
}

static unsigned get_father(struct world *w) {
    return w->round % 2;
}

static unsigned get_child(struct world *w) {
    return get_father(w) ^ 1;
}

static void life_update(struct world *w) {
    unsigned i, j;
    unsigned father, child, neighbors, occupied;

    father = get_father(w);
    child = get_child(w);

    for(j = 0; j < 32; j++) {
        for(i = 0; i < 32; i++) {
            occupied = is_occupied(w, father, i, j);
            neighbors = get_neighbors(w, father, i, j);

            if (neighbors < 2 || neighbors > 3) {
                // RULE1: Any live cell with fewer than two live
                //  neighbours dies, as if caused by underpopulation
                // RULE2: Any live cell with more than three live
                //  neighbours dies, as if by overpopulation
                life_kill(w, child, i, j);
            } else if ((neighbors == 2 || neighbors == 3) && occupied) {
                // RULE3: Any live cell with two or three live
                //  neighbours lives on the next generation.
                life_seed(w, child, i, j);
            } else if (neighbors == 3 && !occupied) {
                // RULE4: Any dead cell with exactly three live
                //  neighbours becomes a live cell, as if by reproduction
                life_seed(w, child, i, j);
            } else {
                // Reset cell
                life_kill(w, child, i, j);
            }
        }
    }

    // update round
    w->round++;
}

static void life_print(struct world *w) {
    unsigned j, f;

#define ANSI_COLOR_GREEN  "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_RESET  "\x1b[0m"

#define P(pw, f, i, j) get_state(pw, f, i, j) ? (char)210 : '.'

    f = get_father(w);

    printf(ANSI_COLOR_YELLOW "I=%lu" ANSI_COLOR_RESET "\n", w->round);
    for (j=0; j < 32; ++j) {
        printf(ANSI_COLOR_GREEN "%c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c %c" ANSI_COLOR_RESET "\n",
            P(w, f, 31, j), P(w, f, 30, j), P(w, f, 29, j), P(w, f, 28, j),
            P(w, f, 27, j), P(w, f, 26, j), P(w, f, 25, j), P(w, f, 24, j),
            P(w, f, 23, j), P(w, f, 22, j), P(w, f, 21, j), P(w, f, 20, j),
            P(w, f, 19, j), P(w, f, 18, j), P(w, f, 17, j), P(w, f, 16, j),
            P(w, f, 15, j), P(w, f, 14, j), P(w, f, 13, j), P(w, f, 12, j),
            P(w, f, 11, j), P(w, f, 10, j), P(w, f,  9, j), P(w, f,  8, j),
            P(w, f,  7, j), P(w, f,  6, j), P(w, f,  5, j), P(w, f,  4, j),
            P(w, f,  3, j), P(w, f,  2, j), P(w, f,  1, j), P(w, f,  0, j));
    }
#undef P
}

int main(int argc, char **argv) {
    struct world w;
    unsigned population[32];

    /* intial seed */
    memset(population, 0, sizeof(population));
    /* glider */
    ///*
//    population[10] = BIT(30);
//    population[11] = BIT(29);
//    population[12] = BIT(31) | BIT(30) | BIT(29);

//    population[0]  = BIT(1);
//    population[1]  = BIT(2);
//    population[2] = BIT(2) | BIT(1) | BIT(0);

//    population[10] |= BIT(3);
//    population[11] |= BIT(4);
//    population[12] |= BIT(4) | BIT(3) | BIT(2);

    //*/

    /*
        ++++
         +++
         +++
         +++
    */
    /*
    population[17] = BIT(20);
    population[18] = BIT(19) | BIT(18) | BIT(17);
    population[19] = BIT(19) | BIT(18) | BIT(17);
    population[20] = BIT(19) | BIT(18) | BIT(17);
    */

    /*
       ++++++ ++
       ++++++ ++
              ++
       ++     ++
       ++     ++
       ++     ++
       ++
       ++ ++++++
       ++ ++++++
    */
    /*
    population[10] = BIT(20) | BIT(19) | BIT(18) | BIT(17) | BIT(16) | BIT(15) | BIT(13) | BIT(12);
    population[11] = BIT(20) | BIT(19) | BIT(18) | BIT(17) | BIT(16) | BIT(15) | BIT(13) | BIT(12);
    population[12] = BIT(13) | BIT(12);
    population[13] = BIT(20) | BIT(19) | BIT(13) | BIT(12);
    population[14] = BIT(20) | BIT(19) | BIT(13) | BIT(12);
    population[15] = BIT(20) | BIT(19) | BIT(13) | BIT(12);
    population[16] = BIT(20) | BIT(19) | BIT(17) | BIT(16) | BIT(15) | BIT(14) | BIT(13) | BIT(12);
    population[17] = BIT(20) | BIT(19) | BIT(17) | BIT(16) | BIT(15) | BIT(14) | BIT(13) | BIT(12);
    */

    /*
        +++ +
        +    
           ++
         ++ +
        + + +
    */
    /*
    population[10] = BIT(20) | BIT(19) | BIT(18) | BIT(16);
    population[11] = BIT(20);
    population[12] = BIT(17) | BIT(16);
    population[13] = BIT(19) | BIT(18) | BIT(16);
    population[14] = BIT(20) | BIT(18) | BIT(16);
    */

    /*
        +      
          +    
       ++   +++
    */
    /*
    population[10] = BIT(20);
    population[11] = BIT(18);
    population[12] = BIT(21) | BIT(20) | BIT(17) | BIT(16) | BIT(15);
    */

    /*         +  +   
         +  +   +     
          +    + +    
         + +          
                      
                      
                      
                      
                      
                      
                      
                     +
                    ++
                     +
    */
    /*
    population[9] = BIT(17) | BIT(14);
    population[10] = BIT(23) | BIT(20) | BIT(16);
    population[11] = BIT(22) | BIT(17) | BIT(15);
    population[12] = BIT(23) | BIT(21);
    population[20] = BIT(10);
    population[21] = BIT(11) | BIT(10);
    population[22] = BIT(10);
    */

    /*
        ++++++++++

        Periodic life form with T=16
    */
    population[10] = BIT(20) | BIT(19) | BIT(18) | BIT(17) | BIT(16) | BIT(15) | BIT(14) | BIT(13) | BIT(12) | BIT(11);

    /*  Glider life form */
    population[29] = BIT(30);
    population[30] = BIT(29);
    population[31] = BIT(31) | BIT(30) | BIT(29);


    /* Online simulator: http://www.cuug.ab.ca/dewara/life/life.html */

    life_init(&w, population);

    for (;;) {
        life_print(&w);
        life_update(&w);
        /* sleep(1); */
        usleep(200 * 1000);
        printf("\n");
        printf("\033[2J");
    }
    return 0;
}
