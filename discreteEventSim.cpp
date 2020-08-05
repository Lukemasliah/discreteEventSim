
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Global Variables
int servCount = 0;				// Amount of servers
int customerCount = 0;			// Amount of Customers
int totalServingTime = 0;		// Total serving time for all customers
int longestCustQ = 0;			// Longest customer queue for simulation
int totalEvents = 0;			// Total Events
double TotalCustQAvgLength = 0;	// Avg total length of customer queue			
double avgTimeSpentInQ = 0;		// Avg time spent in queue for individual customers
double noQCus = 0;				// Amount of customers who didnt have to queue




class Customer {
private:
	double arrTime;
	double tallyTime;
	bool paymentMethod;	// True:cash     False:card
	double timeJoinedQ;

public:
	Customer() {
		this->arrTime = 0;
		this->tallyTime = 0;
		this->paymentMethod = false;
		this->timeJoinedQ = 0;
	}

	Customer(double arrivalTime, double tallyT, bool pMethod)
	{
		this->arrTime = arrivalTime;
		this->tallyTime = tallyT;
		this->paymentMethod = pMethod;
		this->timeJoinedQ = 0;
	}

	double getArriveTime()
	{
		return arrTime;
	}

	double getTallyTime()
	{
		return tallyTime;
	}

	bool getPaymentMethod()
	{
		return paymentMethod;
	}

	void joinQueue(double time)
	{
		timeJoinedQ = time;
	}

	double getTimeJoinedQueue()
	{
		return timeJoinedQ;
	}

	void displayCustomer()
	{
		printf("%6.2f %6.2f %6d \n", arrTime, tallyTime, paymentMethod);
	}

	Customer& operator = (const Customer& rhs)
	{
		arrTime = rhs.arrTime;
		tallyTime = rhs.tallyTime;
		paymentMethod = rhs.paymentMethod;
		return *this;
	}
};



class CustomerQueue {

private:
	Customer* custQueue;
	unsigned int front;
	unsigned int rear;
	const static int maxQ;

public:
	CustomerQueue()
	{
		//implement a circular queue
		front = -1;
		rear = -1;
		custQueue = new Customer[maxQ];
	}

	~CustomerQueue()
	{
		delete[] custQueue;
	}

	void enQueue(Customer cust)
	{
		// Check whether the queue is full
		if (front == 0 && rear == maxQ - 1 || (front == rear + 1))
		{
			printf("\nThe Queue is full");
			return;
		}

		// Wrap queue around to beginning if neccessary
		if (front == -1)
		{
			front = 0;
			rear = 0;
		}
		else
		{
			if (rear == maxQ - 1)
				rear = 0;
			else
				rear++;
		}

		// If not full, add customer to end of queue
		custQueue[rear] = cust;

		// Update longest Q information
		if ((rear - front) > longestCustQ)
			longestCustQ = (rear - front);

		return;
	}

	Customer getFrontOfQueue()
	{
		return custQueue[front];
	}

	Customer deQueue()
	{
		if (front == rear)
		{
			front = -1;
			rear = -1;
		}
		else
		{
			if (front == maxQ - 1)
				front = 0;
			else
			{
				Customer temp = custQueue[front];
				front++;
				return temp;
			}
		}
	}

	int getLength()
	{
		return (rear - front);
	}

	void displayQueue(int amtOfRecs)
	{
		if (isEmpty())
		{
			printf("\nThe Queue is empty");
			return;
		}

		// Display the records from the front of the queue to the searched amt
		int lengthToDisplay = (front + amtOfRecs);

		for (int i = front; i < lengthToDisplay; i++)
		{
			custQueue[i].displayCustomer();
		}
	}

	bool isEmpty()
	{
		if (front == rear)
			return true;
		else
			return false;
	}
};

class Event {
private:
	int eventType;		// -1 - Customer arrival		1-5 - Server finish
	double eventTime;
	double tallyTime;
	bool paymentMethod;

public:
	Event() {
		this->eventType = 0;
		this->eventTime = 0;
		this->tallyTime = 0;
		this->paymentMethod = false;
	}

	Event(Customer cust)		// Customer Event
	{
		this->eventType = -1;
		this->eventTime = cust.getArriveTime();
		this->tallyTime = cust.getTallyTime();
		this->paymentMethod = cust.getPaymentMethod();
	}

	Event(int servPosition, double servBusyTime)	// Server Event
	{
		this->eventType = servPosition;
		this->eventTime = servBusyTime;
		this->tallyTime = 0;
		this->paymentMethod = false;
	}

	Event& operator = (const Event& rhs)
	{
		eventType = rhs.eventType;
		eventTime = rhs.eventTime;
		tallyTime = rhs.tallyTime;
		paymentMethod = rhs.paymentMethod;
		return *this;
	}

	double calcPaymentSpeed()
	{
		if (paymentMethod == true)		// Paying with cash    0.3 mins
			return 0.3;
		else
			return 0.7;		// Paying with card    0.7 mins
	}

	double getEventTime()
	{
		return eventTime;
	}

	bool getPaymentMethod()
	{
		return paymentMethod;
	}

