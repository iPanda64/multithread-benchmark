# 📊 Multithread Benchmark
![Platform](https://img.shields.io/badge/Platform-Windows_x86%2F64-blue) ![Language](https://img.shields.io/badge/Language-C%2B%2B_%7C_Java_%7C_Python-orange)

This application serves as a comprehensive performance benchmarking set designed to evaluate the efficiency of **Windows-based x86/x64** multicore processors. Its primary function is to execute  computational and I/O-intensive tasks across multiple parallel threads, measuring exactly how the system handles the workload as the number of active threads varies using high-precision timers (RDTSC/clock) to analyze workload efficiency.
<p align="center">
  <img src="https://github.com/iPanda64/multithread-benchmark/blob/main/screenshots/sorting 8 threads clock.png" alt="Graph Results" width="600" />
</p>

<p align="center">
  <img src="https://github.com/iPanda64/multithread-benchmark/blob/main/screenshots/start.png" alt="Start Panel" width="150" />
  &nbsp; &nbsp; <img src="https://github.com/iPanda64/multithread-benchmark/blob/main/screenshots/running.png" alt="Runtime Panel" width="200" />
</p>

## ✨ Features

The benchmark provides a flexible set of configuration options:
* **Algorithms**
    * Matrix Multiplication
    * Calculating Prime Numbers
    * Sorting (Parallel Merge Sort)

* **Timer**
    * `clock()` (Standard)
    * `RDTSC` (Read Time-Stamp Counter for high precision)

* **Threads**
    * Manual selection
    * Automatic detection (uses all available hardware threads)

* **I/O Analysis**
    * A dedicated mode to examine performance in file-access workloads.

## 🛠️ Prerequisites
*   **Operating System**: Windows 10 or 11 (x86 or x64).
*   **Architecture**: Intel/AMD x86 Processor.
*   **Java Development Kit (JDK)** (version 8 or later)
*   **C++ Compiler**: Microsoft Visual Studio 2022 (Community Edition).
*   **Python** (version 3.6 or later)

## ⚙️ Setup and Run

1.  **Clone the repository and navigate to the project directory:**
    ```sh
    git clone https://github.com/iPanda64/multithread-benchmark
    cd multithread-benchmark
    ```

2.  **Build the C++ backend:**
    *   Run the `build.cmd` script. This will compile the C++ source files and create `program.exe` in the `build` directory.

3.  **Set up the Python environment:**
    Navigate to the project's root directory and run the following commands to create a virtual environment and install the required packages:
        ```sh
        python -m venv py_src/myenv
        py_src\myenv\Scripts\activate
        pip install -r py_src/requirements.txt
        ```

4.  **Compile and Run the Java Application:**
    From the project's root directory, compile and run the Java application. How you do this will depend on your setup (IDE or command line).
    * **Comand line:** 
    ```sh
    javac -d java_src/out/production java_src/*.java
    java -cp java_src/out/production StartPanel
    ```
    *   **Using an IDE (like IntelliJ or Eclipse):**
        *   Open the project.
        *   Make sure your IDE's run configuration is set to use the project's root directory as the **working directory**.
