#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{
  FILE *image, *outputImage, *lecturas;
  image = fopen("f1.bmp","rb");          //Imagen original a transformar
  int foto = 0;
  for (int i = 10; i > 200; i += 10){
    omp_set_num_threads(i);
    outputImage = fopen("img2_dd_{1}.bmp".format(foto)"wb");    //Imagen transformada
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
    printf("largo img %li\n",alto);
    printf("ancho img %li\n",ancho);

    ptr = (unsigned char*)malloc(alto*ancho*3* sizeof(unsigned char));

    #pragma omp parallel 
    {
      #pragma while schedule(dynamic)
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
    }

    
                                       //Grises

    const double start = omp_get_wtime();
    cuenta = alto*ancho/2;
    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < alto*ancho*3; ++i) {
      fputc(ptr[i+2], outputImage); //(cuenta*3) //(ancho/2)
      fputc(ptr[i+1], outputImage);
      fputc(ptr[i], outputImage);
      /*cuenta--;
      if (cuenta == 0){
        cuenta = alto*ancho/2;
      }*/
    }
      
    const double end = omp_get_wtime();
    printf("%lf\n", (end - start));

    free(ptr);
    
    fclose(outputImage);
    foto++;
  }
    
    fclose(image);
    return 0;
}
