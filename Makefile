BINARY=a.out # name of the binary
CODEDIRS=. src/renderer src/engine src/inputSystem \
		 src/entity src/components libs/SDL_IMGUI/ libs/SDL_IMGUI/backends \
		src/components/camera src/components/ia src/components/sprites \
		src/components/collision src/uiSystem src/collisionSystem \
		src/utils src/math src/fileSystem/
		 # where are the .c/.cpp?
INCDIRS= scr/utils/ src/math src/renderer src/engine src/inputSystem\
		 src/entity src/components libs/SDL_IMGUI/ libs/SDL_IMGUI/backends \
		 src/components/camera src/components/ia src/components/sprites \
		 src/components/collision src/uiSystem src/collisionSystem \
		 src/fileSystem/
		 # where are the .h/.hpp?

LINKERFLAGS = -lSDL2 -lSDL2_image -lGL -lGLEW -lSOIL -lSDL2_ttf
CC=g++
OPT=-O2
# generate files that encode make rules for the .h dependencies
DEPFLAGS=-MP -MD
# automatically add the -I onto each include directory
CFLAGS= -fopenmp -std=c++17 -Wall -Wextra -g $(foreach D,$(INCDIRS),-I$(D)) $(OPT) $(DEPFLAGS) 

# for-style iteration (foreach) and regular expression completions (wildcard)
CFILES=$(foreach D,$(CODEDIRS),$(wildcard $(D)/*.cpp))
# regular expression replacement
OBJECTS=$(patsubst %.cpp,%.o,$(CFILES))
DEPFILES=$(patsubst %.cpp,%.d,$(CFILES))

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^ $(LINKERFLAGS)

# only want the .c file dependency here, thus $< instead of $^.
#
%.o:%.cpp
	$(CC) $(CFLAGS) -c -o $@ $< $(LINKERFLAGS)

clean:
	rm -rf $(BINARY) $(OBJECTS) $(DEPFILES)

# shell commands are a set of keystrokes away
distribute: clean
	tar zcvf dist.tgz *

# @ silences the printing of the command
# $(info ...) prints output
diff:
	$(info The status of the repository, and the volume of per-file changes:)
	@git status
	@git diff --stat

# include the dependencies
-include $(DEPFILES)

# add .PHONY so that the non-targetfile - rules work even if a file with the same name exists.
.PHONY: all clean distribute diff

