#include <cmath>

#include "SobelFilter.h"

SobelFilter::SobelFilter(sc_module_name n) : sc_module(n) {
  SC_THREAD(do_filter);
  sensitive << i_clk.pos();
  dont_initialize();
  reset_signal_is(i_rst, false);
}

// sobel mask
//const int mask[MASK_N][MASK_X][MASK_Y] = {{{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}},

//                                          {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}}};

// gausian mask
const int mask[MASK_N][MASK_X][MASK_Y] = {{{1, 4, 7, 4, 1}, 
                                          {4, 16, 26, 16, 4}, 
                                          {7, 26, 41, 26, 7},
                                          {4, 16, 26, 16, 4},
                                          {1, 4, 7, 4, 1}}};

void SobelFilter::do_filter() {
  unsigned char buffer[MASK_X][MASK_Y] = {{0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0},
                                          {0, 0, 0, 0, 0}};
  while (true) {
    for (unsigned int i = 0; i < MASK_N; ++i) {
      val[i] = 0;
    }
    for (unsigned int v = 0; v < MASK_Y; ++v) {
      for (unsigned int u = 0; u < MASK_X; ++u) {
        if(u == MASK_X-1){ 
          buffer[u][v] = (i_r.read() + i_g.read() + i_b.read()) / 3;
        }else{ //buffer left shift
          buffer[u][v] = buffer[u+1][v];
        }
        //unsigned char grey = (i_r.read() + i_g.read() + i_b.read()) / 3;
        for (unsigned int i = 0; i != MASK_N; ++i) {
          val[i] += buffer[u][v] * mask[i][u][v];
        }
      }
    }
    double total = 0;
    total += val[0];
    int result = (int)(total/273);
    o_result.write(result);
    wait(10); //emulate module delay
  }
}
