#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<math.h>
#include<time.h>
#define Nin 784
#define Nout 10
#define Nneuron 100
#define lr 1
#define SEED   32


float error;
float sumerror=0;

typedef struct NN
{

    float weightso[Nneuron*Nout];
    float weightsi[Nneuron*Nin];
    float neuron[Nneuron];   //Midle storage  
    float biasi;
    float biaso;
    float input[Nin];
    float out[Nout];
    float corect[Nout];

}NN;

float RandF(void)
{

    //return 1;
    if(rand()%2==0)
        //return -1.0f*rand()/((float)RAND_MAX);
    	return -1.0f*(rand()%255);
	else 
        //return 1.0f*rand()/((float)RAND_MAX);
   		return 1.0f*(rand()%255);
}

void init(NN *nn)
{
    	//srand(time(0));
	  	srand(SEED);
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
    //if(x > 0.0f) return x; else return 0;
    return 1.0f/(1.0f+exp(-1.0*x));
    //return tanhf(x);
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
void load_mnist(NN *nn)
{
	FILE *f = fopen("MNIST_test.txt","rb");
	size_t c=0;
	while(!feof(f)&&(c < 1000000)){     
		uint8_t corect=fgetc(f)-48;
		//first char
		for(size_t i = 0; i < 10; i++){c++;
			if(i==corect)
				nn->corect[i]=1.0f;
			else
				nn->corect[i]=0.0f;
		}	
		int16_t j=0,corect1=0,multiplayer = 10,count = 0 ;
		while(j < 784){
			int16_t pixel =  fgetc(f)-48;
			if(pixel!=-4)
			{
				
				corect1+=(multiplayer)*pixel;
				multiplayer/=10;
				count++;
			}
		
			if(pixel==-4){
				if(count==1)
					corect1=corect1/100;
				if(count==2)
					corect1=corect1/10;
				
				nn->input[j] = corect1;
				//printf("%f", nn->input[j]); 
				j++;
				corect1=0;
				count = 0;
				multiplayer=100;
			}
		}
			
      	//char pom = fgetc(f);//New line
			forward(nn);
        	back(nn);
        	error = nn->out[corect];
        	c++;
        	char ch='%';
        	sumerror+=(1.0f-error);
        	if(c%1000==0){//continue;
        		printf("average corect per 1000: %.2f%c\n", 100.0f*((float)sumerror)/1000.0f,ch);sumerror=0;
			}
			
	 } 
	
}

int main(void)
{
    NN nn;
    init(&nn);
    //print(&nn);
    //printout(&nn);
float input[][3]={  {1.0f,1.0f,1.0f},
                    {1.0f,0.0f,1.0f},
                    {0.0f,1.0f,1.0f},
                    {0.0f,0.0f,0.0f}
                };  
    //print(&nn);
    //train_mnist(&nn);
    for(size_t i =0; i < 10; i++)
    {
     	load_mnist(&nn);        
        //printf("%u \n", i);
    }
    
    while(1)
    {
        
        //print(&nn);  
        //load_mnist(&nn);
        forward(&nn);
        for(size_t i = 0;i < 10; i++)
        	printf("%d: %f\n",i,nn.out[i]);
        system("pause");
        system("cls");

    }
    return 0;
}
