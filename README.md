🎨 Multithreaded Producer–Consumer Console Art

This project demonstrates multithreading in C++ using the classic Producer–Consumer pattern and the shared memory technique.

The result is a piece of computer-generated console art, where random characters are drawn at random positions in the terminal with random colors, filling the screen over time until it stops automatically when most of the console is filled.

🚀 Features

Producer Thread

Continuously generates random numbers.

Pushes numbers into a shared memory queue.

Sleeps for a random interval between 100–600 ms.

Notifies consumers when new data is available.

Consumer Threads

Wait for data from the shared queue.

Retrieve numbers and use them to set font color.

Print random characters at random (x, y) positions in the console.

Stop gracefully once most of the console screen is filled (~80%).

Shared Memory & Synchronization

Threads communicate through a shared queue in memory (std::queue<int>).

A std::mutex ensures thread-safe access to the queue.

A std::condition_variable allows the producer to signal consumers when data is ready.

A shared counter tracks how much of the console has been filled.

🛠️ Technologies Used

C++11 multithreading (std::thread, std::mutex, std::condition_variable)

Shared memory technique for inter-thread communication

Windows API (SetConsoleCursorPosition, SetConsoleTextAttribute) for console manipulation

Randomized output to create generative art

📂 How It Works

Producer generates a random number (0–15) and pushes it into a shared queue in memory.

Consumers wait until data appears in the queue.

Each number determines:

The text color for characters.

The number of characters to print.

Consumers print random letters (A–Z) at random positions.

A shared counter tracks how much of the console has been filled. Once ~80% of the grid is full, the program terminates gracefully.

▶️ Running the Program in Visual Studio

Open Visual Studio (any edition that supports C++).

Go to File → New → Project.

Select Console App (C++).

Copy the source code (main.cpp) into the project’s main file.

Make sure the project is set to compile with C++11 or later (Visual Studio supports this by default).

Press Ctrl+F5 (or "Start Without Debugging") to build and run the program.

Watch as the console fills with colorful random characters until it automatically stops 🎨

📚 Learning Outcomes

This project demonstrates:

The shared memory technique for thread communication.

How to safely share resources with std::mutex.

How std::condition_variable prevents wasteful polling.

How concurrency and randomness can be combined to create generative computer art.

🧑‍💻 Author

Developed by DanLDevs
