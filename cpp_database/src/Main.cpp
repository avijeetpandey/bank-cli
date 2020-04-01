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
auto coll = client[db_name][collection_name];

struct Person {
	std::string name;
	std::string gender;
	std::string mobile;
	std::string nationality;
	std::string city;
	std::string postal_code;
};

class Account {
	int ac_no;
	double account_balance;
	std::string IFSC_code;
	Person P;
	int transactions;

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
	void showAccountBalance();
	void createAccount();
	void getDetails();
	void saveDetails();
	void deposit();
	void withdraw();
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

	//making the document to be saved
	auto builder = bsoncxx::builder::stream::document{};
	bsoncxx::document::value doc_value = builder
		<< "ac_no" << ac_no
		<< "IFSC" << IFSC_code
		<<"balance"<<account_balance
		<< "name" << P.name
		<< "gender" << P.gender
		<< "mobile" << P.mobile
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


void Account::showAccountBalance() {
	std::cout << "Account balance  : " << account_balance << endl;
}


//deposit method
void Account::deposit() {
	double money;
	std::cout << "Enter amount : ";
	std::cin >> money;
	account_balance += money;

	std::cout << "New Money is : " << account_balance << endl;
	//getting the access to the collection and updating the account balance
   	coll.update_one(
		bsoncxx::builder::stream::document{}<<"ac_no"<<ac_no
		<<finalize,
		bsoncxx::builder::stream::document{}<<"$set"
		<<open_document
			<<"balance"<<account_balance
		<<close_document<<finalize
	);

	std::cout <<money<< " deposited" << endl;

}

//withdraw method
void Account::withdraw() {
	double money;
	std::cout << "Enter amount : ";
	std::cin >> money;
	if (money > account_balance) {
		std::cout << "Cannot Process , Insufficient funds " << endl;
		return;
	}

	account_balance -= money;

	//updating the withdrawl to the database
	coll.update_one(
		bsoncxx::builder::stream::document{}<<"ac_no"<<ac_no
		<<finalize,
		bsoncxx::builder::stream::document{}
			<<"$set"
			<<open_document
			<<"balance"<<account_balance
			<<close_document<<finalize
	);

	std::cout << money<<"  Withdrawn " << endl;
}

bool moneyTransfer(long ac_no, double money) {
	return false;
}

//driver code
int main() {

	Account  P1;
    P1.createAccount();
	P1.deposit();
	P1.deposit();
	P1.showAccountBalance();
	P1.withdraw();
	P1.withdraw();
	P1.showAccountBalance();

	return 0;
}