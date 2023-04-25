//
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#define RAND_MAX 1 
   // mora biti broj razlicit u skrivenim slojevima
double MAX_NEURONA=0;        //MAKSIMALNI BROJ NEURONA
typedef struct neuron{                //POKAZUJE VRIJEDNOST SA CIM TREBA POMNOZITI TEZINSKE KOEFICIJENTE
	double *TEZINSKI_FAKTORI,*VRIJEDNOSTI_PRIJASNIH_NEURONA; // POKAZUJU VRIJEDNOSTI KONEKCIJA TREBALO BI SE POMNOZITI SA VRIJEDNOTIMA PRIJASNJIH NEURONA
	double PRIPADNOST;        //POKAZIVATI VRIJEDNOST KOJU TREBA DODATI NA NEURON DA BI BIO AKTIVAN U OKVIRU(0 DO 1) MOZDA;	
	double VRIJEDNOST;        //TRENUTNA VRIJEDNOT NEURONA 
	double OD_KOLIKO_NEURONA_JE_NASLIJEDIO;  // OD KOLIKO NEURONA NASLEDJUJE VEZU TREBALO BI SE KORISTITI ZA INICIJALIZACIJU VRIJEDNOTI NIZA I ELEMENATA
	double UKUPNO;
	double SLOJ;
}NEURONI;

double Random_inicijalizacija_tezinskih_koeficijenata(double OD_KOLIKO_NEURONA_JE_NASLIJEDIO);

// FUNKCIJA IZNAD SE KORITI ZA POSTAVLJANJE RANDOM VRIJEDNOSTI KOEFICIJENATA UNUTAR NEURONA
//FUNKCIJA ISPOD POSTALJA VRIJEDNOSTI NEURONA I SLUZI ZA INICIJALIZACIJU MREZE
NEURONI* Inicijalizacija_Mreze_Neurona(void);
double Racunanje_Vrijednosti_Neurona(NEURONI neuron);  //RACUNA VRIJEDNOST NEURONA
double sigmoid(double Vrijednost);     //SIGMOID FUNKCIJA SLUZI ZA SMANJIVANJE VRIJEDNOSTI OD 0 DO 1  
double derivat_sigmoid(double Vrijednost);
NEURONI* PROPAGACIJA_RACUNAJE(NEURONI *neuron);
//OVO BI TREBALO DA RACUNA VRIJEDNOST GRESKE I DA MIJENJA TEZINSE KOEFICIJENTE NA SA IZLAZA
NEURONI* PROMJENA_NEURONA_U_ODNOSU_NA_ULAZ_I_IZLAZ(NEURONI *neuron);// VIDJETI STA TREBA DOVETI NA ULAZC PROLJEDITI FUKCIJI
// OVO BI TREBALO DA MIJENJA VRIJEDNOST NEURONA U ODNOS NA ULAZ ZAVISNO DA LI CE ULAZ BITI FAJL ILI MATRICA(VIDJECEMO)
   
