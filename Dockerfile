# ========================
# Stage 1: Build the Application
# ========================
FROM debian:bookworm-slim AS builder

# Install required dependencies for building the application
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory for building
WORKDIR /build

# Copy the source files to the container
COPY ./src /build/src
COPY ./CppKubernetesApp /build/CppKubernetesApp

# Run the build process
RUN mkdir build && \
    cd build && \
    cmake ../src && \
    make && \
    ./CppKubernetesApp --version

# ========================
# Stage 2: Create the Final Image
# ========================
FROM debian:bookworm-slim

# Set environment variables for runtime
ENV APP_NAME=CppKubernetesApp \
    APP_VERSION=1.0.0 \
    APP_ENV=production

# Set the working directory
WORKDIR /app

# Copy the binary from the builder stage
COPY --from=builder /build/build/CppKubernetesApp /app/

# Add a non-root user for security
RUN useradd --create-home --shell /bin/bash appuser && \
    chown -R appuser:appuser /app

# Switch to non-root user
USER appuser

# Expose the application port
EXPOSE 8080

# Default command to run the application
CMD ["/app/CppKubernetesApp"]
