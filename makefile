SOURCE := ./src
BUILD := ./build

CC := lcc
CFLAGS += -msm83:gb -Wa-l -Wb-yt0x00 -Wf--opt-code-speed "-Wf-I${SOURCE}/include"


src_c := $(wildcard ${SOURCE}/*.c) $(wildcard ${SOURCE}/*/*.c)
obj_c := $(patsubst ${SOURCE}/%.c, ${BUILD}/%.o, ${src_c})
bin := out.gb

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
	rm -f ${obj_c}


${bin}: ${obj_c}
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} $^ -o $@

${obj_c}: ${BUILD}/%.o : ${SOURCE}/%.c
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c $^ -o $@
