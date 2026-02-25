# Makefile for IDATT2104 Nettverksprogrammering
# Bruk: make clean         - Fjern alle genererte filer
#       make clean-all     - Fjern + clean i alle underprosjekter
#       make package       - Lag leveringsklart arkiv
#       make size          - Vis størrelsesoversikt

# Prosjektnavn
PROJECT_NAME = nettverksprogrammering
DELIVERY_NAME = $(PROJECT_NAME)-levering

# Farger for output
BLUE = \033[36m
GREEN = \033[32m
YELLOW = \033[33m
RED = \033[31m
NC = \033[0m # No Color

.PHONY: help clean clean-all clean-node-modules clean-build clean-python clean-db \
        clean-dsstore package size check structure

help:
	@echo "$(BLUE)IDATT2104 Nettverksprogrammering - Renseverktøy$(NC)"
	@echo ""
	@echo "Tilgjengelige kommandoer:"
	@echo "  $(GREEN)make clean$(NC)         - Fjern genererte filer (behold node_modules)"
	@echo "  $(GREEN)make clean-all$(NC)     - Fjern alt inkl. node_modules + db"
	@echo "  $(GREEN)make clean-code$(NC)    - Fjern kun kompilerte binærfiler"
	@echo "  $(GREEN)make package$(NC)       - Lag leveringsklart arkiv (~1MB)"
	@echo "  $(GREEN)make size$(NC)          - Vis størrelsesoversikt"
	@echo "  $(GREEN)make structure$(NC)     - Vis prosjektstruktur"
	@echo "  $(GREEN)make check$(NC)         - Sjekk hva som vil bli fjernet"

# Rask clean - behold node_modules og databaser
clean: clean-build clean-python clean-dsstore
	@echo "$(GREEN)✓$(NC) Fjernet bygg-filer og cache"

# Total clean - alt unødvendig
clean-all: clean clean-node-modules clean-db clean-ide
	@echo "$(GREEN)✓$(NC) Total rensning fullført"

# Kun kode-relatert (binærfiler)
clean-code: clean-build
	@echo "$(GREEN)✓$(NC) Fjernet kompilerte programmer"

# Fjern node_modules overalt
clean-node-modules:
	@echo "$(YELLOW)Fjerner node_modules...$(NC)"
	@find . -type d -name "node_modules" -not -path "./.git/*" -exec rm -rf {} + 2>/dev/null || true
	@echo "$(GREEN)✓$(NC) Fjernet node_modules"

# Fjern bygg-artefakter
clean-build:
	@echo "$(YELLOW)Fjerner kompilerte binærfiler...$(NC)"
	@find . -type f \( \
		-name "*.o" -o \
		-name "*.obj" -o \
		-name "a.out" -o \
		-name "main" -o \
		-name "udp_client" -o \
		-name "udp_server" -o \
		-name "prime_finder" \
	\) -not -path "./.git/*" -delete 2>/dev/null || true
	@find . -type d -name "build" -not -path "./.git/*" -exec rm -rf {} + 2>/dev/null || true
	@find . -type d -name "bin" -not -path "./.git/*" -exec rm -rf {} + 2>/dev/null || true
	@find . -type d -name "out" -not -path "./.git/*" -exec rm -rf {} + 2>/dev/null || true
	@find . -type d -name "target" -not -path "./.git/*" -exec rm -rf {} + 2>/dev/null || true
	@echo "$(GREEN)✓$(NC) Fjernet bygg-filer"

# Fjern Python cache
clean-python:
	@echo "$(YELLOW)Fjerner Python cache...$(NC)"
	@find . -type d -name "__pycache__" -not -path "./.git/*" -exec rm -rf {} + 2>/dev/null || true
	@find . -type f -name "*.pyc" -delete 2>/dev/null || true
	@find . -type f -name "*.pyo" -delete 2>/dev/null || true
	@find . -type f -name "*.pyd" -delete 2>/dev/null || true
	@echo "$(GREEN)✓$(NC) Fjernet Python cache"

# Fjern databaser
clean-db:
	@echo "$(YELLOW)Fjerner databasefiler...$(NC)"
	@find . -type f \( -name "*.db" -o -name "*.sqlite" -o -name "*.sqlite3" \) \
		-not -path "./.git/*" -not -path "*/schema.sql" -delete 2>/dev/null || true
	@echo "$(GREEN)✓$(NC) Fjernet databasefiler"

# Fjern IDE-filer
clean-ide:
	@echo "$(YELLOW)Fjerner IDE-filer...$(NC)"
	@find . -type d -name ".idea" -not -path "./.git/*" -exec rm -rf {} + 2>/dev/null || true
	@find . -type d -name ".vscode" -not -path "./.git/*" -exec rm -rf {} + 2>/dev/null || true
	@echo "$(GREEN)✓$(NC) Fjernet IDE-filer"

# Fjern .DS_Store
clean-dsstore:
	@echo "$(YELLOW)Fjerner .DS_Store filer...$(NC)"
	@find . -name ".DS_Store" -type f -delete 2>/dev/null || true
	@echo "$(GREEN)✓$(NC) Fjernet .DS_Store filer"

# Vis hva som vil bli fjernet (tørrkjøring)
check:
	@echo "$(BLUE)=== Filmer som vil bli fjernet ===$(NC)"
	@echo ""
	@echo "$(YELLOW)Node modules:$(NC)"
	@find . -type d -name "node_modules" -not -path "./.git/*" 2>/dev/null | head -5 || echo "  Ingen funnet"
	@echo ""
	@echo "$(YELLOW)Kompilerte filer:$(NC)"
	@find . -type f \( -name "*.o" -o -name "a.out" -o -name "main" -o -name "udp_client" -o -name "udp_server" \) \
		-not -path "./.git/*" -not -path "*/node_modules/*" 2>/dev/null | head -10 || echo "  Ingen funnet"
	@echo ""
	@echo "$(YELLOW)Python cache:$(NC)"
	@find . -type d -name "__pycache__" -not -path "./.git/*" 2>/dev/null | head -5 || echo "  Ingen funnet"
	@echo ""
	@echo "$(YELLOW)Databaser:$(NC)"
	@find . -type f \( -name "*.db" -o -name "*.sqlite" \) -not -path "./.git/*" 2>/dev/null | head -5 || echo "  Ingen funnet"
	@echo ""
	@echo "$(YELLOW).DS_Store filer:$(NC)"
	@find . -name ".DS_Store" -type f 2>/dev/null | wc -l | xargs echo "  Antall:"

# Lag full backup (med PDF-er men uten node_modules)
backup: clean-all
	@echo "$(BLUE)Lager backup med PDF-er...$(NC)"
	@tar czf $(PROJECT_NAME)-backup.tar.gz \
		--exclude='.git' \
		--exclude='node_modules' \
		--exclude='gatekeep' \
		--exclude='*.tar.gz' \
		.
	@echo "$(GREEN)✓$(NC) Opprettet: $(GREEN)$(PROJECT_NAME)-backup.tar.gz$(NC)"
	@echo "    Størrelse: $$(ls -lh $(PROJECT_NAME)-backup.tar.gz | awk '{print $$5}')"

# Vis størrelsesoversikt
size:
	@echo "$(BLUE)=== Størrelsesoversikt ===$(NC)"
	@echo ""
	@echo "$(YELLOW)Totalt (inkl. .git):$(NC) $$(du -sh . | awk '{print $$1}')"
	@echo "$(YELLOW).git mappe:$(NC)          $$(du -sh .git | awk '{print $$1}')"
	@echo "$(YELLOW)node_modules:$(NC)       $$(find . -type d -name node_modules -exec du -sh {} + 2>/dev/null | awk '{sum+=$$1} END {print sum "K"}' || echo '0')"
	@echo ""
	@echo "$(GREEN)Største filer:$(NC)"
	@find . -type f -not -path './.git/*' -exec du -h {} + 2>/dev/null | sort -hr | head -10

# Vis prosjektstruktur
structure:
	@echo "$(BLUE)=== Prosjektstruktur ===$(NC)"
	@tree -L 3 -I 'node_modules|.git|__pycache__|*.o|a.out|main|udp_client|udp_server' 2>/dev/null || \
		find . -maxdepth 3 -not -path './.git/*' -not -path '*/node_modules/*' -not -path '*/__pycache__/*' | head -30

# Forbered til innlevering (lager mappe i stedet for tar)
delivery-folder: clean-all
	@echo "$(BLUE)Forbereder leveringsmappe...$(NC)"
	@rm -rf $(DELIVERY_NAME)
	@mkdir -p $(DELIVERY_NAME)
	@cp -r ovinger/programmering $(DELIVERY_NAME)/
	@cp README.md $(DELIVERY_NAME)/
	@cp Makefile $(DELIVERY_NAME)/
	@cp .gitignore $(DELIVERY_NAME)/
	@echo "$(GREEN)✓$(NC) Opprettet: $(GREEN)$(DELIVERY_NAME)/$(NC)"
	@echo "    Størrelse: $$(du -sh $(DELIVERY_NAME) | awk '{print $$1}')"
	@echo "    Inneholder: Programmeringsøvinger + README + Makefile"
