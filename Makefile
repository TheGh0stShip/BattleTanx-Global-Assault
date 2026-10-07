PYTHON ?= python3
VENV := .venv
VENV_PYTHON := $(VENV)/bin/python
VENV_STAMP := $(VENV)/.installed
ROM := baseroms/us/baserom.z64

.PHONY: all setup verify split check clean-generated

all: split

$(VENV_STAMP): requirements.txt
	$(PYTHON) -m venv $(VENV)
	$(VENV_PYTHON) -m pip install --upgrade pip
	$(VENV_PYTHON) -m pip install -r requirements.txt
	touch $(VENV_STAMP)

setup: $(VENV_STAMP)
	$(VENV_PYTHON) tools/prepare_rom.py --output $(ROM)

verify:
	$(PYTHON) tools/verify_rom.py $(ROM)

split: $(VENV_STAMP) verify
	$(VENV_PYTHON) -m splat split config/us/splat.yaml

check:
	$(PYTHON) -m unittest discover -s tests -v
	$(PYTHON) tools/check_repository.py

clean-generated:
	rm -rf asm assets/extracted build .splache
