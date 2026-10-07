PYTHON ?= python3
VENV := .venv
VENV_PYTHON := $(VENV)/bin/python
VENV_STAMP := $(VENV)/.installed
ROM := baseroms/us/baserom.z64

.PHONY: all setup toolchain symbols verify split build-code verify-code check clean-generated

all: split

$(VENV_STAMP): requirements.txt
	$(PYTHON) -m venv $(VENV)
	$(VENV_PYTHON) -m pip install --upgrade pip
	$(VENV_PYTHON) -m pip install -r requirements.txt
	touch $(VENV_STAMP)

setup: $(VENV_STAMP)
	$(VENV_PYTHON) tools/prepare_rom.py --output $(ROM)

toolchain:
	tools/bootstrap_mips_binutils.sh
	tools/bootstrap_ido.sh
	tools/bootstrap_kmc_gcc.sh

verify:
	$(PYTHON) tools/verify_rom.py $(ROM)

symbols:
	$(PYTHON) tools/import_recomp_symbols.py \
		config/us/recomp_function_boundaries.toml \
		config/us/recomp_symbol_seed.txt \
		config/us/symbol_addrs.txt \
		--manual config/us/manual_symbols.toml

split: $(VENV_STAMP) verify symbols
	$(VENV_PYTHON) -m splat split config/us/splat.yaml

build-code: split
	tools/build_code.sh

verify-code: build-code
	cmp -s -n 1052672 $(ROM) build/us/battletanx_ga.code.bin
	@echo "Verified reconstructed ROM 0x000000-0x101000 matches the retail dump"

check:
	$(PYTHON) -m unittest discover -s tests -v
	$(PYTHON) tools/check_repository.py

clean-generated:
	rm -rf asm assets/extracted build .splache
