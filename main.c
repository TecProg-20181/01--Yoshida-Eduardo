#include <stdio.h>

typedef struct _pixel {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    Pixel pixel[512][512];
    unsigned int w;
    unsigned int h;
    char tipoImagem[4];
    int max_color;
} Image;

int max(int a, int b);
int min(int a, int b);

Pixel copy_pixel(Pixel p);
Image grayscale(Image img);
Image blur(Image img);
Image rotate_90_right(Image img);
Image invert_colors(Image img);
Image crop_image(Image img);
Image mirror(Image img);
Image read_image();
void print_image(Image img);
Image sepia(Image img);


int max(int a, int b) {
    if (a > b)
        return a;
    return b;
}

int min(int a, int b){
    if(a < b)
      return a;
    return b;
}

Pixel copy_pixel(Pixel p){
  Pixel temp;
  temp.r = p.r;
  temp.g = p.g;
  temp.b = p.b;

  return temp;
}

unsigned int avg_pixel(Pixel p){
  unsigned int avg = (p.r + p.g + p.b)/3;
  return avg;
}

Pixel passar_valor(unsigned int valor){
  Pixel p;
  p.r = valor;
  p.g = valor;
  p.b = valor;
  return p;
}

Image grayscale(Image img) {
    Image temp = img;
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            temp.pixel[i][j] = passar_valor(avg_pixel(temp.pixel[i][j]));
        }
    }
    return temp;
}

Image blur(Image img) {

    int amount = 0;
    scanf("%d", &amount);

    Image temp = img;

    for (unsigned int i = 0; i < temp.h; ++i) {
        for (unsigned int j = 0; j < temp.w; ++j) {
            Pixel avg = {0, 0, 0};

            int menor_h = min(i + amount/2, temp.h -1);
            int menor_w = min(j + amount/2, temp.w - 1);
            for(int x = max(0, i - amount/2); x <= menor_h; ++x) {
                for(int y = max(0, j - amount/2); y <= menor_w; ++y) {
                    avg.r += temp.pixel[x][y].r;
                    avg.g += temp.pixel[x][y].g;
                    avg.b += temp.pixel[x][y].b;
                }
            }

            temp.pixel[i][j].r = avg.r/(amount * amount);
            temp.pixel[i][j].g = avg.g/(amount * amount);
            temp.pixel[i][j].b = avg.b/(amount * amount);
        }
    }

    return temp;
}

Image rotate_90_right(Image img) {

    int number;
    scanf("%d", &number);
    number = number % 4;

    Image temp;

    temp.w = img.h;
    temp.h = img.w;

    for(int x = 0; x < number; x++){
      for (unsigned int i = 0; i < temp.h; ++i) {
          for (int j = temp.w - 1, x = 0; j >= 0; --j, ++x) {
              temp.pixel[i][j] = copy_pixel(img.pixel[x][i]);
          }
      }
    }
    
    return temp;
}

Image invert_colors(Image img) {
  Image temp = img;

  for (unsigned int i = 0; i < temp.h; ++i) {
      for (unsigned int j = 0; j < temp.w; ++j) {
          temp.pixel[i][j].r = 255 - temp.pixel[i][j].r;
          temp.pixel[i][j].g = 255 - temp.pixel[i][j].g;
          temp.pixel[i][j].b = 255 - temp.pixel[i][j].b;
      }
  }
  return temp;
}

Image crop_image(Image img) {
    Image temp;
    int x, y;

    scanf("%d %d", &x, &y);
    scanf("%d %d", &temp.w, &temp.h);

    for(int i = 0; i < temp.h; ++i) {
        for(int j = 0; j < temp.w; ++j) {
            temp.pixel[i][j] = copy_pixel(img.pixel[i + y][j + x]);
        }
    }
    return temp;
}

Image mirror(Image img){
  int horizontal = 0;
  scanf("%d", &horizontal);

  int w = img.w, h = img.h;
  Image temp = img;

  if (horizontal == 1)
    w /= 2;
  else
    h /= 2;

  for (int i = 0; i < h; ++i) {
      for (int j = 0; j < w; ++j) {
          int x = i, y = j;
          if (horizontal == 1)
            y = temp.w - 1 - j;
          else
            x = temp.h - 1 - i;

          Pixel aux1;

          aux1 = copy_pixel(temp.pixel[i][j]);
          temp.pixel[i][j] = copy_pixel(temp.pixel[x][y]);
          temp.pixel[x][y] = copy_pixel(aux1);
      }
  }
  return temp;
}


Image read_image(){

  Image temp;

  scanf("%s", temp.tipoImagem);

  scanf("%u %u %d", &temp.w, &temp.h, &temp.max_color);

  for (unsigned int i = 0; i < temp.h; ++i) {
      for (unsigned int j = 0; j < temp.w; ++j) {
          scanf("%hu %hu %hu", &temp.pixel[i][j].r,
                               &temp.pixel[i][j].g,
                               &temp.pixel[i][j].b);
      }
  }
  return temp;
}

void print_image(Image img){
  // print type of image
  printf("P3\n");

  // print width height and color of image
  printf("%u %u\n255\n", img.w, img.h);

  // print pixels of image
  for (unsigned int i = 0; i < img.h; ++i) {
      for (unsigned int j = 0; j < img.w; ++j) {
          printf("%hu %hu %hu ", img.pixel[i][j].r,
                                 img.pixel[i][j].g,
                                 img.pixel[i][j].b);

      }
      printf("\n");
  }
}

Image sepia(Image img){

  Image temp;

  temp = img;

  for (unsigned int i = 0; i < temp.h; i++) {
      for (unsigned int j = 0; j < temp.w; ++j) {

          int p =  temp.pixel[i][j].r * .393 + temp.pixel[i][j].g * .769 + temp.pixel[i][j].b * .189;
          temp.pixel[i][j].r = min(p, 255);

          p =  temp.pixel[i][j].r * .349 + temp.pixel[i][j].g * .686 + temp.pixel[i][j].b* .168;
          temp.pixel[i][j].g = min(p, 255);

          p =  temp.pixel[i][j].r * .272 + temp.pixel[i][j].g * .534 + temp.pixel[i][j].b * .131;
          temp.pixel[i][j].b = min(p, 255);
      }
  }
  return temp;
}


int main() {
    Image img = read_image();

    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                img = grayscale(img);
                break;
            }
            case 2: { // Filtro Sepia
                img = sepia(img);
                break;
            }
            case 3: { // Blur
                img = blur(img);
                break;
            }
            case 4: { // Rotacao
                img = rotate_90_right(img);
                break;
            }
            case 5: { // Espelhamento
                img = mirror(img);
                break;
            }
            case 6: { // Inversao de Cores
                img = invert_colors(img);
                break;
            }
            case 7: { // Cortar Imagem
                img = crop_image(img);
                break;
            }
        }

    }
    print_image(img);

    return 0;
}
