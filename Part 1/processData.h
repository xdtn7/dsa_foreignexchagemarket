/*-------------------------------------------------
PROJECT: FOREIGN EXCHANGE MARKET
NAME: NGUYEN DUY TINH
Student ID: 1852797
Data Structure and Algorithms - Computer Enginering
Ho Chi Minh University of Technology
--------------------------------------------------*/

#include "main.h"

template <typename T> class Link {
public:
	T ele; // Value for this node 
	Link* next; // Pointer to next node in list // Constructors 
	Link(const T& elemval, Link* nextval = NULL) { ele = elemval; next = nextval; }
	Link(Link* nextval = NULL) { next = nextval; }
};
template <typename T>
class List {
public:	// Return: The number of elements in the list.
	List() {};
	virtual ~List() {};
	virtual int length() const = 0;
	virtual void insert(const T& it) = 0;
	virtual void append(const T& it) = 0;
	virtual void moveToStart() = 0;
	virtual void moveToEnd() = 0;
	virtual int currPos() const = 0;
	virtual void next() = 0;
	virtual void next1() = 0;
	virtual void prev() = 0;
	virtual T& getValue() const = 0;
	virtual T& getValueNext() const = 0;
	virtual void insert0() = 0;
	virtual void insertN() = 0;
	virtual void remove() = 0;
	virtual void moveToPos(int pos) = 0;
	virtual bool isPreLast() = 0;
	virtual void append0() = 0;
	virtual bool isLast() = 0;
	virtual bool is2PreLast() = 0;
	virtual void remove1() = 0;
	// virtual T removeM() = 0;

};
template <typename T>
class LList : public List<T> {
public:
	Link<T>* head;
	Link<T>* last;
	Link<T>* curr;
	int cnt;

	LList() { head = last = curr = new Link<T>(); }
	~LList() {
		while (head != NULL) {
			curr = head;
			head = head->next;
			delete curr;
		}
	}
	int length() const { return cnt; }
	void insert(const T& ele)
	{
		curr->next = new Link<T>(ele, curr->next);
	}
	void insert(T& it) {
		curr->next = new Link<T>(it, curr->next); if (last == curr) last = curr->next; // New tail
		cnt++;
	}
	void append(const T& it) { // Append "it" to list 
		last = last->next = new Link<T>(it, NULL);
		cnt++;
	}
	void append0() { // Append "it" to list 
		last = last->next = new Link<T>(NULL);
		curr = curr->next;
		cnt++;
	}
	void moveToStart() { curr = head; } // Place curr at list start 
	void moveToEnd() { curr = last; } // Place curr at list end 
	int currPos() const {
		Link<T>* temp = head;
		int i;
		for (i = 0; curr != temp; i++) temp = temp->next; delete temp; return i;
	}
	void next() { if (curr != last) curr = curr->next;}
	void next1() { if (curr->next != last) curr = curr->next; }
	T& getValue() const { // Return current element 
		if (curr->next != NULL)// assert(curr->next != NULL, "No value");
			return curr->next->ele;
		else return curr->ele;
	}
	void prev() {
		if (curr == head) return; // No previous element
		Link<T>* temp = head;
		// March down list until we find the previous element
		while (temp->next != curr) temp = temp->next;
		curr = temp;
		delete temp;
	}
	bool isPreLast() {
		if (last == curr->next || last == curr) return 1;
		else return 0;
		}
	bool is2PreLast() {
		if (last == curr->next->next || last == curr->next || last == curr) return 1;
		else return 0;
	}
		
	

	T& getValueNext() const { // Return current element //dang khong xai
		if (curr->next->next != NULL)// assert(curr->next != NULL, "No value");
			return curr->next->next->ele;
		else return curr->next->ele;
	}

	void insert0() {
		curr->next = new Link<T>(NULL); if (last == curr) last = curr->next; // New tail
		cnt++;
	}
	void insertN() {
		curr->next = new Link<T>(curr->next); if (last == curr) last = curr->next; // New tail
		cnt++;
	}
	void remove() {
		//T ele = curr->next->ele; // Remember value
		//Link<T>* ltemp = curr->next; // Remember link node
		if (curr->next == last) {
			last = curr;
			curr->next = NULL;
		} // Reset tail
		else if(curr->next != NULL) {
			Link<T>* temp = curr->next;
			curr->next = curr->next->next;
			delete temp;
		} // Remove from list
		//delete ltemp; // Reclaim space
		cnt--; // Decrement the count
		//return ele;
	}
	void remove1() {
		//T ele = curr->next->ele; // Remember value
		//Link<T>* ltemp = curr->next; // Remember link node
		delete curr;
		delete last;
		delete head;

		// Remove from list
		//delete ltemp; // Reclaim space
		 // Decrement the count
		//return ele;
	}

	bool isLast() {
		if (last == curr) return 1; else return 0;
	}
	void moveToPos(int pos) {
		// assert((pos >= 0) && (pos <= cnt), "Position out of range");
		curr = head;
		for (int i = 0; i < pos; i++) curr = curr->next;
	}
	
	//T removeM() {
	//	T ele = curr->next->ele; // Remember value
	//	Link<T>* ltemp = curr->next; // Remember link node
	//	if (last == curr->next) curr->next = ; // Reset tail
	//	curr->next = curr->next->next; // Remove from list
	//	delete ltemp; // Reclaim space
	//	cnt--; // Decrement the count
	//	return ele;
	//}

};



class ProcessData {
private:

	class Exchange {
	public:
		string BC, QC;

		class Candle {
		public:
			int TIME;
			float OP, HP, LP, CP;
			Candle();
			/*
			Candle(string TIME, string OP, string HP, string LP, string CP) {
			this->TIME = TIME;
			this->OP = OP;
			this->HP = HP;
			this->LP = LP;
			this->CP = CP;
			}*/
			~Candle();
			

			

		};

		List<Candle>* can;

		Exchange();
		~Exchange();
		/*
		void insertcandle(const Candle& candle) {
		int x = 0;
		if (can == nullptr) {
		can = new LList<Candle>();
		}
		for (can->moveToStart(); can->currPos() < can->length(); can->next()) {
		if (can->getValue().TIME != candle.TIME) {
		x++;
		}
		}
		if (x == can->length()){
		can->append(candle);
		}
		} */

	};

	List<Exchange>* data; //Pointer of list
	static const int MAXSIZECODE = 8;
	static int split(string line, string*& sp);

public:
	ProcessData();
	~ProcessData();
	int process(string line);
	int insert(const string* sp, const int n);
	int del(const string* sp, const int n);
	int up(const string* sp, const int n);
	int mb(const string* sp, const int n);
	int st(const string* sp, const int n);
	int dj(const string* sp, const int n);
	int eg(const string* sp, const int n);
	///*int ismb(const double OP0, const double HP0, const double LP0, const double CP0) {
	//	int static countC;
	//	double OP = OP0, HP = HP0, LP = LP0, CP = CP0;
	//	if (OP == HP) {
	//		if ((CP == LP) && (10000 * OP- 10000* CP) > 0.5001) {
	//			countC++;
	//		}
	//	}
	//	else if (CP == HP) {
	//		if ((OP == LP) && (10000 * CP - 10000*OP) > 0.5001) {
	//			countC++;
	//		}
	//	}
	//	else return 0;
	//	return countC;
	//}*/
};
