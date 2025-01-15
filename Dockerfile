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
    find /build

# ========================
# Stage 2: Create the Final Image
# ========================
FROM debian:bookworm-slim

# Set environment variables for runtime (default values can be overridden)
ENV APP_NAME=CppKubernetesApp \
    APP_VERSION=1.0.0 \
    APP_ENV=production

# Set the working directory
WORKDIR /app

# Copy the binary from the builder stage
COPY --from=builder /build/build/CppKubernetesApp .

# Ensure binary is executable
RUN chmod +x CppKubernetesApp

# Add a non-root user for security
RUN useradd --create-home --shell /bin/bash appuser && \
    chown -R appuser:appuser /app

# Switch to non-root user
USER appuser

# Expose the application port (if applicable)
EXPOSE 8080

# Ensure the binary is executable
RUN chmod +x CppKubernetesApp

# Default command to run the application
CMD ["./CppKubernetesApp"]

# ========================
# Additional Notes
# ========================
# 1. The `ENV` section defines environment variables to configure the app at runtime.
# 2. The `USER` directive ensures the application doesn't run as root, increasing security.
# 3. Multi-stage builds reduce the final image size by omitting build tools from the production image.
# 4. `EXPOSE` specifies the default port the app listens to (optional, but recommended for documentation).
