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

Pixel passar_pixel(Pixel p){
  Pixel temp;
  temp.r = p.r;
  temp.g = p.g;
  temp.b = p.b;

  return temp;
}

Image escala_de_cinza(Image img) {
    for (unsigned int i = 0; i < img.h; ++i) {
        for (unsigned int j = 0; j < img.w; ++j) {
            int media = img.pixel[i][j].r +
                        img.pixel[i][j].g +
                        img.pixel[i][j].b;
            media /= 3;
            img.pixel[i][j].r = media;
            img.pixel[i][j].g = media;
            img.pixel[i][j].b = media;
        }
    }

    return img;
}

Image blur(Image img) {

    int quantidade = 0;
    scanf("%d", &quantidade);

    Image temp = img;

    for (unsigned int i = 0; i < temp.h; ++i) {
        for (unsigned int j = 0; j < temp.w; ++j) {
            Pixel media = {0, 0, 0};

            int menor_h = min(i + quantidade/2, temp.h -1);
            int menor_w = min(j + quantidade/2, temp.w - 1);
            for(int x = max(0, i - quantidade/2); x <= menor_h; ++x) {
                for(int y = max(0, j - quantidade/2); y <= menor_w; ++y) {
                    media.r += temp.pixel[x][y].r;
                    media.g += temp.pixel[x][y].g;
                    media.b += temp.pixel[x][y].b;
                }
            }

            temp.pixel[i][j].r = media.r/(quantidade * quantidade);
            temp.pixel[i][j].g = media.g/(quantidade * quantidade);
            temp.pixel[i][j].b = media.b/(quantidade * quantidade);
        }
    }

    return temp;
}

Image rotacionar90direita(Image img) {
    
    Image rotacionada;

    rotacionada.w = img.h;
    rotacionada.h = img.w;

    for (unsigned int i = 0; i < rotacionada.h; ++i) {
        for (int j = rotacionada.w - 1, x = 0; j >= 0; --j, ++x) {
            rotacionada.pixel[i][j] = passar_pixel(img.pixel[x][i]);
        }
    }

    return rotacionada;
}

Image inverter_cores(Image img) {
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

Image cortar_imagem(Image img) {
    Image cortada;
    int x, y;

    scanf("%d %d", &x, &y);
    scanf("%d %d", &cortada.w, &cortada.h);

    for(int i = 0; i < cortada.h; ++i) {
        for(int j = 0; j < cortada.w; ++j) {
            cortada.pixel[i][j] = passar_pixel(img.pixel[i + y][j + x]);
        }
    }
    return cortada;
}

Image espelhar(Image img){
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

          aux1 = passar_pixel(temp.pixel[i][j]);
          temp.pixel[i][j] = passar_pixel(temp.pixel[x][y]);
          temp.pixel[x][y] = passar_pixel(aux1);
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
          scanf("%hu %hu %hu", &temp.pixel[i][j].r,
                               &temp.pixel[i][j].g,
                               &temp.pixel[i][j].b);
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
          printf("%hu %hu %hu ", img.pixel[i][j].r,
                                 img.pixel[i][j].g,
                                 img.pixel[i][j].b);

      }
      printf("\n");
  }
}

Image sepia(Image img){

  Image sepia;

  sepia = img;

  for (unsigned int i = 0; i < sepia.h; i++) {
      for (unsigned int j = 0; j < sepia.w; ++j) {

          int p =  sepia.pixel[i][j].r * .393 + sepia.pixel[i][j].g * .769 + sepia.pixel[i][j].b * .189;
          sepia.pixel[i][j].r = min(p, 255);

          p =  sepia.pixel[i][j].r * .349 + sepia.pixel[i][j].g * .686 + sepia.pixel[i][j].b* .168;
          sepia.pixel[i][j].g = min(p, 255);

          p =  sepia.pixel[i][j].r * .272 + sepia.pixel[i][j].g * .534 + sepia.pixel[i][j].b * .131;
          sepia.pixel[i][j].b = min(p, 255);
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
                img = blur(img);
                break;
            }
            case 4: { // Rotacao
                img = rotacionar90direita(img);
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
