# Use a base image for Windows (temporary, because this .exe is for Windows)
FROM mcr.microsoft.com/windows/servercore:ltsc2022

# Set the working directory
WORKDIR /app

# Copy the Windows executable
COPY ./CppKubernetesApp.exe .

# Command to run the application
CMD ["CppKubernetesApp.exe"]
