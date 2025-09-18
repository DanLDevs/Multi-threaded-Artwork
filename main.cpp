#include <iostream>
#include <thread> // for threading
#include <queue> // for queue to hold numbers
#include <mutex> // for mutex for thread 
#include <condition_variable> // for condition variable to signal between threads
#include <Windows.h> // for console manipulation

using namespace std; // use standard namespace

mutex mtx; // protects access to numberQueue
condition_variable cv; // signals that new data is available
queue<int> numberQueue; // declare a queue to hold numbers shared between threads
bool done = false; // flag to signal consumer to stop

// Set cursor position in console
void setCursor(int x, int y) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); // get console handle
	COORD pos = { (SHORT)x, (SHORT)y };
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
		}
		cv.notify_one(); // notify one waiting consumer
		this_thread::sleep_for(chrono::milliseconds(rand() % 500 + 100)); // sleep for a random time between 100-600ms
	}
}

// Consumer thread function
void consumerThread() {
	// Continuously process numbers from the queue until signaled to stop
	while (true) {
		unique_lock<mutex> lock(mtx); // lock the mutex
		cv.wait(lock, [] { return !numberQueue.empty() || done;  }); // wait until there's data or done is true

		// if there's data, process it
		if (!numberQueue.empty()) {
			int num = numberQueue.front(); // get the number from the front of the queue
			numberQueue.pop(); // remove it from the queue
			lock.unlock(); // unlock the mutex while processing

			// Process the retrieved number by drawing random colored characters at random positions on the console.
			// The number determines how many characters are drawn, and the color is set based on the number value.
			for (int i = 0; i < num; ++i) {
				int x = rand() % 80; // console width
				int y = rand() % 25; // console height
				char c = 'A' + rand() % 26; // random character

				setCursor(x, y); // move cursor to random position
				setColor(num); // set text color based on number
				cout << c; // print the character
			}
		}
		else if (done) { // if done and queue is empty, exit
			break;
		}
	}
}

int main() {
	thread prod(producerThread); // start producer thread 
	thread cons1(consumerThread); // start first consumer thread
	thread cons2(consumerThread); // start second consumer thread

	prod.join();
	cons1.join();
	cons2.join();

	return 0;
}