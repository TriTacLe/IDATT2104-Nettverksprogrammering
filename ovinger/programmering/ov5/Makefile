# Makefile for cleaning up the project

.PHONY: clean clean-docker clean-all stop

# Stop running containers
stop:
	docker-compose down

# Remove Docker containers and images
clean-docker: stop
	@echo "Removing Docker containers..."
	docker-compose rm -f
	@echo "Removing Docker images..."
	docker rmi ov5_backend ov5_frontend 2>/dev/null || true
	docker rmi backend 2>/dev/null || true
	@echo "Pruning unused Docker resources..."
	docker system prune -f

# Clean Maven/Java build files
clean-backend:
	@echo "Cleaning backend build files..."
	cd backend && ./mvnw clean 2>/dev/null || mvn clean
	rm -rf backend/target
	rm -rf backend/.mvn/wrapper/*.jar

# Clean Node.js build files
clean-frontend:
	@echo "Cleaning frontend build files..."
	rm -rf frontend/node_modules
	rm -rf frontend/dist
	rm -f frontend/package-lock.json

# Remove temp directories
clean-temp:
	@echo "Cleaning temp directories..."
	rm -rf /tmp/cpp-build*
	rm -rf /tmp/temp*

# Full clean (everything)
clean-all: clean-docker clean-backend clean-frontend clean-temp
	@echo "Full cleanup complete!"

# Quick clean (keep Docker images)
clean: clean-backend clean-frontend clean-temp
	@echo "Cleanup complete (Docker images kept)!"

# Help
help:
	@echo "Available commands:"
	@echo "  make stop        - Stop running Docker containers"
	@echo "  make clean       - Clean build files (keep Docker)"
	@echo "  make clean-docker - Remove all Docker containers and images"
	@echo "  make clean-all   - Full cleanup (everything)"
	@echo "  make clean-temp  - Remove temp directories only"
