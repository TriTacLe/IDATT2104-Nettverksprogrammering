# IDATT2104 Nettverksprogrammering

Kursmateriale og øvinger i Nettverksprogrammering ved NTNU.

## Mappestruktur

```
├── gatekeep/               # Eksamensoppgaver og sensorveiledninger
├── ovinger/
│   ├── datakommunikasjon/  # Teoriøvinger (PDF/DOCX)
│   │   ├── oblig1/         # Øving 1-3: Lagmodell, DNS, TCP/TLS
│   │   └── oblig2/         # Øving 4-6: IP, lenkelaget
│   └── programmering/      # Programmeringsøvinger
│       ├── ov1/            # Tråder: Primtall
│       ├── ov2/            # Thread pool
│       ├── ov3/            # HTTP-server (Python sockets)
│       ├── ov4/            # UDP-server (ASIO)
│       ├── ov5/            # Webchat (Spring Boot + Vue.js)
│       └── ov6/            # WebSocket
└── presentasjoner/         # Forelesningsnotater
```

## Programmeringsøvinger

### Øving 1: Parallelle Tråder (C++)
**Fil:** `ovinger/programmering/ov1/main.cpp`

Finn alle primtall mellom to tall ved hjelp av parallelle tråder. Hver tråd får tildelt et intervall å sjekke. Resultater samles i en delt vektor med mutex-beskyttelse.

**Kjøring:**
```bash
cd ovinger/programmering/ov1
make
./main <fra> <til> <antall_tråder>
```

### Øving 2: Thread Pool (C++)
**Fil:** `ovinger/programmering/ov2/main.cpp`

Implementasjon av en thread pool med task-kø. Støtter:
- Faste worker-tråder
- Event loop (single thread)
- Timeout-funksjon for forsinkede tasks

**Kjøring:**
```bash
cd ovinger/programmering/ov2
make
./main
```

### Øving 3: HTTP-Server (Python)
**Filer:**
- `ovinger/programmering/ov3/server.py` - Grunnleggende HTTP-server
- `ovinger/programmering/ov3/flask-test/` - Flask-versjon med database

Enkel HTTP/1.1-server som håndterer GET-forespørsler og routing.

**Kjøring:**
```bash
cd ovinger/programmering/ov3
python3 server.py          # Port 3000
```

### Øving 4: UDP-Server (C++ / ASIO)
**Filer:**
- `ovinger/programmering/ov4/udp_server.cpp`
- `ovinger/programmering/ov4/udp_client.cpp`
- `ovinger/programmering/ov4/https_server.py`

UDP-server med C++20 coroutines (ASIO) som beregner skalarprodukt av to vektorer. Inkluderer også HTTPS-server i Python.

**Kjøring:**
```bash
cd ovinger/programmering/ov4
make
./udp_server &             # Port 3000
./udp_client               # Sender testvektorer
```

### Øving 5: Webchat (Fullstack)
**Filer:** `ovinger/programmering/ov5/`
- `backend/` - Spring Boot (Java)
- `frontend/` - Vue.js
- `docker-compose.yml`

Chat-applikasjon med:
- REST API (Spring Boot)
- PostgreSQL database
- Docker-containerisering
- Vue.js frontend

**Kjøring:**
```bash
cd ovinger/programmering/ov5

# Alternativ 1: Backend lokalt, Frontend Docker
./mvnw spring-boot:run     # Terminal 1
docker-compose up -d       # Terminal 2 (frontend)

# Alternativ 2: Alt i Docker
docker-compose -f docker-compose.full.yml up --build
```

### Øving 6: WebSocket
**Fil:** `ovinger/programmering/ov6/websocket.js`

WebSocket-implementasjon for sanntidskommunikasjon.

## Datakommunikasjonsøvinger

- **Øving 1:** Lagmodell og Wireshark
- **Øving 2:** DNS, E-post og Web (HTTP)
- **Øving 3:** TCP og TLS
- **Øving 4:** IP og Nettlaget
- **Øving 5:** Ruting og nettverksfunksjoner
- **Øving 6:** Lenkelaget

## Bygging

### C++
Alle C++ prosjekter har `makefile`. Kompiler med:
```bash
make
./program_navn
```

### Python
Krevde pakker:
```bash
pip install flask
```

### Docker
Fullstack-prosjektet (øving 5) bruker Docker. Se `ovinger/programmering/ov5/README.md` for detaljer.

## Gitignore

Repositoriet ignorerer:
- Kompilerte binærfiler (`*.o`, `a.out`, `main`)
- Python cache (`__pycache__/`, `*.pyc`)
- Node modules
- Databaser (`*.db`)
- IDE-filer (`.vscode/`, `.idea/`)
- macOS systemfiler (`.DS_Store`)
- Eksamensløsninger (`gatekeep/`)

## Forbedringsforslag til struktur

1. **Konsistent navngivning:** 
   - Bruk `ov1`, `ov2` konsistent (ikke `ov1` og `øv1`)
   - Vurder `assignment-01` format for bedre sortering

2. **Byggmapper:**
   - Legg til `build/` i .gitignore og kompiler dit
   - Eller bruk `/bin` eller `/out` mapper per øving

3. **Dokumentasjon:**
   - Legg til README.md i hver øvingsmappe med spesifikke instruksjoner
   - Dokumenter avhengigheter (ASIO, Java-versjon, etc.)

4. **Testfiler:**
   - Separat testkode fra produksjonskode
   - Vurder `tests/` mapper

5. **Konfigurasjon:**
   - Flytt hardkodede porter/hosts til config-filer
   - Bruk `.env` for miljøvariabler
