TARGET = main
CC = gcc
CFLAGS = -Wall

PREF = ./src/
SRC = $(wildcard $(PREF)*.c)

$(PREF)$(TARGET)1 : $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(PREF)$(TARGET)
encode :
	$(PREF)$(TARGET) encode $(PREF)points $(PREF)units

decode:
	$(PREF)$(TARGET) encode $(PREF)points $(PREF)units