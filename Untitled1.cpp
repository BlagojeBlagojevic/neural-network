#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<math.h>
#include<winbgim.h>
#define SIZE_MAX 100*100*100*100*10
#define Nin 100*100
#define Nout 100*100
#define Nneuron 10
#define SEED   10
typedef struct NN
{

    float weightso[Nneuron*Nout];
    float weightsi[Nneuron*Nin];
    float neuron[Nneuron];   //Midle storage  
    float biasi[Nneuron*Nin];
    float biaso[Nneuron*Nout];
    uint32_t input[Nin];
    uint32_t out[Nout];
    uint32_t corect[Nout];

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

    for (size_t i = 0; i < Nneuron; i++)
        nn->biasi[i]=0;

    for (size_t i = 0; i < Nneuron; i++)
        nn->biaso[i]=0;

    for (size_t i = 0; i < Nneuron; i++)
        nn->neuron[i]=0.0f;

}

float activation(float x)
{
    if(x>0) return x;else return -0.01;
    //return 1.0f/(1.0f+exp(-1.0*x));
    //return tanhf(x);
    //return sin(x)*x;
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
        nn->neuron[i]=activation(nn->neuron[i]);
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
        nn->out[i]=activation(nn->out[i]);
        prescaler+=j;
        //printf("out %d\n",prescaler);
    }

}
 
#define lr 1

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
              sumb+=lr*derror*dsigmoid*nn->biaso[j+prescaler_in];
              nn->weightso[j+prescaler_in]-=lr*derror*dsigmoid*nn->neuron[i];
              //nn->biaso[j+prescaler_in]-=lr*derror*dsigmoid*nn->neuron[i];

          }
          size_t k = 0;
          for (k = 0; k < Nin; k++)  //adjustment in input layer
          {
            const float hv=nn->neuron[i]*(1.0f-nn->neuron[i]);
             nn->weightsi[k+prescaler_out]-=lr*sum*hv*nn->input[k];
             //nn->biaso[j+prescaler_out]-=lr*sumb*hv*nn->input[k];
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

    for (size_t i = 0; i < Nout; i++)
        printf("out[%d]: %f \n",i+1,nn->out[i]);

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
//275 183 236 378
//const int height=378,width=236;
const int height=100,width=100;

void save_image_as_ppm(const char *file_path,uint32_t image[height][width])
{
    FILE *f = fopen(file_path, "wb");
    if (f == NULL) {
        fprintf(stderr, "ERROR: could write into file %s: %s\n", file_path, strerror(errno));
        exit(1);
    }
    fprintf(f, "P6\n%d %d 255\n",width, height);
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            // 0xAABBGGRR
            uint32_t pixel = image[y][x];
            uint8_t bytes[4] = {
                (pixel&0x000000FF)>>8*0,
                (pixel&0x0000FF00)>>8*1,
                (pixel&0x00FF0000)>>8*2,
                (pixel&0xFF000000)>>8*3,
            };
            fwrite(bytes, sizeof(bytes), 1, f);
            //assert(!ferror(f));
        }
    }

     fclose(f);
    //assert(ret == 0);
}


void read_image(uint32_t image[height][width])
{
	FILE *f=fopen("asd.ppm","rb");
	char pom[23];
	fgets(pom,23,f);
	fgets(pom,23,f);
	fgets(pom,23,f);
	size_t x=0,y=0;
	while(!feof(f))
	{
		fread(&image[y][x],sizeof(uint32_t),1,f);
		//fread(&image[0][0],sizeof(uint32_t),1,f);
		//fread(&image[0][0],sizeof(uint32_t),1,f);
		printf("0x%X\n",image[y][x]);
		putpixel(x,y,image[y][x]);
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

void throught_image(NN *nn, uint32_t image[height][width])
{
	size_t j = 0;
	for(size_t y; y < height; y++)
        {
        	for(size_t x; x < width; x++)
        	{
        		nn->input[j]=image[y][x];
        		nn->corect[j]=image[y][x];
        		j++;
			}
		}
}

void save_half_image(NN *nn,uint32_t image[height][width])
{
		size_t j = 0;
		uint32_t image1[height][width];
		for(size_t y; y < height; y++)	
	     {
        	for(size_t x; x < width; x++)
        	{
        		image1[y][x]=(uint32_t)nn->corect[j];
        		j++;
        		printf("img1 0x%X\n",image1[y][x]);
			}
		}
	save_image_as_ppm("saved.ppm",image1);
}

int main(void)
{
	system("pause");
    NN nn;
    init(&nn);
    //print(&nn);
    
    int gdriver = DETECT, gmode, errorcode;
	
    initwindow(height,width,"image");
	uint32_t image[height][width],image1[height][width];
	read_image(image);
	save_image_as_ppm("saved.ppm",image);
	//system("saved.ppm");
    //printout(&nn);
float input[][3]={  {1.0f,1.0f,1.0f},
                    {1.0f,0.0f,1.0f},
                    {0.0f,1.0f,1.0f},
                    {0.0f,0.0f,0.0f}
                };  
    //print(&nn);
    for(size_t i = 0; i < 10000; i++)
    {
        
       	throught_image(&nn,image);
        forward(&nn);
        back(&nn);
        if(i==9999)
        	save_half_image(&nn,image1);
        
    }
   
    return 0;
}
