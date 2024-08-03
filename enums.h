#ifndef __ENUMS_H__
#define __ENUMS_H__

/* Tipo "Character" 
 * Tipo enumerável que define constantes para cada
 * personagem do jogo. */
typedef enum { 
   RYU,
   KEN
} Character;

/* Define constantes para cada direção de movimento do jogador */
typedef enum { 
   UP, 
   DOWN, 
   LEFT, 
   RIGHT 
} Direction;

/* Tipo "Movement" 
 * Tipo enumerável que define o movimento que está
 * sendo realizado pelo jogador no instante. */
typedef enum {
   IDLE,
   WALK_POSITIVE,
   WALK_NEGATIVE,
   JUMP,
   CROUCH,
   ATTACK_MP,
   ATTACK_LP,
   ATTACK_HP,
   ATTACK_MK,
   ATTACK_LK,
   ATTACK_HK,
   ATTACK_SP
} Movement;

#endif