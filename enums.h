#ifndef __ENUMS_H__
#define __ENUMS_H__

/* Define o Tipo Enumerável CHARACTER, que estabelece
 * identificadores para cada personagem disponível. */
typedef enum { 
   RYU,
   KEN
} Character;

/* Define o Tipo Enumerável MOVEMENT, que estabelece
 * identificadores para cada movimento possível de um JOGADOR. */
typedef enum {
   /* Movimentos Padrão */
   IDLE, WALK_POSITIVE, WALK_NEGATIVE,

   /* Pular e Agachar */
   JUMP, CROUCH,

   /* Ataques - Soco, Chute e Especial */
   ATTACK_MP, ATTACK_LP, ATTACK_HP,
   ATTACK_MK, ATTACK_LK, ATTACK_HK,
   ATTACK_SP
} Movement;

#endif