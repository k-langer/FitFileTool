CC=gcc
#WIN_CC=x86_64-w64-mingw32-gcc 
#OPT=-Ofast -ffp-contract=off 
CFLAGS=-std=gnu99 -Wno-unused-result -lm
WARNING=-Wall -Wformat=0
TARGET=Addpower
API=fit_api/
HEADERS=headers/
UNDEF=FIT_USE_STDINT_H
#DEBUG=-D DEBUG
DEFINE=

target windows: CC=x86_64-w64-mingw32-gcc
target windows: DEFINE=-D WINDOWS

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
	$(CC) -c $(CFLAGS) $(DEFINE) $(OPT) $(DEBUG) -I $(HEADERS) -I $(API) $(WARNING) -o $@ $<
$(OBJDIR)/%.o: $(API)/%.c 
	$(CC) -c $(CFLAGS) $(DEFINE) $(OPT) -u $(UNDEF) -I $(API) $(WARNING) -o $@ $<

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

windows: $(OBJS)
	$(CC) -o WIN_$(TARGET) $^ $(CFLAGS)

run: 
	./$(TARGET) workouts/one.fit 
clean:  
	rm -f *.o obj/*.o $(TARGET) test*fit PowerAdd*fit WIN_$(TARGET)
