CC := lcc
CFLAGS += -msm83:gb

SOURCE := ./src
BUILD := ./build


src_c := $(wildcard ${SOURCE}/*.c)
obj_c := $(patsubst ${SOURCE}/%.c, ${BUILD}/%.o, ${src_c})
bin := out.gb

.PHONY: help echo build clean

help:
	@echo "Targets:"
	@echo "  help  - Print this message"
	@echo "  echo  - Print all variables of the makefile"
	@echo "  build - Build the project"
	@echo "  clean - Remove all build files"

echo:
	@echo "CC: ${CC}"
	@echo "PORT: ${PORT}"
	@echo "SOURCE: ${SOURCE}"
	@echo "BUILD: ${BUILD}"
	@echo "src_c: ${src_c}"
	@echo "obj_c: ${obj_c}"

build: ${bin}

clean:


${bin}: ${obj_c}
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} $^ -o $@

${obj_c}: ${BUILD}/%.o : ${SOURCE}/%.c
	@mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c $^ -o $@
