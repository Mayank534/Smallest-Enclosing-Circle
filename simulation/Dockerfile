# Use an official Python runtime as a parent image
FROM python:3.10-slim

# Install system dependencies for C++, CGAL, and Pybind11
RUN apt-get update && apt-get install -y \
    g++ \
    libcgal-dev \
    libgmp-dev \
    libmpfr-dev \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory in the container
WORKDIR /app

# Copy the current directory contents into the container
COPY . /app

# Install Python packages
RUN pip install --no-cache-dir -r requirements.txt

# Compile the C++ code to a shared library that Python can import
RUN g++ -O3 -Wall -shared -std=c++17 -fPIC \
    $(python -m pybind11 --includes) \
    mec_wrapper.cpp \
    -o mec_lib$(python3-config --extension-suffix) \
    -lgmp -lmpfr

# Expose port 8000 for the web server
EXPOSE 8000

# Command to run the application
CMD ["uvicorn", "main:app", "--host", "0.0.0.0", "--port", "8000"]