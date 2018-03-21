#include <stdio.h>

typedef struct _pixel {
    unsigned short int r;
    unsigned short int g;
    unsigned short int b;
} Pixel;

typedef struct _image {
    // [width][height][rgb]
    // 0 -> r
    // 1 -> g
    // 2 -> b
    unsigned short int pixel[512][512][3];
    unsigned int w;
    unsigned int h;
    char tipoImagem[4];
    int max_color; // é um atributo relacionado a imagem, então melhor colocar no "objeto"
} Image;


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

int pixel_igual(Pixel p1, Pixel p2) {
    if (p1.r == p2.r &&
        p1.g == p2.g &&
        p1.b == p2.b)
        return 1;
    return 0;
}


Image escala_de_cinza(Image img) {
    /*for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            print("%u", img.pixel[i][j][0] + img.pixel[i][j][1] + img.pixel[i][j][2]);
        }
    }*/

    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            int media = img.pixel[i][j][0] +
                        img.pixel[i][j][1] +
                        img.pixel[i][j][2];
            media /= 3;
            img.pixel[i][j][0] = media;
            img.pixel[i][j][1] = media;
            img.pixel[i][j][2] = media;
        }
    }

    return img;
}

void blur(unsigned int h, unsigned short int pixel[512][512][3], int T, unsigned int w) {
    for (unsigned int i = 0; i < h; ++i) {
        for (unsigned int j = 0; j < w; ++j) {
            Pixel media = {0, 0, 0};

            int menor_h = (h - 1 > i + T/2) ? i + T/2 : h - 1;
            int min_w = (w - 1 > j + T/2) ? j + T/2 : w - 1;
            for(int x = (0 > i - T/2 ? 0 : i - T/2); x <= menor_h; ++x) {
                for(int y = (0 > j - T/2 ? 0 : j - T/2); y <= min_w; ++y) {
                    media.r += pixel[x][y][0];
                    media.g += pixel[x][y][1];
                    media.b += pixel[x][y][2];
                }
            }

            // printf("%u", media.r)
            media.r /= T * T;
            media.g /= T * T;
            media.b /= T * T;

            pixel[i][j][0] = media.r;
            pixel[i][j][1] = media.g;
            pixel[i][j][2] = media.b;
        }
    }
}

Image rotacionar90direita(Image img) {
    Image rotacionada;

    rotacionada.w = img.h;
    rotacionada.h = img.w;

    for (unsigned int i = 0, y = 0; i < rotacionada.h; ++i, ++y) {
        for (int j = rotacionada.w - 1, x = 0; j >= 0; --j, ++x) {
            rotacionada.pixel[i][j][0] = img.pixel[x][y][0];
            rotacionada.pixel[i][j][1] = img.pixel[x][y][1];
            rotacionada.pixel[i][j][2] = img.pixel[x][y][2];
        }
    }

    return rotacionada;
}

Image inverter_cores(Image img) {
  Image temp = img;

  for (unsigned int i = 0; i < temp.h; ++i) {
      for (unsigned int j = 0; j < temp.w; ++j) {
          temp.pixel[i][j][0] = 255 - temp.pixel[i][j][0];
          temp.pixel[i][j][1] = 255 - temp.pixel[i][j][1];
          temp.pixel[i][j][2] = 255 - temp.pixel[i][j][2];
      }
  }
  return temp;
}

Image cortar_imagem(Image img) {
    Image cortada;
    int x, y;

    scanf("%d %d", &x, &y);
    scanf("%d %d", &cortada.w, &cortada.h);

    for(int i = 0; i < cortada.h; ++i) {
        for(int j = 0; j < cortada.w; ++j) {
            cortada.pixel[i][j][0] = img.pixel[i + y][j + x][0];
            cortada.pixel[i][j][1] = img.pixel[i + y][j + x][1];
            cortada.pixel[i][j][2] = img.pixel[i + y][j + x][2];
        }
    }
    return cortada;
}

