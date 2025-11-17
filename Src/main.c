#include "main.h"
#include "mx_init.h"
#include "filter_Coeff.h"
#include "musique.h"

#define AUDIOFREQ_16K 		((uint32_t)16000U)  //AUDIOFREQ_16K = 16 Khz
#define BUFFER_SIZE_INPUT	4000
#define BUFFER_SIZE_OUTPUT	4000
#define BUFFER_SIZE_SINUS 	16000
#define BUFFER_SIZE_AUDIO	16000
#define AMPLITUDE			300
#define SAI_WAIT			100
#define TIME_START			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET)
#define TIME_STOP			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET)
#define PI					3.141592

#define ALPHA 0.6 // Facteur d'écho à 60%


extern SAI_HandleTypeDef hsai_BlockA2;
extern SAI_HandleTypeDef hsai_BlockB2;
extern TIM_HandleTypeDef htim3;

int16_t sinusTable[BUFFER_SIZE_SINUS]  = { 0 };
int16_t audioTable[BUFFER_SIZE_SINUS]  = { 0 };
int16_t bufferInputRight[BUFFER_SIZE_INPUT] = { 0 };
int16_t bufferInputLeft[BUFFER_SIZE_INPUT]  = { 0 };
int16_t bufferOutputRight[BUFFER_SIZE_OUTPUT] = { 0 };
int16_t bufferOutputLeft[BUFFER_SIZE_OUTPUT] = { 0 };
int16_t echInputLeft   = 0;
int16_t echInputRight  = 0;
int16_t echOutputRight = 0;
int16_t echOutputLeft  = 0;

// Définir la position du buffer circulaire
int16_t pos = 0;






void passThrough(void);
uint32_t calculNbEchPeriod(uint32_t);
uint32_t calculNbEchNote(float);
void notePlayClassic(uint32_t, float);
void musicPlay(Note *);
void initSinusTable(void);
void musicPlayDDS(Note *);

void passThrough(void){
	/* Reception des échantillons d'entrée */
	HAL_SAI_Receive (&hsai_BlockB2,(uint8_t *)&echInputLeft,1,SAI_WAIT);	// Left
	HAL_SAI_Receive (&hsai_BlockB2,(uint8_t *)&echInputRight,1,SAI_WAIT);	// Right

    /* Calcul du reberb du canal gauche */
    int16_t echoLeft = bufferOutputLeft[pos];  // Prendre l'échantillon à retard du canal gauche
    echOutputLeft = echInputLeft + (int16_t)(ALPHA * echoLeft);  // Ajout de l'écho sur le signal gauche

    /* Calcul de l'écho pour le canal droit */
    int16_t echoRight = bufferOutputRight[pos];  // Prendre l'échantillon à retard du canal droit
    echOutputRight = echInputRight + (int16_t)(ALPHA * echoRight); // Ajout de l'écho sur le signal droit

    /* Envoi des échantillons de sortie pour chaque canal */

	/*  Output = Input */
	// echOutputLeft = echInputLeft;
	// echOutputRight = echInputRight;

	/* Envoi des échantillons de sortie */
	HAL_SAI_Transmit(&hsai_BlockA2,(uint8_t *)&echOutputLeft,1,SAI_WAIT);	// Left
	HAL_SAI_Transmit(&hsai_BlockA2,(uint8_t *)&echOutputRight,1,SAI_WAIT);	// Right

    /* Mise à jour des buffers circulaires pour chaque canal */
	bufferOutputLeft[pos] = echOutputLeft;   // Stocke l'échantillon d'entrée dans le buffer gauche
	bufferOutputRight[pos] = echOutputRight; // Stocke l'échantillon d'entrée dans le buffer droit

    /* Mise à jour de la position dans les buffers circulaires */
    pos = (pos + 1) % BUFFER_SIZE_INPUT;
}


uint32_t calculNbEchPeriod(uint32_t frequence) {
	return (1 / frequence) * AUDIOFREQ_16K;
}

uint32_t calculNbEchNote(float duree) {
	return duree * AUDIOFREQ_16K;
}

void notePlayClassic(uint32_t frequence, float duree) {

	uint32_t NbEchNote = calculNbEchNote(duree);


	for(uint32_t n = 0; n < NbEchNote; n++)
	{

		int16_t sample = (int16_t)(AMPLITUDE * sin(2.0f * PI * frequence * n / AUDIOFREQ_16K)); // sinus mis à l'échelle audio

		HAL_SAI_Transmit(&hsai_BlockA2, (uint8_t*)&sample, 1, SAI_WAIT); // gauche
		HAL_SAI_Transmit(&hsai_BlockA2, (uint8_t*)&sample, 1, SAI_WAIT); // droite
	}
}

void musicPlay(Note *t) {
    for(uint32_t i = 0; i < TAILLE_MUSIQUE; i++) {
        notePlayClassic(t[i].freqNote, t[i].dureeNote);
    }
}

void initSinusTable(void) {
	for(uint32_t i = 0; i < BUFFER_SIZE_SINUS; i++){
		sinusTable[i] = (int16_t)(AMPLITUDE * sinf(2.0f * M_PI * i / BUFFER_SIZE_SINUS));
	}
}

void notePlayDDS(uint32_t frequence, float duree) {
    uint32_t NbEchNote = (uint32_t)(duree * AUDIOFREQ_16K);   // nombre d'échantillons
    double phase = 0;
    double phaseIncrement = (double)BUFFER_SIZE_SINUS * frequence / AUDIOFREQ_16K;

    for(uint32_t n = 0; n < NbEchNote; n++) {
        int16_t sample = sinusTable[(int)phase % BUFFER_SIZE_SINUS];
        HAL_SAI_Transmit(&hsai_BlockA2, (uint8_t*)&sample, 1, SAI_WAIT); // gauche
        HAL_SAI_Transmit(&hsai_BlockA2, (uint8_t*)&sample, 1, SAI_WAIT); // droite
        phase += phaseIncrement;
    }
}

void musicPlayDDS(Note *t) {
	for(uint32_t i = 0; i < TAILLE_MUSIQUE; i++) {
		notePlayDDS(t[i].freqNote, t[i].dureeNote);
	}
}


int main(void)
{
	SCB_EnableICache();
	SCB_EnableDCache();
	HAL_Init();
	BOARD_Init();

	//Note n;

	//uint32_t NbEchPeriod;

	Note musique[TAILLE_MUSIQUE]=
	{{494,0.3},{659,0.45},{784,0.3},{740,0.3},{659,0.6},{988,0.3},{880,0.9},{740,0.9},{659,0.45},{784,0.3},
			{740,0.3},{622,0.6},{698,0.3},{494,0.9},{494,0.9},{494,0.3},{659,0.45},{784,0.3},{740,0.3},{659,0.6},
			{988,0.3},{1174,0.6},{1108,0.3},{1046,0.6},{830,0.3},{1046,0.45},{988,0.3},{932,0.3},{932,0.6},{784,0.3},
			{659,0.9},{659,0.6},{784,0.3}};




	//NbEchPeriod = calculNbEchPeriod(n.freqNote);
	//NbEchNote = calculNbEchNote(n.dureeNote);

	initSinusTable();




	while(1){

	    //passThrough();
		//musicPlay(musique);
		musicPlayDDS(musique);

	}

}


