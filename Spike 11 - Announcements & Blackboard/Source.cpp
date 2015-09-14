#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
using namespace std;



template <class T> class Message{
private:
	T message;
	vector<int> subscribers;
public:
	void packMessage(T);
	T unpackMessage(int);
	int registerSubscriber(int);
};
template<class T> void Message<T>::packMessage(T gv){
	message = gv;
}
template<class T> T Message<T>::unpackMessage(int sub_id){
	//search subscribers for id
	for (int i : subscribers){
		if (i == sub_id)
			//object has registered id
			return message;
	}
	//object doesn't have registered id
	return "Not a registered subscriber";
}
template<class T> int Message<T>::registerSubscriber(int subscriber){
	//generate a subscriber id and store it
	subscriber = rand();
	subscribers.push_back(subscriber);

	return subscriber;
}


//blackboard
template <class T> class StoreMessages{
private:
	vector<Message<T>> messages;
public:
	void saveMessage(Message<T> m){ messages.push_back(m); }
};



//sender
template <class T> class Sender{
private:
	T datafield;
public:
	T sendStuff();
};
template<class T> T Sender<T>::sendStuff(){
	datafield = "A message to all subscribed objects";
	return datafield;
}



//registered
template <class T> class Component1{
private:
	T datafield;
	int subscriber_no;
public:
	void OutputResult();
	void receiveMessage(Message<T>, int);
	int getSubscriber(){ return subscriber_no; }
	void setSubscriber(int s){ subscriber_no = s; }
};
template<class T> void Component1<T>::OutputResult(){
	cout << "Component 1 received a message: " << datafield << endl;
}
template<class T> void Component1<T>::receiveMessage(Message<T> gm, int sub_id){
	datafield = gm.unpackMessage(sub_id);
}

template <class T> class Component3{
private:
	T datafield;
	int subscriber_no;
public:
	void OutputResult();
	void receiveMessage(Message<T>, int);
	int getSubscriber(){ return subscriber_no; }
	void setSubscriber(int s){ subscriber_no = s; }
};
template<class T> void Component3<T>::OutputResult(){
	cout << "Component 3 received a message: " << datafield << endl;
}
template<class T> void Component3<T>::receiveMessage(Message<T> gm, int sub_id){
	datafield = gm.unpackMessage(sub_id);
}



//not registered
template <class T> class Component2{
private:
	T datafield;
	int subscriber_no;
public:
	void OutputResult();
	void receiveMessage(Message<T>, int);
	int getSubscriber(){ return subscriber_no; }
	void setSubscriber(int s){ subscriber_no = s; }
};
template<class T> void Component2<T>::OutputResult(){
	cout << "Component 2 received a message: " << datafield << endl;
}
template<class T> void Component2<T>::receiveMessage(Message<T> gm, int sub_id){
	datafield = gm.unpackMessage(sub_id);
}


int main(){
	StoreMessages<string> b;

	Sender<string> s;
	string message = s.sendStuff();
	//string message = "default message";

	//example objects
	Component1<string> c1;
	Component2<string> c2;
	Component3<string> c3;

	//the object to transport the data
	Message<string> m;

	//register subscribers
	//only registered subscribers will receive the message
	int sub_no1 = m.registerSubscriber(c1.getSubscriber());
	c1.setSubscriber(sub_no1);
	int sub_no2 = m.registerSubscriber(c3.getSubscriber());
	c3.setSubscriber(sub_no2);

	//pack the message
	m.packMessage(message);

	//receive a message
	//pass the message data, the subscriber id
	c1.receiveMessage(m, c1.getSubscriber());
	c2.receiveMessage(m, c2.getSubscriber());
	c3.receiveMessage(m, c3.getSubscriber());

	//verify only subscribers receive the message
	c1.OutputResult();
	c2.OutputResult();	//not subscribed
	c3.OutputResult();

	//keep the message
	b.saveMessage(m);

	system("pause");
	return 0;
}