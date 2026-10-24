// Generalized Langton's ant (turmite with R/L rule string).
// usage: ant RULE [steps]
// prints: rule class onset period dx dy steps_run extent sym_checks sym_hits sym_kinds last_sym_step
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define W 8192
static unsigned char g[W * W];
static unsigned char *mv;

static int DX[4] = {0, 1, 0, -1}, DY[4] = {-1, 0, 1, 0};

// symmetry of bounding box contents; returns bitmask: 1 vmirror 2 hmirror 4 rot180 8 diag 16 antidiag
static int symcheck(int x0, int y0, int x1, int y1) {
    int w = x1 - x0 + 1, h = y1 - y0 + 1, m = 31;
    if (w != h) m &= ~(8 | 16);
    for (int y = 0; y < h && m; y++)
        for (int x = 0; x < w && m; x++) {
            unsigned char c = g[(y0 + y) * W + x0 + x];
            if ((m & 1) && c != g[(y0 + y) * W + x0 + (w - 1 - x)]) m &= ~1;
            if ((m & 2) && c != g[(y0 + h - 1 - y) * W + x0 + x]) m &= ~2;
            if ((m & 4) && c != g[(y0 + h - 1 - y) * W + x0 + (w - 1 - x)]) m &= ~4;
            if ((m & 8) && c != g[(y0 + x) * W + x0 + y]) m &= ~8;
            if ((m & 16) && c != g[(y0 + w - 1 - x) * W + x0 + (h - 1 - y)]) m &= ~16;
        }
    return m;
}

int main(int argc, char **argv) {
    const char *rule = argv[1];
    long N = argc > 2 ? atol(argv[2]) : 20000000;
    int n = strlen(rule);
    mv = malloc(N);
    int x = W / 2, y = W / 2, d = 0, ox = x, oy = y;
    int bx0 = x, by0 = y, bx1 = x, by1 = y;
    long t, nextcheck = 1000, checks = 0, hits = 0, lastsym = -1;
    int kinds = 0;
    for (t = 0; t < N; t++) {
        unsigned char *c = &g[y * W + x];
        d = (d + (rule[*c] == 'R' ? 1 : 3)) & 3;
        *c = (*c + 1) % n;
        x += DX[d]; y += DY[d];
        mv[t] = d;
        if (x < bx0) bx0 = x; if (x > bx1) bx1 = x;
        if (y < by0) by0 = y; if (y > by1) by1 = y;
        if (x <= 1 || y <= 1 || x >= W - 2 || y >= W - 2) { t++; break; }
        if (x == ox && y == oy && t >= nextcheck) {
            // box of painted cells (use visited box; unvisited cells are 0 anyway, so trim to nonzero)
            int a0 = bx1, a1 = bx0, b0 = by1, b1 = by0;
            for (int yy = by0; yy <= by1; yy++)
                for (int xx = bx0; xx <= bx1; xx++)
                    if (g[yy * W + xx]) {
                        if (xx < a0) a0 = xx; if (xx > a1) a1 = xx;
                        if (yy < b0) b0 = yy; if (yy > b1) b1 = yy;
                    }
            checks++;
            if (a0 <= a1) {
                int m = symcheck(a0, b0, a1, b1);
                if (m) { hits++; kinds |= m; lastsym = t; }
            }
            nextcheck = t + t / 40 + 500;
        }
    }
    long T = t;
    // highway detection on the move sequence tail
    long best = 0, onset = -1; int hdx = 0, hdy = 0;
    for (long p = 1; p <= 200000 && 3 * p < T; p++) {
        int ok = 1;
        long chk = 3 * p > 4000 ? 3 * p : 4000;
        if (chk > T - p) chk = T - p;
        for (long j = 1; j <= 2000 && ok; j++) if (mv[T - j] != mv[T - j - p]) ok = 0;
        if (!ok) continue;
        for (long j = 1; j <= chk && ok; j++) if (mv[T - j] != mv[T - j - p]) ok = 0;
        if (!ok) continue;
        int sx = 0, sy = 0;
        for (long j = T - p; j < T; j++) { sx += DX[mv[j]]; sy += DY[mv[j]]; }
        if (sx == 0 && sy == 0) { best = -p; break; } // bounded cycle
        best = p; hdx = sx; hdy = sy;
        long i = T - 1 - p;
        while (i >= 0 && mv[i] == mv[i + p]) i--;
        onset = i + 1;
        break;
    }
    const char *cls;
    if (best > 0) cls = "H";
    else if (best < 0) cls = "LOOP";
    else if (hits > 0) cls = "S";
    else cls = "C";
    int ext = (bx1 - bx0 > by1 - by0 ? bx1 - bx0 : by1 - by0) + 1;
    printf("%s %s %ld %ld %d %d %ld %d %ld %ld %d %ld\n", rule, cls, onset, best < 0 ? -best : best, hdx, hdy,
           T, ext, checks, hits, kinds, lastsym);
    return 0;
}