	int getEventType()
	{
		return eventType;
	}

	int getTallyTime()
	{
		return tallyTime;
	}
};

class EventQueue {

private:
	Event* eventQueue;
	int rear;
	const static int capacity;

public:
	EventQueue()
	{
		rear = 0;
		eventQueue = new Event[capacity];
	}

	~EventQueue()
	{
		delete[] eventQueue;
	}

	void insertEvent(Event evnt)
	{
		if (rear == capacity)
		{
			cout << "Queue is full" << endl;
			return;
		}
		else
		{
			// Insert the new element and then siftDown into correct position
			++rear;

			eventQueue[rear] = evnt;
			siftUp(rear);
		}
	}


	void siftUp(int position)
	{
		//move element i up to it's correct position
		if (position != 1)
		{
			int p = (position / 2);

			if (eventQueue[p].getEventTime() > eventQueue[position].getEventTime())
			{
				swap(p, position);
				siftUp(p);
			}
		}
	}

	void siftDown(int siftPosition)
	{
		int c = (siftPosition * 2);

		if (c <= rear)
		{
			if (eventQueue[c].getEventTime() > eventQueue[c + 1].getEventTime())
			{
				c = c + 1;
			}

			if (eventQueue[siftPosition].getEventTime() > eventQueue[c].getEventTime())
			{
				swap(c, siftPosition);
			}
			siftDown(c);
		}
	}

	void swap(int origPos, int newPos)
	{
		Event placeHolder = eventQueue[newPos];
		eventQueue[newPos] = eventQueue[origPos];
		eventQueue[origPos] = placeHolder;
	}

	Event deQueue()
	{
		Event temp = eventQueue[1];
		eventQueue[1] = eventQueue[rear];
		rear--;
		siftDown(1);
		return temp;
	}

	bool isEmpty()
	{
		if (rear == 0)
			return true;
		else
			return false;
	}
};

class Server {

private:
	bool busy;
	double efficiency;
	int custsServed;
	double totalServiceTime;

public:
	Server() {};

	Server(double effic)
	{
		busy = false;
		efficiency = effic;
		custsServed = 0;
		totalServiceTime = 0;
	}

	void serveCustomer(double serviceTime)
	{
		this->busy = true;
		custsServed++;
		totalServiceTime += serviceTime;
	}

	void finishServing()
	{
		this->busy = false;
	}

	int isBusy()
	{
		return busy;
	}

	double getEfficiency()
	{
		return efficiency;
	}

	int getCustsServed()
	{
		return custsServed;
	}

	int getTotalServiceTime()
	{
		return totalServiceTime;
	}
};


class ServerArray {

private:
	Server* servers;
	int rear;
	const static int maxServers;

public:
	ServerArray()
	{
		rear = 0;
		servers = new Server[maxServers];
	}

	~ServerArray()
	{
		delete[] servers;
	}

	void addServer(Server server)
	{
		if (rear == maxServers)
		{
			cout << "You have reached max Server allowance" << endl;
			return;
		}

		// Add new server to server array
		++rear;
		
		servers[rear] = server;
		siftUp(rear);
	}

	void siftUp(int position)
	{
		//move element i up to it's correct position
		if (position != 1)
		{
			int p = (position / 2);

			if (servers[p].getEfficiency() > servers[position].getEfficiency())
			{
				swap(p, position);
				siftUp(p);
			}
		}
	}
	
	void swap(int origPos, int newPos)
	{
		Server placeHolder = servers[newPos];
		servers[newPos] = servers[origPos];
		servers[origPos] = placeHolder;
	}


	void displayServers()
	{
		for (int i = 1; i <= rear; i++)
		{
			printf("%-14d %-12d %-12d \n", i, servers[i].getCustsServed(), (totalServingTime - servers[i].getTotalServiceTime()));
		}
	}

	void setBusy(int servPosition, double serviceTime)
	{
		servers[servPosition].serveCustomer(serviceTime);
	}

	void setIdle(int servPosition)
	{
		servers[servPosition].finishServing();
	}

	double getEfficiency(int servPosition)
	{
		return servers[servPosition].getEfficiency();
	}

	int getFastestServer()
	{
		for (int i = 1; i <= rear; i++)
		{
			// Search for fastest idle server
			if (!servers[i].isBusy())
			{
				return i;
			}
		}
		// -1 means all servers are busy, otherwise returns position of fastest server
		return -1;
	}
};

// Initializer List
const int CustomerQueue::maxQ = 500;
const int EventQueue::capacity = 100;
const int ServerArray::maxServers = 20;