int main()
{
	Random_inicijalizacija_tezinskih_koeficijenata(3.0);
	return 0;
}
double Random_inicijalizacija_tezinskih_koeficijenata(double OD_KOLIKO_NEURONA_JE_NASLIJEDIO)
{
	int i=0;
	NEURONI neuron;
	time_t t;srand((unsigned) time(&t)); //INICIJALIZACIJA TEZINSKIH KOEFICIJENATA
	neuron.TEZINSKI_FAKTORI = (double*)malloc(OD_KOLIKO_NEURONA_JE_NASLIJEDIO * sizeof(double));
	for(i=0;i<OD_KOLIKO_NEURONA_JE_NASLIJEDIO;i++)
	{
		neuron.TEZINSKI_FAKTORI[i]=rand();
		if(neuron.TEZINSKI_FAKTORI[i]<=100&&neuron.TEZINSKI_FAKTORI>=0)
		{
			neuron.TEZINSKI_FAKTORI[i]=neuron.TEZINSKI_FAKTORI[i]/100;
		}
		else;
		if(neuron.TEZINSKI_FAKTORI[i]<=1000&&neuron.TEZINSKI_FAKTORI[i]>100)
		{
			neuron.TEZINSKI_FAKTORI[i]=neuron.TEZINSKI_FAKTORI[i]/1000;
		}else;
		if(neuron.TEZINSKI_FAKTORI[i]<=10000&&neuron.TEZINSKI_FAKTORI[i]>1000)
		{
			neuron.TEZINSKI_FAKTORI[i]=neuron.TEZINSKI_FAKTORI[i]/10000;
		}else;
		if(neuron.TEZINSKI_FAKTORI[i]<=32700&&neuron.TEZINSKI_FAKTORI[i]>10000)
		{
			neuron.TEZINSKI_FAKTORI[i]=neuron.TEZINSKI_FAKTORI[i]/32700;
		}else;
		//printf("random koeficijent je:%f\n",neuron.TEZINSKI_FAKTORI[i]);
	}
	return neuron.TEZINSKI_FAKTORI[i];
}
NEURONI* Inicijalizacija_Mreze_Neurona(void)
{
	NEURONI *neuron=NULL;
	neuron=(NEURONI*)malloc(0 * sizeof(NEURONI));
	int Broj_Neurona,Broj_Slojeva,i,j=0,k=0,n,prijasnji_nivo=0;
	printf("Unesite Broj slojeva unutar neuralne mreze\n\n");
	scanf("%d",&Broj_Slojeva);
	for(i=0;i<Broj_Slojeva;i++)
	{
		
		
		printf("Unesite Broj neurona unutar %d sloja  neuralne mreze\n\n",i+1);
		scanf("%d",&Broj_Neurona);
		MAX_NEURONA+=Broj_Neurona;
		neuron = (NEURONI *) realloc(neuron, sizeof(NEURONI)*Broj_Neurona*Broj_Slojeva+sizeof(NEURONI)*5);
		if(i!=0&&i!=Broj_Slojeva-1)
		{
			for(j=1;j<Broj_Neurona;j++)
			{
				neuron[i*k+j].VRIJEDNOST=0.0;
				neuron[i*k+j].OD_KOLIKO_NEURONA_JE_NASLIJEDIO=prijasnji_nivo; // INICIJALIZACIJA
				neuron[i*k+j].TEZINSKI_FAKTORI=(double*)malloc(prijasnji_nivo * sizeof(double));
				neuron[i*k+j].VRIJEDNOSTI_PRIJASNIH_NEURONA=(double*)malloc(prijasnji_nivo * sizeof(double));
				neuron[i*k+j].UKUPNO=Broj_Neurona;
				neuron[i*k+j].SLOJ=Broj_Slojeva;
				for(n=0;n<prijasnji_nivo;n++)
				{
					*(neuron[i*k+j].TEZINSKI_FAKTORI+n)=Random_inicijalizacija_tezinskih_koeficijenata(1.0);
						//Inicijalizacija random tezinskih koeficijenata
					*(neuron[i*k+j].VRIJEDNOSTI_PRIJASNIH_NEURONA+n)=0.0; //Inicijalizacija prijanih nivoa
					
				}	
			}
		}else;
		if(i==0||i==Broj_Slojeva-1) //Postavljanje ulaznih neurona postavljanje na nulu j radi u prvom slucaju 0 u drugom 1 trebala bi matrica 
		{
			for(j=0;j<Broj_Neurona;j++)
			{
				neuron[i*k+j].VRIJEDNOST=0.0;
				neuron[i*k+j].OD_KOLIKO_NEURONA_JE_NASLIJEDIO=0;
				neuron[i*k+j].TEZINSKI_FAKTORI=(double*)malloc(0 * sizeof(double));
				neuron[i*k+j].UKUPNO=Broj_Neurona;
				neuron[i*k+j].SLOJ=Broj_Slojeva;
			}
			k+=j;
			prijasnji_nivo=Broj_Neurona;
		}	
	}
	return neuron;
	
}
double sigmoid(double Vrijednost)
{
	return 1/(1+exp(-Vrijednost));
}
double derivat_sigmoid(double Vrijednost)
{
	return exp(-Vrijednost)/pow(exp(-Vrijednost)+1,2);
}
double Racunanje_Vrijednosti_Neurona(NEURONI neuron)
{
	int i=0,j=0,k=0;
	double Vrijednost=0.0;
	for(i=0;i<neuron.OD_KOLIKO_NEURONA_JE_NASLIJEDIO;i++)
		Vrijednost+=neuron.VRIJEDNOSTI_PRIJASNIH_NEURONA[i]*neuron.TEZINSKI_FAKTORI[i];
			return Vrijednost*sigmoid(Vrijednost);
}
 

NEURONI* PROMJENA_NEURONA_U_ODNOSU_NA_ULAZ_I_IZLAZ(NEURONI *neuron)
{
	int Broj_neurona=0,j=0,k=0,n=0,Broj_ulaznih_neurona,Pamti_Broj_Neurona=0,i=0;    //UNESEMO ULAZNE VRIJEDNOSTI  TREBALO BI VIDJETI KOLIKO ULAZNIH NEURONA IMAMO UNOSITI 
	double Ulazni_Niz_za_Neurone[Broj_ulaznih_neurona],Proizvod=1,sum=0,GRESKA;   
	for(Broj_neurona=0;Broj_neurona<Broj_ulaznih_neurona;)
	{
		neuron[Broj_neurona].VRIJEDNOST=Ulazni_Niz_za_Neurone[Broj_neurona];  //INICIJALIZACIJA ULAZNIH NEURONA
		Broj_neurona++;
	}
		for(i=Broj_neurona;i<Broj_neurona+neuron[i].UKUPNO*neuron[i].SLOJ;)
		{
			for(j=0;j<neuron[i].VRIJEDNOST;)
			{
				Proizvod*=(neuron[i].VRIJEDNOSTI_PRIJASNIH_NEURONA[j]*neuron[i].TEZINSKI_FAKTORI[j]);
				sum+=Proizvod;
				Proizvod=1;
			}
			neuron[i].VRIJEDNOST=sigmoid(neuron[i].VRIJEDNOST);
			sum=0;
			i++;
			Broj_neurona++;
			if(Broj_neurona>=MAX_NEURONA)
			{
				break;
			}	
		}
		//ULAZNA PROPAGACIJA 
		//NE RADI 100%  cudo RADI DOSTA
		
		for(i=MAX_NEURONA-1,j=Broj_ulaznih_neurona-1;i>0;i--,j--)
		{
		
			GRESKA=(neuron[i].VRIJEDNOST-Ulazni_Niz_za_Neurone[j]);
			GRESKA*=sigmoid(GRESKA);    //RACUNA GRESKU NA IZLAZU
			if(i==0)
			break;
			else;
			
		}
		
}
	


	
	
	


