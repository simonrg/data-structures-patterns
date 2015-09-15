#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
using namespace std;



class Subscriber{
protected:
	string datafield;
public:
	virtual void OutputResult() = 0;
	void receiveMessage(string message){ datafield = message; }
};
template <class T> class Message{
private:
	T message;
public:
	void packMessage(T gv){ message = gv };
	T unpackMessage(){ return message; }
};







template <class T> class Blackboard{
private:
	vector<Message<T>> messages;
	Message<T> top_message;
	vector<Subscriber*> subscribers;
public:
	void saveMessage(Message<T> m){ messages.push_back(m); }
	void sendSubscriber(Subscriber*);
	void sendAll();
	void registerSubscriber(Subscriber*);
	void removeSubscriber(Subscriber*);
};
template<class T> void Blackboard<T>::sendSubscriber(Subscriber* s){
	top_message = messages.front();

	for (Subscriber* registered_sub : subscribers){
		if (s == registered_sub)
			s->receiveMessage(top_message.unpackMessage());
	}
	s->OutputResult();
}
template<class T> void Blackboard<T>::sendAll(){
	top_message = messages.front();

	for (Subscriber* registered_sub : subscribers){
		registered_sub->receiveMessage(top_message.unpackMessage());
		registered_sub->OutputResult();
	}
}
template<class T> void Blackboard<T>::registerSubscriber(Subscriber* s){
	subscribers.push_back(s);
}
template<class T> void Blackboard<T>::removeSubscriber(Subscriber* s){
	subscribers.erase(s);
}




class Component1 : public Subscriber{
public:
	void OutputResult(){ cout << "Component 1 messages: " << datafield << endl; }
};

class Component2 : public Subscriber{
public:
	void OutputResult() { cout << "Component 2 messages: " << datafield << endl; }
};

class Component3 : public Subscriber{
public:
	void OutputResult(){ cout << "Component 3 messages: " << datafield << endl; }
};




int main(){
	Blackboard<string> b;

	//message object
	Message<string> m;
	m.packMessage("A message to my subscribers!");

	//save message to blackboard
	b.saveMessage(m);


	//some subscribers
	Subscriber *sub1 = new Component1();
	Subscriber *sub2 = new Component2();
	Subscriber *sub3 = new Component3();

	//register subscribers
	//so they can receive messages
	b.registerSubscriber(sub1);
	b.registerSubscriber(sub2);
	b.registerSubscriber(sub3);


	//send a message to a specific subscriber
	cout << "Send to a specific subscriber" << endl;
	b.sendSubscriber(sub2);

	//send a message to all subscribers
	cout << endl << "Send to all subscribers" << endl;
	b.sendAll();

	system("pause");
	return 0;
}