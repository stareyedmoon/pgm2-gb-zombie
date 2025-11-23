SOURCE := ./src
BUILD := ./build

CC := lcc
CFLAGS += -msm83:gb -Wa-l -Wb-yt0x00 -Wf--opt-code-speed "-Wf-I${SOURCE}/include"

ASM := sdasgb

src_c := $(shell find ${SOURCE} -name '*.c')
obj_c := $(patsubst ${SOURCE}/%.c, ${BUILD}/%.o, ${src_c})

src_asm := $(shell find ${SOURCE} -name '*.asm')
obj_asm := $(patsubst ${SOURCE}/%.asm, ${BUILD}/%.o, ${src_asm})

bin := out.gb

resrc_png := $(wildcard ${SOURCE}/resource/*.png)
resrc_png_bin := $(patsubst ${SOURCE}/resource/%.png, ${BUILD}/resource/%.bin, ${resrc_png})
resrc_png_c := $(patsubst ${SOURCE}/resource/%.png, ${BUILD}/resource/%.c, ${resrc_png})
resrc_png_h := $(patsubst ${SOURCE}/resource/%.png, ${SOURCE}/include/resource/%.h, ${resrc_png})
resrc_png_obj := $(patsubst ${SOURCE}/resource/%.png, ${BUILD}/resource/%.o, ${resrc_png})

obj = ${obj_c} ${obj_asm} ${resrc_png_obj}

.PHONY: help echo build clean

help:
	@echo "Targets:"
	@echo "  help  - Print this message"
	@echo "  echo  - Print all variables of the makefile"
	@echo "  build - Build the project"
	@echo "  clean - Remove all build files"
	@echo ""
	@echo "Variables:"
	@echo "  CC     - C Compiler (default: lcc)"
	@echo "  CFLAGS - Flags to pass to the compiler"
	@echo "  SOURCE - Source directory"
	@echo "  BUILD  - Build directory"

echo:
	@echo "CC: ${CC}"
	@echo "CFLAGS: ${CFLAGS}"
	@echo "SOURCE: ${SOURCE}"
	@echo "BUILD: ${BUILD}"
	@echo "src_c: ${src_c}"
	@echo "obj_c: ${obj_c}"

build: ${bin}

clean:
	rm -f ${obj} ${resrc_png_c} ${resrc_png_h} ${resrc_png_bin}


${bin}: ${obj}
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} $^ -o $@

${obj_c}: ${BUILD}/%.o : ${SOURCE}/%.c ${resrc_png_h}
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c $< -o $@

${obj_asm}: ${BUILD}/%.o : ${SOURCE}/%.asm
	@mkdir -p $(dir $@)
	${ASM} -o $@ $<

resrc: ${resrc_png_c} ${resrc_png_h}

${resrc_png_obj}: ${BUILD}/resource/%.o : ${BUILD}/resource/%.c
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c $^ -o $@

${resrc_png_c}: ${BUILD}/resource/%.c : ${BUILD}/resource/%.bin
	@mkdir -p $(dir $@)
	./scripts/bin_to_c.py $^ $@ /dev/null
	
${resrc_png_h}: ${SOURCE}/include/resource/%.h : ${BUILD}/resource/%.bin
	@mkdir -p $(dir $@)
	./scripts/bin_to_c.py $^ /dev/null $@

${resrc_png_bin}: ${BUILD}/resource/%.bin : ${SOURCE}/resource/%.png
	@mkdir -p $(dir $@)
	./scripts/compress_sprite.py $^ $@
