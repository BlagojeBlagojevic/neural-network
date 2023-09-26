#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#define Nin 2
#define Nout 1
#define Nneuron 3
#define Nlayer 2 //Number of a hiden layer
#define SEED   10
#define lr 0.1

typedef struct NN
{

    float weightso[Nneuron*Nout];
    float weightsi[Nneuron*Nin];
    float weightsi_layer[Nlayer][Nneuron*Nneuron];
    //float weightso_layer[Nlayer][Nneuron*Nneuron];
    float neuron_layer[Nlayer][Nneuron];
    float neuron[Nneuron];   //Midle storage  
    float biasi[Nneuron*Nin];
    float biaso[Nneuron*Nout];
    float input[Nin];
    float out[Nout];
    float corect[Nout];

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
	srand(SEED);

	for (size_t i = 0; i < Nneuron*Nneuron; i++)
		for(size_t j = 0; j < Nlayer; j++)
        nn->weightsi_layer[j][i]=RandF();

	for (size_t i = 0; i < Nneuron; i++)
		for(size_t j = 0; j < Nlayer; j++)
			nn->neuron_layer[j][i] = 0.0f;   	
		


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
    //if(x>0) return x; else return 0;
    return 1.0f/(1.0f+exp(-1.0*x));
    //return tanhf(x);
    //return sin(x);
    //return fabsf(x);
} 
void forward(NN *nn)   //done
{
	// THRU INPUT WEIGHTS
    int prescaler=0;
       for (size_t i = 0; i < Nneuron; i++)  //weights    ulaz
    {
        size_t j = 0;
        nn->neuron_layer[0][i]=0.0f;
        for (j = 0; j < Nin ; j++)  // od prijasnjeg
        	nn->neuron_layer[0][i]+=(nn->weightsi_layer[0][j+prescaler]*nn->input[j]);
        //nn->neuron[i]+=nn->biasi[i];
        nn->neuron_layer[0][i]=activation(nn->neuron_layer[0][i]);
        prescaler+=j;
    }
   

    
    //THRU NETWORK OF NEURON
    for(size_t y = 1 ; y < Nlayer ; y++){
	
    for (size_t i = 0; i < Nneuron; i++)  //weights    ulaz
    {
        size_t j = 0;
        nn->neuron_layer[y][i]=0.0f;
        for (j = 0; j < Nneuron ; j++)  // od prijasnjeg
        	nn->neuron_layer[y][i]+=(nn->weightsi_layer[y][j+prescaler]*nn->neuron_layer[y-1][j]);
        //nn->neuron[i]+=nn->biasi[i];
        nn->neuron_layer[y][i]=activation(nn->neuron_layer[y][i]);
        prescaler+=j;
        //printf("in %d\n",prescaler);
         //printf("%d %f\n",i,nn->neuron_layer[y][i]);
        //system("pause");

    }
    prescaler=0;
	
	}
    // GO THRU OUT LAYER
    for (size_t i = 0; i < Nout; i++)  //weights    ulaz
    {
        size_t j=0;
        nn->out[i]=0.0f;
        for (j = 0; j < Nneuron ; j++)  // od prijasnjeg
            nn->out[i]+=(nn->weightso[j+prescaler]*nn->neuron_layer[Nlayer-1][j]);
        //nn->out[i]+=nn->biaso[i];
        nn->out[i]=activation(nn->out[i]);
        prescaler+=j;
        
        //printf("out %f\n",nn->out[i]);
    }

}
 


