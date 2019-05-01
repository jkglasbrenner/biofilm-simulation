.PHONY: clean env help

#################################################################################
# GLOBALS                                                                       #
#################################################################################

PROJECT_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
PROJECT_NAME = biofilms-sim

RM = rm
COPY = cp
FIND = find

MAKE_SH = ./make.sh

CLANG_FORMAT = clang-format
CLANG_FORMAT_OPTS = -style=file -i

CONDA = conda
CONDA_ENV_FILE = environment.yml

PY ?= python3
PY_FILES = scripts/*.py

PRECOMMIT = pre-commit

BLACK = black
BLACK_OPTS = -t py37

SRC_FILES = src/*.cpp src/*.hpp

SUBDIRS = bin build

CLEAN_FILES = bin/ build/

#################################################################################
# FUNCTIONS                                                                     #
#################################################################################

define python_black
    $(BLACK) $(BLACK_OPTS) $(PY_FILES)
endef

define cleanup
    $(FIND) -name "__pycache__" -type d -exec $(RM) -rf {} +
    -$(RM) -rf $(CLEAN_FILES)
endef

define clang_format
    $(CLANG_FORMAT) $(CLANG_FORMAT_OPTS) $(SRC_FILES)
endef

define compile
    @$(MAKE_SH) $(1)
endef

define precommit_cmd
    $(PRECOMMIT) $(1)
endef

define make_subdirectory
    mkdir -p "$@"
endef

define update_conda_env
    $(CONDA) env update --file $(CONDA_ENV_FILE)
endef

#################################################################################
# COMMANDS                                                                      #
#################################################################################

## Compile using cmake (local machine)
build-local:
	$(call compile, local)

## Compile using cmake (ARGO)
build-argo:
	$(call compile, argo)

## Remove temporary and build files
clean:
	$(call cleanup)

## Create/update conda-based virtual environment
conda:
	$(call update_conda_env)

## Setup development environment
dev: conda
	$(call precommit_cmd, install)

## Reformat C++ code with clang-format and Python code with black
prettify:
	$(call clang_format)
	$(call python_black)

#################################################################################
# PROJECT RULES                                                                 #
#################################################################################

$(SUBDIRS):
	$(call make_subdirectory)

#################################################################################
# Self Documenting Commands                                                     #
#################################################################################

.DEFAULT_GOAL := help

# Inspired by <http://marmelab.com/blog/2016/02/29/auto-documented-makefile.html>
# sed script explained:
# /^##/:
# 	* save line in hold space
# 	* purge line
# 	* Loop:
# 		* append newline + line to hold space
# 		* go to next line
# 		* if line starts with doc comment, strip comment character off and loop
# 	* remove target prerequisites
# 	* append hold space (+ newline) to line
# 	* replace newline plus comments by `---`
# 	* print line
# Separate expressions are necessary because labels cannot be delimited by
# semicolon; see <http://stackoverflow.com/a/11799865/1968>
help:
	@echo "$$(tput bold)Available rules:$$(tput sgr0)"
	@echo
	@sed -n -e "/^## / { \
		h; \
		s/.*//; \
		:doc" \
		-e "H; \
		n; \
		s/^## //; \
		t doc" \
		-e "s/:.*//; \
		G; \
		s/\\n## /---/; \
		s/\\n/ /g; \
		p; \
	}" ${MAKEFILE_LIST} \
	| LC_ALL='C' sort --ignore-case \
	| awk -F '---' \
		-v ncol=$$(tput cols) \
		-v indent=19 \
		-v col_on="$$(tput setaf 6)" \
		-v col_off="$$(tput sgr0)" \
	'{ \
		printf "%s%*s%s ", col_on, -indent, $$1, col_off; \
		n = split($$2, words, " "); \
		line_length = ncol - indent; \
		for (i = 1; i <= n; i++) { \
			line_length -= length(words[i]) + 1; \
			if (line_length <= 0) { \
				line_length = ncol - indent - length(words[i]) - 1; \
				printf "\n%*s ", -indent, " "; \
			} \
			printf "%s ", words[i]; \
		} \
		printf "\n"; \
	}' \
	| more $(shell test $(shell uname) = Darwin && echo '--no-init --raw-control-chars')
