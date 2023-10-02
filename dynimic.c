#include<SDL2/SDL.h>
#include<SDL2/SDL_hints.h>
#include<SDL2/SDL_error.h>
#include<SDL2/SDL_log.h>
#include<SDL2/SDL_video.h>
#include<SDL2/SDL_render.h>
#include<SDL2/SDL_pixels.h>
#include<SDL2/SDL_rect.h>
#include<SDL2/SDL_surface.h>
#include<SDL2/SDL_clipboard.h>
#include<SDL2/SDL_events.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDl_timer.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<math.h>
#undef main                //275 183
#define width 275
#define height 183
#define Nin 275*183
#define Nout 275*183
#define Nneuron 10
#define lr 0.02
#define SEED   32
typedef struct NN
{

    float *weightso;
    float *weightsi;
    float *neuron;   //Midle storage  
    float biasi;
    float biaso;
    float *input;
    float *out;
    float *corect;

}NN;

float RandF(void)
{

    //return 1;
    if(rand()%2==0)
        return -1.0f*rand()/((float)RAND_MAX);
    else 
        return 1.0f*rand()/((float)RAND_MAX);
   
}

void init(NN *nn)
{
    	//srand(time(0));
	  	  srand(32);
	  	  
	 //DYNAMIC ALLOCATION
	 //system("pause");
	  nn->input=(float*)malloc(sizeof(float)*Nin);
	  nn->corect=(float*)malloc(sizeof(float)*Nout);
	  nn->out=(float*)malloc(sizeof(float)*Nout);	  
	  nn->neuron=(float*)malloc(sizeof(float)*Nneuron);	
	  nn->weightsi=(float*)malloc(sizeof(float)*Nin*Nneuron);
	  nn->weightso=(float*)malloc(sizeof(float)*Nout*Nneuron); 
     //system("pause"); 
    for (size_t i = 0; i < Nin; i++)
        nn->input[i]=0.0f;

    for (size_t i = 0; i < Nout; i++)
        nn->out[i]=0.0f;

       for (size_t i = 0; i < Nout; i++)
        nn->corect[i]=0.0f;
    
    for (size_t i = 0; i < Nin*Nneuron; i++)
        nn->weightsi[i]=RandF();

    for (size_t i = 0; i < Nin*Nneuron; i++)
        nn->weightso[i]=RandF();

        nn->biasi=0.0f;
        nn->biaso=0.0f;

    for (size_t i = 0; i < Nneuron; i++)
        nn->neuron[i]=0.0f;

}

float activation(float x)
{
    if(x > 0) return x;else return 0;
    return 1.0f/(1.0f+exp(-1.0*x));
    return tanhf(x);
    //return sin(x);
    //return fabsf(x);
} 
void forward(NN *nn)   //done
{

    int prescaler=0;
    for (size_t i = 0; i < Nneuron; i++)  //weights    ulaz
    {
        size_t j = 0;
        nn->neuron[i]=0.0f;
        for (j = 0; j < Nin ; j++)  // od prijasnjeg
        nn->neuron[i]+=(nn->weightsi[j+prescaler]*nn->input[j]);
        //nn->neuron[i]+=nn->biasi[i];
        nn->neuron[i]=activation(nn->neuron[i]+nn->biasi);
        prescaler+=j;
        //printf("in %d\n",prescaler);

    }
    prescaler=0;
     for (size_t i = 0; i < Nout; i++)  //weights    ulaz
    {
        size_t j=0;
        nn->out[i]=0.0f;
        for (j = 0; j < Nneuron ; j++)  // od prijasnjeg
            nn->out[i]+=(nn->weightso[j+prescaler]*nn->neuron[j]);
        //nn->out[i]+=nn->biaso[i];
        nn->out[i]=activation(nn->out[i]+nn->biaso);
        prescaler+=j;
        //printf("out %d\n",prescaler);
    }

}
 


