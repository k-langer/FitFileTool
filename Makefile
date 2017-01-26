CC=gcc
#OPT=-Ofast -ffp-contract=off 
CFLAGS=-std=gnu99 -Wno-unused-result -lm
WARNING=-Wall -Wformat=0
TARGET=Addpower
API=fit_api/
HEADERS=headers/
UNDEF=FIT_USE_STDINT_H

OBJDIR=obj
all: $(TARGET)
OBJS = obj/addpower.o \
    obj/filetools.o \
	obj/fit.o \
	obj/fit_convert.o \
	obj/fit_crc.o \
	obj/fit_product.o \
	obj/fit_ram.o 

$(OBJDIR)/%.o: %.c 
	$(CC) -c $(CFLAGS) $(OPT) -I $(HEADERS) -I $(API) $(WARNING) -o $@ $<
$(OBJDIR)/%.o: $(API)/%.c 
	$(CC) -c $(CFLAGS) $(OPT) -u $(UNDEF) -I $(API) $(WARNING) -o $@ $<

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

run: 
	./$(TARGET) workouts/one.fit 
clean:  
	rm -f *.o obj/*.o $(TARGET) test*fit PowerAdd*fit
