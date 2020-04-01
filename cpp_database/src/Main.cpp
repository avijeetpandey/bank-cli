//necessary header files
#include<iostream>
#include<string>
#include<random>
#include<cstring>
#define endl '\n';

//other libraries imports
#include<boost/filesystem.hpp>

//bson imports
#include<bsoncxx/json.hpp>
#include<bsoncxx/oid.hpp>
#include<bsoncxx/builder/stream/document.hpp>

//mongo imports
#include<mongocxx/client.hpp>
#include<mongocxx/stdx.hpp>
#include<mongocxx/uri.hpp>
#include<mongocxx/instance.hpp>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::finalize;
using mongocxx::cursor;


//db constructs
const std::string db_name = "bank";
const std::string collection_name = "account_details";

mongocxx::instance inst{}; //one and only instance of the database
mongocxx::client client{ mongocxx::uri{} };


struct Person {
	std::string name;
	char gender;
	std::string mobile;
	std::string nationality;
	std::string city;
	int postal_code;
};

class Account {
	int ac_no;
	double account_balance;
	std::string IFSC_code;
	Person P;

	//other variables
	std::string welcome = "Welcome to C++ Bank ";
	std::string goodBye = "Happy To Help You ";

public:
	//when a bank account opens
	Account() {
		account_balance = 0.00;
		IFSC_code = "SBIN000XX78";
	}

	void menu();
	int createAccountNumber();
	void createAccount();
	void getDetails();
	void saveDetails();
	bool deposit(double money);
	bool withdraw(double money);
	bool moneyTransfer(long ac_no, double money);

};

//menu for the bank class
void Account::menu() {

}

//create account
void Account::createAccount() {
	std::cout << "Thanks for choosing C++ bank " << endl;
	getDetails();
	ac_no = createAccountNumber();
	std::cout << "Your account number is : " << ac_no << endl;
	saveDetails();
}

int Account::createAccountNumber() {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(100000000, 500000000);
	int generatedNumber = dist(rng);
	return generatedNumber;
}

//getting  user details
void Account::getDetails() {
	std::cout << "=========================================" << endl;
	std::cout << "Please enter the following details : " << endl;
	std::cout << "=======================================" << endl endl;
	std::cout << "Personal Details : " << endl;
	std::cout << "Enter you Name : ";
	std::getline(std::cin,P.name);
	fflush(stdin);
	std::cout << "Enter your gender (M/F) : ";
	std::cin >> P.gender;
	std::cout << "Enter your mobile number : ";
	std::cin >> P.mobile;
	std::cout << endl;
	std::cout << "Address related details" << endl;
	std::cout << "Enter nationality : ";
	std::cin >> P.nationality;
	std::cout << "Enter city : ";
	std::cin >> P.city;
	std::cout << "postal code : ";
	std::cin >> P.postal_code;

}

//method to print user details
void Account::saveDetails() {

	//getting access to the specified collection
	auto coll = client[db_name][collection_name];
	//making the document to be saved
	auto builder = bsoncxx::builder::stream::document{};
	bsoncxx::document::value doc_value = builder
		<< "ac_no" << ac_no
		<< "IFSC" << IFSC_code
		<<"balance"<<account_balance
		<< "name" << P.name
		<< "mobile" << P.mobile
		<< "gender" << P.gender
		<< "address" << bsoncxx::builder::stream::open_document
		<< "city" << P.city
		<< "zip" << P.postal_code
		<< bsoncxx::builder::stream::close_document
		<< bsoncxx::builder::stream::finalize;

	//creating view for the document
	bsoncxx::document::view view = doc_value.view();

	//saving the details
	bsoncxx::stdx::optional<mongocxx::result::insert_one> result = coll.insert_one(view);
	std::cout << "operation succesfull" << endl;
}

//deposit method
bool Account::deposit(double money) {
	return false;
}

//withdraw method
bool Account::withdraw(double money) {
	return false;
}

bool moneyTransfer(long ac_no, double money) {
	return false;
}

//driver code
int main() {




	return 0;
}