all: KingOfWorkshop

KingOfWorkshop:
	gcc -Wall main.c player.c -o KingOfWorkshop -L. -lliballegro_main -lliballegro_font