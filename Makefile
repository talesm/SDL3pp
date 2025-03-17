ORIGINAL_SDL_HEADERS := ${wildcard external/SDL/include/SDL3/*.h}
ORIGINAL_HEADER_NAMES := $(patsubst external/SDL/include/SDL3/%,%,$(ORIGINAL_SDL_HEADERS))
TARGET_HEADERS := ${wildcard include/SDL3pp/*.h}

all: scripts/source.json scripts/target.json update

clean:
	rm -r scripts/source.json scripts/target.json

scripts/source.json: ${ORIGINAL_HEADERS}
	node scripts/ parse -d ./external/SDL/include/SDL3/ -d ./external/SDL_image/include/SDL3_image/ scripts/config.json $^

scripts/target.json: scripts/source.json scripts/transform.json
	node scripts/ transform scripts/config.json

update: scripts/target.json
	node scripts/ update scripts/config.json
