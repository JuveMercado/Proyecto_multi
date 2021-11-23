#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
int main()
{
    FILE *image, *outputImage, *lecturas;
    for (int s = 0; s < 10; s += 1)
  {
    omp_set_num_threads(s);
     const double start = omp_get_wtime();
    image = fopen("f1.bmp","rb");          //Imagen original a transformar
    outputImage = fopen("grises.bmp","wb");    //Imagen transformada

    unsigned char r, g, b;               //Pixel

    for(int i=0; i<54; i++) fputc(fgetc(image), outputImage);   //Copia cabecera a nueva imagen
    #pragma omp parallel 
        {
            #pragma while schedule(dynamic)
            while(!feof(image)){                                        //Grises
       b = fgetc(image);
       g = fgetc(image);
       r = fgetc(image);

       unsigned char pixel = 0.21*r+0.72*g+0.07*b;
       fputc(pixel, outputImage);
       fputc(pixel, outputImage);
       fputc(pixel, outputImage);
    }}
const double end = omp_get_wtime();
  printf("%lf\n", (end - start));
    fclose(image);
    fclose(outputImage);
 }
    return 0;
}
