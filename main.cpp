#include "ATM_controller.h"

using namespace std;

int main() {
    // Create the cardInfo objects
    CardInfo c1 = {"1234", {{"main", 2000}, {"second", 150}}};
    CardInfo c2 = {"9999", {{"main", 10000}}};

    // Next, we create Bank object with key-value pairs
    unordered_map<int, CardInfo> customers{{123123123, c1}, {123456789, c2}};
    Bank* bank = new Bank(customers);

    // Now we can create our ATMController object
    ATMController controller_test(bank);

    // test invaild/vaild card number
    int card_num1 = 123123123, card_num2 = 12131;
    controller_test.insertCard(card_num2);  // the cardnum does not exist, Error
    controller_test.insertCard(card_num1);
    controller_test.insertCard(card_num1); // insert another card while transcation of current card is not finished, Error

    //test invaild/vaild PIN
    controller_test.enterPin("0000"); // PIN is incorrect, should not work
    controller_test.enterPin("1234"); // ok
    
    // test invaild/vaild account name
    controller_test.selectAccount("wrong_name"); // should not work, account name not valid
    controller_test.selectAccount("second"); // should work, valid name
    
    cout << "current balance: " << controller_test.seeBalance() << "\n";
    
    // test withdraw/deposit/show balance
    int returned_amount = 0;
    controller_test.withdraw(200, returned_amount); // try to withdraw more money than there is, withdraw the current amount
    
    int amount_to_deposit = 200;
    controller_test.deposit(amount_to_deposit);
    cout << "deposited " << amount_to_deposit << " dollars\n";
    cout << "new balance: " << controller_test.seeBalance() << "\n";

    controller_test.withdraw(200, returned_amount); // ok
    cout << "withdrew " << 200 << " dollars\n";
    cout << "new balance: " << controller_test.seeBalance() << "\n";

    controller_test.removeCard();
    return 0;
}
