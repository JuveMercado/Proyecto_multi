#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

int main()
{
    FILE *image, *outputImage, *lecturas;
      for (int s = 0; s < 3; s += 1)
  {
    omp_set_num_threads(s);
    image = fopen("f1.bmp","rb");          //Imagen original a transformar
    outputImage = fopen("blur.bmp","wb");    //Imagen transformada
    long ancho;
    long alto;
    unsigned char r, g, b;               //Pixel
    unsigned char* ptr;

    unsigned char xx[54];
    int cuenta = 0;
    for(int i=0; i<54; i++) {
      xx[i] = fgetc(image);
      fputc(xx[i], outputImage);   //Copia cabecera a nueva imagen
    }
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    //printf("largo img %li\n",alto);
    //printf("ancho img %li\n",ancho);

    ptr = (unsigned char*)malloc(alto*ancho*3* sizeof(unsigned char));

    while(!feof(image)){
      b = fgetc(image);
      g = fgetc(image);
      r = fgetc(image);

      unsigned char pixel = 0.21*r+0.72*g+0.07*b;
      ptr[cuenta] = pixel; //b
      ptr[cuenta+1] = pixel; //g
      ptr[cuenta+2] = pixel; //r
      cuenta++;
    }

    
 const double start = omp_get_wtime();
   #pragma omp parallel for schedule(dynamic)
    for (int i = ancho; i < alto*ancho*3-ancho; ++i) {
            fputc(ptr[i-3-ancho]/9+ptr[i-ancho]/9+ptr[i+3-ancho]/9+ptr[i-3]/9+ptr[i]/9+ptr[i+3]/9+ptr[i-3+ancho]/9+ptr[i+ancho]/9+ptr[i+3+ancho]/9, outputImage);
            fputc(ptr[i-3-ancho+1]/9+ptr[i-ancho+1]/9+ptr[i+3-ancho+1]/9+ptr[i-3+1]/9+ptr[i+1]/9+ptr[i+3+1]/9+ptr[i-3+ancho+1]/9+ptr[i+ancho+1]/9+ptr[i+3+ancho+1]/9, outputImage);
            fputc(ptr[i-3-ancho+2]/9+ptr[i-ancho+2]/9+ptr[i+3-ancho+2]/9+ptr[i-3+2]/9+ptr[i+2]/9+ptr[i+3+2]/9+ptr[i-3+ancho+2]/9+ptr[i+ancho+2]/9+ptr[i+3+ancho+2]/9, outputImage);
      cuenta--;
      if (cuenta == 0){
        cuenta = ancho;
      }}
const double end = omp_get_wtime();
  printf("%lf\n", (end - start));
    free(ptr);
    fclose(image);
    fclose(outputImage);
   // printf("fin");
  }
    return 0;
}
