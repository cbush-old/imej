#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <imlib2.h>
#include <cmath>
#include <cstdint>

using namespace std;

int get_terminal_width(){
  winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w.ws_col;
}

void put_go(){
  cout << "\033(0";
}

void put_gf(){
  cout << "\033(B";
}

void put_chars(Imlib_Color pixels[2]){
  Imlib_Color *bg = pixels, *fg = pixels + 1;
  uint8_t limit[] = {
    0x7f,
    0x7f,
    0x7f,
  };
  int bg_x = 40
    + ((bg->red > limit[0]) << 0)
    + ((bg->green > limit[1]) << 1)
    + ((bg->blue > limit[2]) << 2);

  int fg_x = 30
    + ((fg->red > limit[0]) << 0)
    + ((fg->green > limit[1]) << 1)
    + ((fg->blue > limit[2]) << 2);

  int bold =
    (fg->red > 0xa0) ||
    (fg->green > 0xa0) ||
    (fg->blue > 0xa0);

  cout << "\033[" << bold << ";" << fg_x << ";" << bg_x << "m";
  cout << "0";
}

void put_endl(){
  cout << "\033[0m";
  cout << endl;
}

int main(int argc, char* argv[]){
  if(argc < 2){
    cerr << "Usage: " << argv[0] << " <image file>\n";
    return 1;
  }

  Imlib_Image image = imlib_load_image(argv[1]);
  if(!image){
    cerr << "Invalid image\n";
    return 1;
  }

  imlib_context_set_image(image);

  int w = imlib_image_get_width(), h = imlib_image_get_height();
  int scaled_w = get_terminal_width() - 1;
  int scaled_h = (int)((double)h / (double)w * (double)scaled_w) * 2;

  Imlib_Image scaled = imlib_create_cropped_scaled_image(
    0, 0, w, h, scaled_w, scaled_h
  );

  cout << "New size: " << scaled_w << "x" << scaled_h << "\n";

  imlib_free_image();
  imlib_context_set_image(scaled);

  for(int y = 0; y < scaled_h - 1; y += 2){
    for(int x = 0; x < scaled_w; ++x){
      Imlib_Color color[2];
      imlib_image_query_pixel(x, y, color);
      imlib_image_query_pixel(x, y + 1, color + 1);
      put_chars(color);
    }
    put_endl();
  }
  imlib_free_image();


}