int main()
{

	// Array classes
	CustomerQueue custQ;
	EventQueue evntQ;
	ServerArray servers;

	double crntTime;		// Keep track of time
	double servEffic;		// Server Efficiency
	double arrT;			// Customer arrival time
	double tallT;			// Customer tally time
	bool paym;				// Customer payment method ( true = cash, false = card )
	string payString;		// holds payment info per cust
	double firstServedTime;
	double lastServedTime;

	ifstream inFile;
	inFile.open("ass2.txt");

	if (inFile.is_open())
	{
		// Amount of servers
		inFile >> servCount;
		for (int i = 0; i < servCount; i++)
		{
			inFile >> servEffic;
			Server newServ(servEffic);
			servers.addServer(newServ);
		}
		
		// Read first customer
		inFile >> arrT >> tallT >> payString;

		firstServedTime = arrT;
		// Cash or card
		if (payString.compare("card") == 0)
			paym = false;
		else
			paym = true;

		// Create customer object and add it to Event queue
		Customer newCus(arrT, tallT, paym);
		Event custArrival(newCus);
		evntQ.insertEvent(custArrival);

		while (!evntQ.isEmpty())
		{
			Event currentEvent = evntQ.deQueue();
			crntTime = currentEvent.getEventTime();
			totalEvents++;

			TotalCustQAvgLength += custQ.getLength();	// Take a measure of custQLength

			if (currentEvent.getEventType() == -1)		// Customer arrival event
			{
				int fastestServer = servers.getFastestServer();
				if (fastestServer != -1)
				{
					double serviceTime = (currentEvent.getTallyTime() * servers.getEfficiency(fastestServer)) + currentEvent.calcPaymentSpeed();
					servers.setBusy(fastestServer, serviceTime);		// Set to busy

					// Calculate server busy time, add to eventQueue w/ server position
					Event serverFinish(fastestServer, (serviceTime + crntTime));
					evntQ.insertEvent(serverFinish);
					customerCount++;	// Increment the total customers
					noQCus++;			// Customers who didnt have to wait
				}
				else
				{
					// Convert event to customer and add to custQueue
					Customer evntCust(currentEvent.getEventTime(), currentEvent.getTallyTime(), currentEvent.getPaymentMethod());
					evntCust.joinQueue(crntTime);	// Record when the cust joins the Queue
					custQ.enQueue(evntCust);
				}

				if (!inFile.eof())
				{
					// Read next customer
					inFile >> arrT >> tallT >> payString;

					// Cash or card
					if (payString.compare("card") == 0)
						paym = false;
					else
						paym = true;

					// Create customer object and add it to Event queue
					Customer newCus(arrT, tallT, paym);
					Event custArrival(newCus);
					evntQ.insertEvent(custArrival);
				}
			}
			else	// Server finish event
			{
				servers.setIdle(currentEvent.getEventType());	// Sets server to idle

				if (!custQ.isEmpty())
				{
					Customer nxtCus = custQ.deQueue();

					int fastestServer = servers.getFastestServer();
					if (fastestServer != -1)
					{
						// Calculate how long each customer spent in the queue and add it to total
						avgTimeSpentInQ += (crntTime - nxtCus.getTimeJoinedQueue());

						double serviceTime = (currentEvent.getTallyTime() * servers.getEfficiency(fastestServer)) + currentEvent.calcPaymentSpeed();
						servers.setBusy(fastestServer, serviceTime);		// Set to busy

						// Calculate server busy time, add to eventQueue w/ server position
						Event serverFinish(fastestServer, (serviceTime + crntTime));
						evntQ.insertEvent(serverFinish);
						customerCount++;	// Increment the total customers
					}
				}
			}
		}
		// Find the time of the last server finishes serving.
		lastServedTime = crntTime;
	}
	else
	{
		cout << "File could not be opened" << endl;
	}
	inFile.close();


	// Post simulation equations
	TotalCustQAvgLength = (TotalCustQAvgLength / totalEvents);
	avgTimeSpentInQ = (avgTimeSpentInQ / customerCount);
	totalServingTime = lastServedTime - firstServedTime;


	// PRINT STATS_____-----*****
	cout << "\n		+  Discrete Simulator Output  + " << endl;
	cout << "---------------------------------------------------------------\n" << endl;
	printf("%-42s %d \n", "Total Customers Served: ", customerCount);
	printf("%-42s %d %s \n", "Total Service Time (All Customers): ", totalServingTime, "mins");
	printf("%-42s %d %s \n", "Longest Customer Queue: ", longestCustQ, "Customers");
	printf("%-42s %.2f %s \n", "Customer Queue Average Length:", TotalCustQAvgLength, "Customers");
	printf("%-42s %.2f %s \n", "Average time spent by Customer in Queue:", avgTimeSpentInQ, "secs");
	printf("%-42s %.2f%s \n", "Percent of Customers who didnt Queue:", ((noQCus / customerCount) * 100), "%");

	cout << "\n   ~Server details~" << endl;

	printf("%-14s %-12s %-12s \n", "Server", "Amt Served", "Time idle(secs)");
	servers.displayServers();

}


/*

Student: lm189
Name: Luke Masliah

-Data Structures and Algorithms-

Server Queue = Dynamic Array of Server Object with linear search used
Customer Queue - Dynamic Array of Customer object with insertion sort
Event Queue - Dynamic array of Event object. Basic priority queue structure

Enhancements Made:
Customer Queue - Implemented a circular queue
Event Queue - Implemented a heap structure w/ siftDown & siftUp
ServerQueue - Implement a heap to organise servers efficiency

*/


