#include <iostream>
#include <cstdio>
#include <unordered_map>

// sample Bank and CardInfo classes
struct CardInfo {
    std::string pin;  // PIN of this card
    std::unordered_map<std::string, int> accounts;  // account(s) on this card, key = account name, value = balance
};

class Bank {
    std::unordered_map<int, CardInfo> cards_; // key = card ID, value = cardInfo, store all card record for this bank
    public:
    
    Bank(const std::unordered_map<int, CardInfo>& cards) : cards_(cards) {};

    bool cardExists(int card_num) {  // check if the card exists
        return cards_.count(card_num);
    }

    bool isPinCorrect(int card_num, std::string pin, CardInfo*& active_card) { // check if the PIN is correct
        if (cardExists(card_num)) {
            active_card = &(cards_.at(card_num));
            return active_card->pin == pin;
        }
        return false;
    }
};

class ATMController {
    Bank* bank_ = nullptr;
    CardInfo* active_card_ = nullptr;
    int* account_balance_ = nullptr;
    int card_num_ = -1;
    std::string pin_;
    
    bool has_card_ = false;
    bool is_pin_valid_ = false;
    bool account_exists_ = false; 

    public:
        ATMController(Bank* bank) {
            if (bank != nullptr) {
                bank_ = bank;
            }
        }

        void insertCard(int card_num) {
            if (has_card_) {
                std::cout << "Do not insert multiple card at same time\n";
            } else {
                if (bank_->cardExists(card_num)) {
                    card_num_ = card_num;
                    has_card_ = true;
                    std::cout << "Card inserted successfully!\n";
                } else {
                    std::cout << "ERROR: card number does not exist in bank database.\n";
                }
            }
        }

        void removeCard() {
            if (has_card_) {
                has_card_ = false;
                is_pin_valid_ = false;
                account_exists_ = false;
                account_balance_ = nullptr;
                active_card_ = nullptr;
                card_num_ = -1;
                std::cout << "Card removed successfully!\n";
            }
        }

        void enterPin(const std::string& pin) {
            if (has_card_) {
                if (bank_->isPinCorrect(card_num_, pin, active_card_)) {
                    pin_ = pin;
                    is_pin_valid_ = true;
                    std::cout << "PIN Correct!\n";
                    std::cout << "Existing accounts: ";  // print out all the existing accounts
                    for (auto const& pair : active_card_->accounts) {
                        std::cout << pair.first << " ";
                    }
                    std::cout << '\n';
                } else {
                    std::cout << "ERROR: PIN entered is not correct.\n";
                } 
            }
        }

        void selectAccount(const std::string& account_name) {
            if (is_pin_valid_) {
                auto it = active_card_->accounts.find(account_name);
                if (it != active_card_->accounts.end()) {
                    account_exists_ = true;
                    account_balance_ = &(it->second);
                    std::cout << "Selected account with name \"" << it->first << "\" and balance " << *account_balance_ << "\n";
                } else {
                    std::cout << "ERROR: account does not exist.\n";
                }
            } else {
                std::cout << "ERROR: must enter PIN before accessing the accounts\n";
            }
        }

        int seeBalance() {
            if (account_exists_) {
                return *account_balance_;
            }
            std::cout << "ERROR: no account has been selected\n";
            return -1;
        }

        void withdraw(int amount, int& returned_amount) {
            if (account_exists_) {
                if (*account_balance_ < amount) {
                    std::cout << "ERROR: insufficient funds, drop out all current balance\n";
                    returned_amount = *account_balance_;
                    *account_balance_ = 0;
                    return;
                }
                *account_balance_ -= amount;
                returned_amount = amount;
                return;
            }
            std::cout << "ERROR: no account has been selected\n";
        }

        void deposit(int amount) {
            if (account_exists_) {
                *account_balance_ += amount;
                return;
            }
            std::cout << "ERROR: no account has been selected\n";
        }

};