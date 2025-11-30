# Multithread Benchmark
This application serves as a comprehensive performance benchmarking set designed to evaluate the efficiency of multicore processors. Its primary function is to execute  computational and I/O-intensive tasks across multiple parallel threads, measuring exactly how the system handles the workload as the number of active threads varies.
<p align="center">
  <img src="https://github.com/iPanda64/multithread-benchmark/blob/main/screenshots/sorting 8 threads clock.png" alt="Graph Results" width="600" />
</p>

<p align="center">
  <img src="https://github.com/iPanda64/multithread-benchmark/blob/main/screenshots/start.png" alt="Start Panel" width="150" />
  &nbsp; &nbsp; <img src="https://github.com/iPanda64/multithread-benchmark/blob/main/screenshots/running.png" alt="Runtime Panel" width="200" />
</p>
## Prerequisites

Before you begin, ensure you have the following installed:

*   **Java Development Kit (JDK)** (version 8 or later)
*   **C++ Compiler**: A C++ compiler that supports C++11. On Windows, you can use Visual Studio with the C++ toolchain.
*   **Python** (version 3.6 or later)

## Setup and Run

1.  **Clone the repository:**
    ```sh
    git clone https://github.com/iPanda64/multithread-benchmark
    ```

2.  **Build the C++ backend:**
    *   **Windows:** Run the `build.cmd` script. This will compile the C++ source files and create `program.exe` in the `build` directory.

3.  **Set up the Python environment:**
    Navigate to the project's root directory and run the following commands to create a virtual environment and install the required packages:

    *   **Windows:**
        ```sh
        python -m venv py_src/myenv
        py_src\myenv\Scripts\activate
        pip install -r py_src/requirements.txt
        ```

4.  **Compile and Run the Java Application:**
    From the project's root directory, compile and run the Java application. How you do this will depend on your setup (IDE or command line).

    *   **Using an IDE (like IntelliJ or Eclipse):**
        *   Open the project.
        *   **Important:** Make sure your IDE's run configuration is set to use the project's root directory as the **working directory**. For example, in IntelliJ IDEA, you should set the "Working directory" field to `$PROJECT_DIR# Multithread Benchmark
