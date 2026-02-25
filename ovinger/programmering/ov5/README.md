# Kjøre kommandoer

## Utvikling (anbefalt - Backend lokalt, Frontend Docker)

### 1. Start Backend (lokalt)
```bash
cd backend
./mvnw spring-boot:run
```
Kjører på: http://localhost:8080

### 2. Start Frontend (Docker)
```bash
docker-compose up -d
```
Kjører på: http://localhost:3000

### 3. Stopp
```bash
# Terminal 1: Ctrl+C (backend)
docker-compose down  # (frontend)
```

## Alt i Docker (tregere bygg)

```bash
docker-compose -f docker-compose.full.yml up --build
```

## Rydde opp

```bash
make clean-all
```
