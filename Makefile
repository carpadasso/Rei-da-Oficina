# ---------------------------------
# Projeto Street Fighter
# Leonardo Carpwiski  - GRR20232331
# ---------------------------------

# Nome do Projeto
PROJECT = StreetFighter

# Arquivos .c
C_SOURCE = $(wildcard *.c)

# Arquivos .h
H_SOURCE = $(wildcard *.h)

# Objetos
OBJ = $(C_SOURCE:.c=.o)

# Compilador
CC = gcc

# Flags do Compilador
CC_FLAGS= -c -Wall -Wextra -O2

# Bibliotecas
LIBS = -L/usr/lib -lallegro -lallegro_main -lallegro_font -lallegro_ttf \
                  -lallegro_primitives -lallegro_image -lallegro_audio  \
						-lallegro_acodec -lallegro_color

# ----------------------------

# all: Compila o projeto
all: $(PROJECT)

$(PROJ_NAME): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CC_FLAGS) -o $@ $^ $(LIBS)

# clean: Remove os arquivos objeto e temporários
clean:
	rm -rf *.o $(PROJ_NAME) *~