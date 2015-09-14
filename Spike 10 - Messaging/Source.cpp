#include <string>
#include <iostream>
using namespace std;



template <class T> class Message{
private:
	T message;
public:
	void packMessage(T);
	T unpackMessage();
};
template<class T> void Message<T>::packMessage(T gv){
	message = gv;
}
template<class T> T Message<T>::unpackMessage(){
	return message;
}




template <class T> class Input{
private:
	T datafield;
public:
	T inputValues();
	void receiveMessage(Message<T>);
};

template<class T> T Input<T>::inputValues(){
	cout << "Enter some data to print: ";
	cin >> datafield;
	return datafield;
}
template<class T> void Input<T>::receiveMessage(Message<T> gm){
	datafield = gm.unpackMessage();
}



template <class T> class Output{
private:
	T datafield;
public:
	void outputResult();
	void receiveMessage(Message<T>);
};

template<class T> void Output<T>::outputResult(){
	cout << "The value entered was: " << datafield << endl;
}
template<class T> void Output<T>::receiveMessage(Message<T> gm){
	datafield = gm.unpackMessage();
}




int main(){
	int message;

	//setup objects to pass integers
	Input<int> intIn;
	Message<int> intMsg;
	Output<int> intOut;

	//the Input class receives input from the user
	message = intIn.inputValues();

	//pass the message
	//data from Input object is stored in the Message object
	intMsg.packMessage(message);

	//the Output class receives the Message object
	//outputs the received data
	intOut.receiveMessage(intMsg);
	intOut.outputResult();

	system("pause");
	return 0;
}