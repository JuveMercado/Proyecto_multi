#include <stdio.h>
#include <stdlib.h>
#include<omp.h>
#define NUM_THREADS 8

void img_blur(long alto, long ancho, FILE *file, unsigned char* ptr){
    int avgB = 0, avgG = 0, avgR = 0;
    int ile = 0;
    #pragma omp parallel
    {
        #pragma omp for nowait
            for(int xx = 0; xx < alto; xx++){
                for(int yy = 0; yy < ancho; yy++){
                    avgB = avgG = avgR = 0;
                    ile = 0;

                    for(int x = xx; x < alto && x < xx + 11; x++){
                        for(int y = yy; y < ancho && y < yy + 11; y++){
                            avgB += ptr[(x) * ancho + (y) ];
                            avgG += ptr[(x) * ancho + (y) + 1];
                            avgR += ptr[(x) * ancho + (y) + 2];
                            ile++;
                        }
                    }
                    avgB = avgB / ile;
                    avgG = avgG / ile;
                    avgR = avgR / ile;

                    fputc(ptr[(xx) * ancho + (yy) ] = avgB, file);
                    fputc(ptr[(xx) * ancho + (yy) + 1] = avgG, file);
                    fputc(ptr[(xx) * ancho + (yy) + 2] = avgR, file);
                }
            }
    }
}



int main()
{
    const double startTime = omp_get_wtime();
    omp_set_num_threads(NUM_THREADS);
    FILE *image, *image2, *outputImage, *outputImage2, *lecturas;
    image = fopen("f1.bmp","rb");          //Imagen original a transformar
    image2 = fopen("f2.bmp","rb");          //Imagen original a transformar
    outputImage = fopen("blur_img_1.bmp","wb");    //Imagen transformada
    outputImage2 = fopen("blur_img_2.bmp","wb");    //Imagen transformada
    long ancho;
    long alto;
    long ancho2;
    long alto2;
    unsigned char r, g, b;               //Pixel
    unsigned char* ptr;
    unsigned char* ptr2;
    int avgB = 0, avgG = 0, avgR = 0;
    int ile = 0;

    //Specifications of  image 1
    unsigned char xx[54];
    int cuenta = 0;
    int cuenta2 = 0;
    for(int i=0; i<54; i++) {
      xx[i] = fgetc(image);
      fputc(xx[i], outputImage);   //Copia cabecera a nueva imagen
    }
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    printf("largo img %li\n",alto);
    printf("ancho img %li\n",ancho);

    ptr = (unsigned char*)malloc(alto*ancho*3* sizeof(unsigned char));

    while(!feof(image)){
      b = fgetc(image);
      g = fgetc(image);
      r = fgetc(image);
       
      unsigned char pixel = 0.21*r+0.72*g+0.07*b;

      ptr[cuenta]= pixel; //b
      ptr[cuenta+1] = pixel; //g
      ptr[cuenta+2] = pixel; //r
      cuenta++;
    }
    //Specifications of  image 2
    for(int i=0; i<54; i++) {
      xx[i] = fgetc(image2);
      fputc(xx[i], outputImage2);   //Copia cabecera a nueva imagen
    }
    ancho2 = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto2 = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];
    printf("largo img %li\n",alto2);
    printf("ancho img %li\n",ancho2);

    ptr2 = (unsigned char*)malloc(alto2*ancho2*3* sizeof(unsigned char));

    while(!feof(image2)){
      b = fgetc(image2);
      g = fgetc(image2);
      r = fgetc(image2);
       
      unsigned char pixel = 0.21*r+0.72*g+0.07*b;

      ptr2[cuenta2]= pixel; //b
      ptr2[cuenta2+1] = pixel; //g
      ptr2[cuenta2+2] = pixel; //r
      cuenta2++;
    } 
    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section // 
            {
                img_blur(alto, ancho, outputImage, ptr);
            }
            #pragma omp section //
            {
                img_blur(alto2, ancho2, outputImage2, ptr2);
            }
        }  
    }
    

    free(ptr);
    free(ptr2);
    fclose(image);
    fclose(outputImage);
    fclose(image2);
    fclose(outputImage2);
    const double endTime = omp_get_wtime();
    printf("tomo (%lf) segundos\n", (endTime - startTime));
    return 0;
}