Image espelhar(Image img){
  int horizontal = 0;
  scanf("%d", &horizontal);

  int w = img.w, h = img.h, x, y;
  Image temp = img;

  if (horizontal == 1)
    w /= 2;
  else
    h /= 2;

  for (int i = 0; i < h; ++i) {
      for (int j = 0; j < w; ++j) {
          x = i; y = j;
          if (horizontal == 1)
            y = temp.w - 1 - j;
          else
            x = temp.h - 1 - i;

          Pixel aux1;
          aux1.r = temp.pixel[i][j][0];
          aux1.g = temp.pixel[i][j][1];
          aux1.b = temp.pixel[i][j][2];

          temp.pixel[i][j][0] = temp.pixel[x][y][0];
          temp.pixel[i][j][1] = temp.pixel[x][y][1];
          temp.pixel[i][j][2] = temp.pixel[x][y][2];

          temp.pixel[x][y][0] = aux1.r;
          temp.pixel[x][y][1] = aux1.g;
          temp.pixel[x][y][2] = aux1.b;
      }
  }
  return temp;
}


Image ler_imagem(){

  Image temp;

  scanf("%s", temp.tipoImagem);

  scanf("%u %u %d", &temp.w, &temp.h, &temp.max_color);

  for (unsigned int i = 0; i < temp.h; ++i) {
      for (unsigned int j = 0; j < temp.w; ++j) {
          scanf("%hu %hu %hu", &temp.pixel[i][j][0],
                               &temp.pixel[i][j][1],
                               &temp.pixel[i][j][2]);
      }
  }
  return temp;
}

void imprimir_imagem(Image img){
  // print type of image
  printf("P3\n");
  // print width height and color of image
  printf("%u %u\n255\n", img.w, img.h);

  // print pixels of image
  for (unsigned int i = 0; i < img.h; ++i) {
      for (unsigned int j = 0; j < img.w; ++j) {
          printf("%hu %hu %hu ", img.pixel[i][j][0],
                                 img.pixel[i][j][1],
                                 img.pixel[i][j][2]);

      }
      printf("\n");
  }
}

Image sepia(Image img){

  Image sepia;

  sepia = img;

  for (unsigned int i = 0; i < sepia.h; i++) {
      for (unsigned int j = 0; j < sepia.w; ++j) {
          unsigned short int pixel[3];
          pixel[0] = sepia.pixel[i][j][0];
          pixel[1] = sepia.pixel[i][j][1];
          pixel[2] = sepia.pixel[i][j][2];

          int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
          sepia.pixel[i][j][0] = min(p, 255);

          p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
          sepia.pixel[i][j][1] = min(p, 255);

          p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
          sepia.pixel[i][j][2] = min(p, 255);
      }
  }
  return sepia;
}

int main() {
    Image img;

    img = ler_imagem();

    int n_opcoes;
    scanf("%d", &n_opcoes);

    for(int i = 0; i < n_opcoes; ++i) {
        int opcao;
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: { // Escala de Cinza
                img = escala_de_cinza(img);
                break;
            }
            case 2: { // Filtro Sepia
                img = sepia(img);
                break;
            }
            case 3: { // Blur
                int tamanho = 0;
                scanf("%d", &tamanho);
                blur(img.h, img.pixel, tamanho, img.w);
                break;
            }
            case 4: { // Rotacao
                int quantas_vezes = 0;
                scanf("%d", &quantas_vezes);
                quantas_vezes %= 4;
                for (int j = 0; j < quantas_vezes; ++j) {
                    img = rotacionar90direita(img);
                }
                break;
            }
            case 5: { // Espelhamento
                img = espelhar(img);
                break;
            }
            case 6: { // Inversao de Cores
                img = inverter_cores(img);
                break;
            }
            case 7: { // Cortar Imagem
                img = cortar_imagem(img);
                break;
            }
        }

    }

    imprimir_imagem(img);

    return 0;
}
