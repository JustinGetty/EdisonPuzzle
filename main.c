#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------------------- Constants & Types ---------------------- */

#define SIZE 56               // Board width/height
#define ROW_MASK ((1ULL << SIZE) - 1ULL)

typedef struct {
    uint64_t row[SIZE];  // 56-wide board stored in 64 bits per row
    int space;           // how many free cells remain
} Board;

typedef struct {
    int w, h;
    int area;
} Piece;

static inline uint64_t mask_bits(int width, int shift) {
    if (width == 0) return 0ULL;
    uint64_t base = (1ULL << width) - 1ULL;  
    return base << shift;                    
}


static inline bool row_is_free(uint64_t row_val, int x, int pw) {
    
    uint64_t m = mask_bits(pw, x);
    return ((row_val & m) == 0ULL);
}


static inline void occupy_row(uint64_t *row_val, int x, int pw) {
    uint64_t m = mask_bits(pw, x);
    *row_val |= m;  
}


static inline void free_row(uint64_t *row_val, int x, int pw) {
    uint64_t m = mask_bits(pw, x);
    *row_val &= ~m;  
}

void init_board(Board *b) {
    for (int y = 0; y < SIZE; y++) {
        b->row[y] = 0ULL;  
    }
    b->space = SIZE * SIZE;
}

int find_next_free(const Board *b, int start_pos) {
    for (int pos = start_pos; pos < SIZE * SIZE; pos++) {
        int y = pos / SIZE;
        int x = pos % SIZE;
        // If bit is 0 => cell is free
        if (((b->row[y] >> x) & 1ULL) == 0ULL) {
            return pos;
        }
    }
    return -1; // no free cell found
}

/*
 * Check if a piece of size (pw x ph) fits at coordinate (x, y).
 * We already know from 'pos' => x, y.
 */
bool does_fit(const Board *b, int x, int y, int pw, int ph) {
    // boundaries
    if (x + pw > SIZE || y + ph > SIZE) {
        return false;
    }
    // check each row from y..y+ph-1
    for (int r = 0; r < ph; r++) {
        if (!row_is_free(b->row[y + r], x, pw)) {
            return false;
        }
    }
    return true;
}

/*
 * Occupy the cells for piece (pw x ph) at (x, y).
 *  - sets bits to 1 in each row
 */
void place_piece(Board *b, int x, int y, int pw, int ph) {
    for (int r = 0; r < ph; r++) {
        occupy_row(&b->row[y + r], x, pw);
    }
    b->space -= (pw * ph);
}

void remove_piece(Board *b, int x, int y, int pw, int ph) {
    for (int r = 0; r < ph; r++) {
        free_row(&b->row[y + r], x, pw);
    }
    b->space += (pw * ph);
}

int compare_pieces(const void *a, const void *b) {
    const Piece *pa = (const Piece *)a;
    const Piece *pb = (const Piece *)b;
    return pb->area - pa->area; // descending area
}

bool backtrack(Board *b, Piece *pieces, int n_pieces,
               int start_pos,
               int positions[][3], int *pos_count) {
    if (b->space == 0) {
        return true;
    }
  
    if (n_pieces == 0) {
        return false;
    }

    
    int next = find_next_free(b, start_pos);
    if (next < 0) {
        return (b->space == 0);
    }

    int y = next / SIZE;
    int x = next % SIZE;

    for (int i = 0; i < n_pieces; i++) {
        {
            int pw = pieces[i].w;
            int ph = pieces[i].h;
            if (does_fit(b, x, y, pw, ph)) {
                
                place_piece(b, x, y, pw, ph);

               
                positions[*pos_count][0] = pw;
                positions[*pos_count][1] = ph;
                positions[*pos_count][2] = y * SIZE + x;
                (*pos_count)++;

                
                Piece new_pieces[n_pieces - 1];
                int idx = 0;
                for (int j = 0; j < n_pieces; j++) {
                    if (j != i) {
                        new_pieces[idx++] = pieces[j];
                    }
                }

                
                if (backtrack(b, new_pieces, n_pieces - 1, next + 1, positions, pos_count)) {
                    return true;
                }

                
                (*pos_count)--;
                remove_piece(b, x, y, pw, ph);
            }
        }
        if (pieces[i].w != pieces[i].h) {
            int pw = pieces[i].h;
            int ph = pieces[i].w;
            if (does_fit(b, x, y, pw, ph)) {
                
                place_piece(b, x, y, pw, ph);

                positions[*pos_count][0] = pw;
                positions[*pos_count][1] = ph;
                positions[*pos_count][2] = y * SIZE + x;
                (*pos_count)++;

                Piece new_pieces[n_pieces - 1];
                int idx = 0;
                for (int j = 0; j < n_pieces; j++) {
                    if (j != i) {
                        new_pieces[idx++] = pieces[j];
                    }
                }

                if (backtrack(b, new_pieces, n_pieces - 1, next + 1, positions, pos_count)) {
                    return true;
                }

                (*pos_count)--;
                remove_piece(b, x, y, pw, ph);
            }
        }
    }

    return false;
}

int main() {
    // Prepare board
    Board board;
    init_board(&board);

    // Original pieces
    // (28,14), (21,18), (21,18), (21,14), (21,14), (32,11),
    // (32,10), (28,7), (28,6), (17,14), (14,4), (10,7)
    Piece pieces[] = {
        {28,14, 28*14}, {21,18,21*18}, {21,18,21*18},
        {21,14,21*14},  {21,14,21*14}, {32,11,32*11},
        {32,10,32*10},  {28,7,28*7},   {28,6,28*6},
        {17,14,17*14},  {14,4,14*4},   {10,7,10*7}
    };
    int n_pieces = sizeof(pieces)/sizeof(pieces[0]);

    qsort(pieces, n_pieces, sizeof(Piece), compare_pieces);

    int positions[12][3]; 
    int pos_count = 0;

    if (backtrack(&board, pieces, n_pieces, 0, positions, &pos_count)) {
        printf("Solution:\n");
        for (int i = 0; i < pos_count; i++) {
            int pw = positions[i][0];
            int ph = positions[i][1];
            int coord = positions[i][2];
            int y = coord / SIZE;
            int x = coord % SIZE;
            printf("Piece (%2d, %2d) at (%2d, %2d)\n", pw, ph, x, y);
        }
    } else {
        printf("No solution found.\n");
    }

    return 0;
}
