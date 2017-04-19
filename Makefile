# Имя проекта
TARGET = itron

# Платформа
MCU ?= attiny13a
F_CPU ?= 4800000
TOOLCHAIN = avr
CC = $(TOOLCHAIN)-gcc
LD = $(TOOLCHAIN)-gcc
OD = $(TOOLCHAIN)-objdump
OC = $(TOOLCHAIN)-objcopy
RM = rm

# Пути поиска исходных файлов
SOURCEDIR := src
SOURCES := $(wildcard $(SOURCEDIR)/*.c)

BUILDDIR = build
OBJECTS = $(patsubst $(SOURCEDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))

# Внешние библиотеки
LIBS  = -lm

EXTRA_CFLAGS = -I/usr/local/Cellar/avr-libs/2.0.0/avr/include

# Настройки компилятора
CFLAGS  = $(EXTRA_CFLAGS)
CFLAGS += -DF_CPU=$(F_CPU)
CFLAGS += -DMCU_STR=$(MCU)
CFLAGS += -mmcu=$(MCU)            # целевой процессор
CFLAGS += -O1                     # оптимизация
CFLAGS += -g                      # добавлять отладочную информация
CFLAGS += -Wall                   # выводить все предупреждения
CFLAGS += -Wstrict-prototypes
CFLAGS += -Winline
CFLAGS += -std=gnu99              # стандарт языка С
CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
CFLAGS += -fno-move-loop-invariants
CFLAGS += -fno-tree-scev-cprop
CFLAGS += -ffunction-sections
CFLAGS += -finline-functions-called-once
CFLAGS += -mcall-prologues
#CFLAGS += -Wa, adhlns=$(@:.o=.al)
CFLAGS += -save-temps=obj

# Настройки линковщика
#  -Wl,... указывает аргумент для линковщика
LDFLAGS  = -Wl,--as-needed -Wl,--gc-sections

all: $(BUILDDIR) $(TARGET).hex $(TARGET).lst size

test: $(SOURCES)
	@echo $(SOURCES)
	@echo $(OBJECTS)

clean:
	$(RM) -rf $(BUILDDIR) $(TARGET).{elf,hex,lst} *~

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# pull in dependency info
-include $(OBJECTS:%.o=%.P)

# Создание .hex файла
$(TARGET).hex: $(TARGET).elf
	@echo "Create HEX $< => $@"
	@$(OC) -O ihex -j .text -j .data -O ihex -R .eeprom -R .fuse -R .lock $< $@

$(TARGET).lst: $(TARGET).elf
	@echo "Create LST $< => $@"
	@$(OD) -h -S $(TARGET).elf > $(TARGET).lst

# Линковка
$(TARGET).elf: $(OBJECTS)
	@echo "Linking $^ => $@"
	@$(LD) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

# Компиляция С
$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c | $(BUILDDIR)
	@echo "Compilling C source $< => $@"
	@$(CC) -c $(CFLAGS) -o $@ $<

# Компиляция ASM
$(BUILDDIR)/%.o: %.s
	@echo "Compilling ASM source =>" $<
	@$(AS) $(AFLAGS) -c $< -o $@


# Сгенерированные gcc зависимости
include $(wildcart *.d)


## Interpret TARGET section sizes wrt different AVR chips
## Usage:  $(call show_size,section-name,section-regex,Atmega168-size-in-k,Atmega328p-size-in-k,Atmega644p-size-in-k,Atmega1280-size-in-k)
define show_size
	@$(OD) -h $^ | \
		perl -MPOSIX -ne \
		'/.($2)\s+([0-9a-f]+)/ && do { $$a += eval "0x$$2" }; \
		END { printf "    %-7s: %5d bytes         %3d%%       %3d%%       %3d%%       %3d%%     %3d%%\n", "$1", $$a, \
			ceil($$a * 100 / ($3 * 1024)), \
			ceil($$a * 100 / ($4 * 1024)), \
			ceil($$a * 100 / ($5 * 1024)), \
			ceil($$a * 100 / ($6 * 1024)),  \
			ceil($$a * 100 / ($7 * 1024))  \
		}'
endef

size: $(TARGET).elf
	@echo "    SIZES         ATmega...  'Tiny13A       '168    '328(P)    '644(P)    '1280"
	$(call show_size,FLASH,text,1,14,30,62,126)
	$(call show_size,RAM,data|bss,0.0625,1,2,4,8)
	$(call show_size,EEPROM,eeprom,0.0625,1,2,2,4)
