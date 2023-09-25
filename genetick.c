#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include <complex.h>
#include<string.h>
#define Nin 2
#define Nout 1
#define Nneuron 20
#define SEED   10

typedef struct NN
{

    float weightso[Nneuron*Nout];
    float weightsi[Nneuron*Nin];
    float neuron[Nneuron];   //Midle storage  
    float biasi[Nneuron*Nin];
    float biaso[Nneuron*Nout];
    float input[Nin];
    float out[Nout];
    float corect[Nout];

}NN;

float RandF(void)
{

    //return 0;
    if(rand()%2==0)
        return -1.0f*rand()/((float)RAND_MAX);
    else 
        return 1.0f*rand()/((float)RAND_MAX);
   
}

void init(NN *nn)
{
    srand(time(0));

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
    //return x;
    //return 1.0f/(1.0f+exp(-1.0*x));
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
float score_avr=0.0,n=1;
float genetick(NN nn[100])
{
	float score[100];
	int network = 0 ; //best
	for(size_t i = 0;i < 100; i++)
		score[i]=0.0f;
	for(size_t i = 0;i < 100; i++){
		forward(&nn[i]);
		for(size_t j = 0; j < Nout; j++)
		{
			score[i]+=activation(nn[i].corect[j]-nn[i].out[j]);
		}
		score[i]/=Nout;
	
	
	for(size_t k = 0;k < i; k++){
	
		if(score[i]<score[k])
		{
			float pom = score[k];
			score[k]=score[i];
			score[i]=score[k];
			network=i;	
		}	
	}}


	for(size_t i = 0; i < 100; i++)
	{
		if(i!=network)
		memccpy(&nn[i],&nn[network],1,sizeof(NN));
	}
	
	for(size_t i = 0; i < 100; i++)
	{
			for(size_t y = 0; y < Nin*Nneuron; y++){
				if((float)rand()/RAND_MAX > 0.8f)
				{
					if((float)rand()/RAND_MAX>0.9)
						nn[i].weightsi[y]-=1;
					else
						nn[i].weightsi[y]+=1;
					//print(&nn[i]);
				}
			}
			
				for(size_t y = 0; y < Nin*Nout; y++){
				if((float)rand()/RAND_MAX > 0.8f)
				{
					if((float)rand()/RAND_MAX>0.9)
						nn[i].weightso[y]-=0.1;
					else
						nn[i].weightso[y]+=1;
					//print(&nn[i]);
				}
			}
				
	}
	score_avr+=score[0];
	score_avr/=(n++);
	printf("%f\n\n",score_avr);
	return score[0]-score_avr;
}
int main(void)
{
    NN nn[100];
   for(size_t i = 0; i < 100; i++)
   	init(&nn[i]);
    //printout(&nn);
float input[][3]={  {1.0f,1.0f,1.0f},
                    {1.0f,0.0f,1.0f},
                    {0.0f,1.0f,1.0f},
                    {0.0f,0.0f,0.5f}
                };  
    print(&nn);
    
    //genetick(nn);
    size_t i = 0;
    while(1)
    {
    	i++;
    	if(i==100000)
    		i=0;
    	 for(size_t j = 0; j < 100; j++)
    	 {
    		nn[j].corect[0]=input[i%4][2];
        	nn[j].input[0]=input[i%4][0];
        	nn[j].input[1]=input[i%4][1];
		 }
        if(genetick(nn)<0.01&&genetick(nn)>0)
        //if(i==10000)
		{
        	print(&nn[0]);
        	break;
		}


    }
    
      while(1)
    {
        
        printf("\n\n");
        //print(&nn[0]);  
        printf("Unesite ulaz\n");
        scanf("%f %f",&nn[0].input[0],&nn[0].input[1]);
        forward(&nn);
        printf("out %f\n",nn[0].out[0]);

    }
    
    return 0;
}

