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

void passThrough(void){
	/* Reception des échantillons d'entrée */
	HAL_SAI_Receive (&hsai_BlockB2,(uint8_t *)&echInputLeft,1,SAI_WAIT);	// Left
	HAL_SAI_Receive (&hsai_BlockB2,(uint8_t *)&echInputRight,1,SAI_WAIT);	// Right

    /* Calcul de l'écho pour le canal gauche */
    int16_t echoLeft = bufferInputLeft[pos];  // Prendre l'échantillon à retard du canal gauche
    echOutputLeft = echInputLeft + (int16_t)(ALPHA * echoLeft);  // Ajout de l'écho sur le signal gauche

    /* Calcul de l'écho pour le canal droit */
    int16_t echoRight = bufferInputRight[pos];  // Prendre l'échantillon à retard du canal droit
    echOutputRight = echInputRight + (int16_t)(ALPHA * echoRight); // Ajout de l'écho sur le signal droit

    /* Envoi des échantillons de sortie pour chaque canal */

	/*  Output = Input */
	// echOutputLeft = echInputLeft;
	// echOutputRight = echInputRight;

	/* Envoi des échantillons de sortie */
	HAL_SAI_Transmit(&hsai_BlockA2,(uint8_t *)&echOutputLeft,1,SAI_WAIT);	// Left
	HAL_SAI_Transmit(&hsai_BlockA2,(uint8_t *)&echOutputRight,1,SAI_WAIT);	// Right

    /* Mise à jour des buffers circulaires pour chaque canal */
	bufferInputLeft[pos] = echInputLeft;   // Stocke l'échantillon d'entrée dans le buffer gauche
	bufferInputRight[pos] = echInputRight; // Stocke l'échantillon d'entrée dans le buffer droit

    /* Mise à jour de la position dans les buffers circulaires */
    pos = (pos + 1) % BUFFER_SIZE_INPUT;
}


int main(void)
{
	SCB_EnableICache();
	SCB_EnableDCache();
	HAL_Init();
	BOARD_Init();

	while(1){
		passThrough();
	}

}


