#include <iostream>
#include <thread> // for threading
#include <queue> // for queue to hold numbers
#include <mutex> // for mutex for thread 
#include <condition_variable> // for condition variable to signal between threads
#include <Windows.h> // for console manipulation

using namespace std; // use standard namespace

mutex mtx; // protects access to numberQueue
condition_variable cv; // signals that new data is available
queue<int> numberQueue; // declare a queue to hold numbers shared between 
int consoleWidth = 80;
int consoleHeight = 25;

// Set cursor position in console
void setCursor(int x, int y) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // get console handle
	COORD pos = { (SHORT)x, (SHORT)y }; // set coordinates
	SetConsoleCursorPosition(hConsole, pos); // set cursor position
}

// Set text color in console
void setColor(int color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // get console handle
	SetConsoleTextAttribute(hConsole, color); // set text color
}

// Producer thread 
void producerThread() {
	while (true) {
		int num = rand() % 256; // generate a random number from 0-255 for colors
		{
			lock_guard<mutex> lock(mtx); // lock the mutex while accessing the queue
			numberQueue.push(num); // push the number onto the queue
		} // lock_guard goes out of scope and unlocks the mutex here
		cv.notify_one(); // notify one waiting consumer
		this_thread::sleep_for(chrono::milliseconds(rand() % 500 + 100)); // sleep for a random time between 100-599 ms
	}
}

// Consumer thread function
void consumerThread() {
	// Continuously process numbers from the queue until signaled to stop
	while (true) {
		unique_lock<mutex> lock(mtx); // lock the mutex
		cv.wait(lock, [] { return !numberQueue.empty(); }); // wait until there's data

		// if there's data, process it
		if (!numberQueue.empty()) {
			int num = numberQueue.front(); // get the number from the front of the queue
			numberQueue.pop(); // remove it from the queue
			lock.unlock(); // unlock the mutex while processing

			// Process the retrieved number by drawing random colored characters at random positions on the console.
			// The number determines how many characters are drawn, and the color is set based on the number value.
			for (int i = 0; i < num; ++i) {
				int x = rand() % consoleWidth; // console width
				int y = rand() % consoleHeight; // console height
				char c = 'A' + rand() % 26; // random character

				setCursor(x, y); // move cursor to random position
				setColor(num); // set text color based on number
				cout << c; // print the character
			}
		}
	}
}

int main() {
	thread prod(producerThread); // start producer thread 
	thread cons1(consumerThread); // start first consumer thread
	thread cons2(consumerThread); // start second consumer thread

	prod.join(); // Wait for the producer thread to finish before proceeding (main thread blocks until producer completes)
	cons1.join(); // Wait for first consumer thread to finish before proceeding
	cons2.join(); // Wait for second consumer thread to finish before proceeding

	return 0;
}
