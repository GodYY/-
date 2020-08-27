EXE = k_merge replace_selection

k_merge:
	gcc -DKeyType=int \
	k_merge.c \
	losertree.c \
	-o k_merge

replace_selection:
	gcc replace_selection.c \
	-o replace_selection

all: ${EXE}

clean:
	rm -rf ${EXE}