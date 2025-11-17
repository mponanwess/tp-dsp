
#ifndef MUSIQUE_H_
#define MUSIQUE_H_

#define TAILLE_MUSIQUE 	33

typedef struct
{
	uint32_t freqNote;
	float dureeNote;
} Note;

extern Note musique[TAILLE_MUSIQUE];

#endif /* MUSIQUE_H_ */
