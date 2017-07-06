#include "led.h"
#include <cstdlib>
#include <cmath>

#include "bitmap.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <cstdlib>
#include <vector>
#include <algorithm>

void welcome(makerfaire::fxat::Led & led)
{
  const int IM_H = 16;
  using makerfaire::fxat::Led;
  struct letter_t {
    int w, h;
    std::string pix;
    int at(int x, int y) const{
      int pos = x + y*w;
      return pix.at(pos) - 'A';
    }
  };
  std::vector<letter_t> letters{
#include "welcome.hxx"
  };
  letter_t heart
#include "heart.hxx"
  ;
  const int MARGIN = 4;
  int sum = Led::Width;
  for (auto const & e : letters){
    sum += e.w - MARGIN;
  }
  for (double t=0;;){
    for (int x0 = letters.front().w; -sum < x0; --x0){
      t += 0.3;
      led.Clear();
      {
        for (int x_ = 0; x_ < heart.w; ++x_){
          for (int y = 0; y < heart.h; ++y){
            double dx = x_ - heart.w*0.5;
            int x = std::floor(std::cos(t) * dx + 8);
            int z = std::floor(std::sin(t) * dx / 2 + 4);
            led.SetLed(x, y, z, heart.at(x_, y) * 0x110008);
          }
        }
      }
      int left = x0;
      for (auto const & e : letters){
        int right = left + e.w;
        int center = (left + right) / 2;
        int z = std::min<int>(Led::Depth - 1, std::max<int>(std::abs(center - Led::Width / 2) / 2, 0));
        auto color = [&](int col)->int {
          double t0 = 3.1416 * 2 / 3;
          double t = 3.1416 * 3 * left / 32;
          auto c = [t0, col](double x)->int {
            double v = (std::sin(x) + 0.5) / 1.5;
            return v < 0 || col == 0 ? 0 : std::min<int>(255, v * 127 * col / 15 + 128);
          };
          return c(t) + c(t + t0) * 256 + c(t + t0 * 2) * 65536;
        };

        if (left < Led::Width && 0 < right){
          for (int x = left; x < right; ++x){
            for (int y = 0; y < IM_H; ++y){
              if (0 <= x && x < Led::Width){
                int col = e.at(x - left, y);
                if (0 <= z && z < Led::Depth){
                  led.SetLed(x, y + 12, z, color(col));
                }
              }
            }
          }
        }
        left = right - MARGIN;
      }
      led.Show();
      makerfaire::fxat::Wait(50);
    }
  }
}

int main()
{
  using namespace makerfaire::fxat;
  Led led;
  welcome(led);
}
