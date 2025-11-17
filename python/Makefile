.PHONY: help install install-dev run format lint type-check test clean

help:  ## Mostra esta mensagem de ajuda
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'

install:  ## Instala as dependências do projeto com uv
	uv pip install -e .

install-dev:  ## Instala as dependências de desenvolvimento com uv
	uv pip install -e ".[dev]"

run:  ## Executa o exemplo LT-3F-765kv.py
	python LT-3F-765kv.py

format:  ## Formata o código com black
	black campelmag/ LT-3F-765kv.py

lint:  ## Executa linting com ruff
	ruff check campelmag/ LT-3F-765kv.py

lint-fix:  ## Executa linting com ruff e corrige automaticamente
	ruff check --fix campelmag/ LT-3F-765kv.py

type-check:  ## Verifica tipos com mypy
	mypy campelmag/

test:  ## Executa os testes (quando implementados)
	pytest tests/ -v

clean:  ## Remove arquivos temporários e cache
	find . -type d -name "__pycache__" -exec rm -rf {} + 2>/dev/null || true
	find . -type f -name "*.pyc" -delete
	find . -type f -name "*.pyo" -delete
	find . -type d -name "*.egg-info" -exec rm -rf {} + 2>/dev/null || true
	rm -rf build/ dist/ .mypy_cache/ .pytest_cache/ .ruff_cache/

setup-uv:  ## Instala uv (se não estiver instalado)
	@command -v uv >/dev/null 2>&1 || { \
		echo "Instalando uv..."; \
		curl -LsSf https://astral.sh/uv/install.sh | sh; \
	}

venv:  ## Cria ambiente virtual com uv
	uv venv

init: setup-uv venv install-dev  ## Setup completo do projeto (instala uv, cria venv, instala deps)
	@echo "✅ Projeto configurado com sucesso!"
	@echo "Execute 'source .venv/bin/activate' (ou '.venv\\Scripts\\activate' no Windows)"