void back(NN *nn)
{
      float sum=0.0f,sumb=0.0f;
      int prescaler_in=0,prescaler_out=0;
      for (size_t i = 0; i < Nneuron; i++)//go thru neurons
      {
          size_t j = 0;
          for (j = 0; j < Nout; j++)   //adjustment in out layer
          {
              const float derror=nn->out[j]-nn->corect[j];
              const float dsigmoid=nn->out[j]*(1.0f-nn->out[j]); 
              sum+=lr*derror*dsigmoid*nn->weightso[j+prescaler_in];
              sumb+=lr*derror*dsigmoid*nn->biaso;
              nn->weightso[j+prescaler_in]-=lr*derror*dsigmoid*nn->neuron[i];
              nn->biaso-=lr*derror*dsigmoid*nn->neuron[i];

          }
          size_t k = 0;
          for (k = 0; k < Nin; k++)  //adjustment in input layer
          {
            const float hv=nn->neuron[i]*(1.0f-nn->neuron[i]);
             nn->weightsi[k+prescaler_out]-=lr*sum*hv*nn->input[k];
             nn->biasi-=lr*sumb*hv*nn->input[k];
          }
          prescaler_in+=j;
          prescaler_out+=k;       
      }
      
}

void print(NN *nn)
{
    forward(nn);
    printf("\n");
    for (size_t i = 0; i < Nout*Nneuron; i++)
        printf("wo %f ",nn->weightso[i]);
    printf("\n");
    for (size_t i = 0; i < Nin*Nneuron; i++)
        printf("wi %f ",nn->weightsi[i]);
    printf("\n");

	printf("biasi %f biaso %f\n",nn->biasi,nn->biaso);

}

void printout(NN *nn)
{
    //nn->input[0]=0;
    //nn->input[1]=0;
    //forward(&nn);
    //printf("out[0] %f\n",nn->out[0]);
    nn->input[0]=1;
    nn->input[1]=1;
    //forward(&nn);
    printf("out[1] %f\n",nn->out[0]);
    //print(&nn);
}

void save_image_as_ppm(const char *file_path,uint32_t image[height][width])
{
    FILE *f = fopen(file_path, "wb");
    if (f == NULL) {
        fprintf(stderr, "ERROR: could write into file %s: %s\n", file_path, strerror(errno));
        exit(1);
    }
    fprintf(f, "P6\n%d %d 255\n",width, height);
    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            // 0xAABBGGRR
            uint32_t  pixel = image[y][x];
            uint8_t bytes[4] = {
                (pixel&0x000000FF)>>8*0,
                (pixel&0x0000FF00)>>8*1,
                //0,0,0,
                (pixel&0x00FF0000)>>8*2,
                (pixel&0xFF000000)>>8*3,
                //(pixel&0x00000000),
            };
            fwrite(bytes, sizeof(bytes), 1, f);
        }
    }

     fclose(f);
    //assert(ret == 0);
}


void read_image(const char *file_path,uint32_t image[height][width])
{
	FILE *f=fopen(file_path,"rb");
	char pom[23];
	fgets(pom,23,f);
	fgets(pom,23,f);
	fgets(pom,23,f);
	size_t x=0,y=0;
	while(!feof(f))
	{
		fread(&image[y][x],sizeof(uint32_t),1,f);
		x++;
		//void arrow;
		
		if(x==width)
		{
			x=0;
			y++;
		}
		if(y==height)
			{
				y=0;
				fclose(f);
				break;
			}
	}
		fclose(f);
}



int main(void)
{
    NN nn;
    init(&nn);
	uint32_t image[height][width],out[height][width];
	
    read_image("baa.ppm",image);
    system("pause");
    for(size_t i = 0, y =0; y < height; y++){
    	for(size_t x  = 0; x < width; x++){
    		nn.input[i] = (float)image[y][x];
    		nn.corect[i++] = (float)image[y][x];
		}
	}
    for(size_t i =0; i < 100; i++)
    {
    	printf(" iteracija: %d\n",i);

        forward(&nn);
        back(&nn);
    }
    
     for(size_t i = 0,y = 0; y < height; y++){
    	for(size_t x  = 0; x < width; x++){
    		out[y][x] = (uint32_t)nn.out[i++];
		}
	}
    save_image_as_ppm("saved.ppm",out);
    while(1)
    {
        
        //print(&nn);  
        printf("Unesite ulaz\n");
        scanf("%f %f",&nn.input[0],&nn.input[1]);
        forward(&nn);
        printf("out %f\n",nn.out[0]);

    }
    return 0;
}