void back(NN *nn)
{
      float sum=0.0f,sumb=0.0f;
      int prescaler_in=0,prescaler_out=0;
      for(size_t y = Nlayer-1; y > -1; y--){
	  
      for (size_t i = 0; i < Nneuron; i++)//go thru neurons
      {
          size_t j = 0;
          if(y==Nlayer-1)
          for (j = 0; j < Nout; j++)   //adjustment in out layer
          {
              const float derror=nn->out[j]-nn->corect[j];
              const float dsigmoid=nn->out[j]*(1.0f-nn->out[j]); 
              sum+=lr*derror*dsigmoid*nn->weightso[j+prescaler_in];
              sumb+=lr*derror*dsigmoid*nn->biaso[j+prescaler_in];
              nn->weightso[j+prescaler_in]-=lr*derror*dsigmoid*nn->neuron[i];
              //nn->biaso[j+prescaler_in]-=lr*derror*dsigmoid*nn->neuron[i];
		}
     
          for (size_t k = 0; k < Nneuron; k++)  //adjustment in input layer
          {
              const float derror=nn->neuron_layer[y][k]-sum;
              const float dsigmoid=nn->neuron_layer[y][k]*(1.0f-nn->neuron_layer[y][k]); 
              sum+=lr*derror*dsigmoid*nn->weightsi_layer[y][k];
              //sumb+=lr*derror*dsigmoid*nn->biaso[j+prescaler_in];
              const float hv=nn->neuron_layer[y][i]*(1.0f-nn->neuron_layer[y][i]);
              nn->weightsi_layer[y][k+prescaler_out]-=lr*sum*hv*nn->neuron_layer[y][k];
          }
          

          
          
          //input
          size_t k = 0;
          if(y==0)
          for (k = 0; k < Nin; k++)  //adjustment in input layer
          {
            const float hv=nn->neuron_layer[Nlayer-1][i]*(1.0f-nn->neuron_layer[Nlayer-1][i]);
             nn->weightsi[k+prescaler_out]-=lr*sum*hv*nn->input[k];
             //nn->biaso[j+prescaler_out]-=lr*sumb*hv*nn->input[k];
          }
          prescaler_in+=j;
          prescaler_out+=k;       
      }
     
	//sum=0.0f;
	//sumb=0.0f;      
}
		prescaler_in=0;
        prescaler_out=0;
}

void backpropagaton(NN *nn)
{
	size_t counter=0;
	size_t x = 0;
	float sum = 0.0f,suma=0.0f;
	//FIRST OUT LAYER
	for(size_t y = 0; y < Nneuron;y++){
		for(x = 0; x < Nout; x++){
			  const float derror=nn->out[x]-nn->corect[x];
              const float dsigmoid=nn->out[x]*(1.0f-nn->out[x]);
              nn->weightso[counter+x]-=lr*derror*dsigmoid*nn->neuron_layer[Nlayer-1][y];
              sum+=lr*derror*dsigmoid*nn->weightso[x+counter];
		}
		counter+=x;
	}
	//all oder layers
	for(int i = Nlayer-1; i >= 0 ;i--){counter=0;
		for(size_t y = 0; y < Nneuron;y++){
				for(x = 0; x < Nneuron; x++){
						  const float derror=nn->neuron_layer[i][x]-sum;
              			  const float dsigmoid=nn->neuron_layer[i][x]*(1.0f-nn->neuron_layer[i][x]);
              			  nn->weightsi_layer[i][counter+x]-=lr*derror*dsigmoid*nn->neuron_layer[i][y];
                          suma+=lr*derror*dsigmoid*nn->weightso[x+counter];
                          //system("pause");
		}
		counter+=x;
		sum=suma;
		suma=0.0f;		
				}
				
		}
		counter=0;
    
      }
		
//}
	
	//

void print(NN *nn)
{
    forward(nn);
    printf("\n");
    for (size_t i = 0; i < Nout*Nneuron; i++)
        printf("wo %f ",nn->weightso[i]);
    printf("\n");
	for (size_t y = 0; y < Nlayer; y++){
	
	for (size_t i = 0; i < Nneuron*Nneuron; i++)
        printf("wi %f ",nn->weightsi_layer[y][i]);
    printf("\n");
     }

        
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


int main(void)
{
    NN nn;
    init(&nn);
    print(&nn);
    //printout(&nn);
float input[][3]={  {1.0f,1.0f,1.0f},
                    {1.0f,0.0f,1.0f},
                    {0.0f,1.0f,1.0f},
                    {0.0f,0.0f,0.0f}
                };  
    print(&nn);
    for(size_t i =0; i<100000; i++)
    {
        nn.corect[0]=input[i%4][2];
        nn.input[0]=input[i%4][0];
        nn.input[1]=input[i%4][1];
        forward(&nn);
        //back(&nn);
        backpropagaton(&nn);
    }
    print(&nn);
    while(1)
    {
        
          
        printf("Unesite ulaz\n");
        scanf("%f %f",&nn.input[0],&nn.input[1]);
        forward(&nn);
        printf("out %f\n",nn.out[0]);

    }
    return 0;
}
