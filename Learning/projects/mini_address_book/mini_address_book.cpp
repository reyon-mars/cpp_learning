#include <iostream>
#include <array>
#include <string>

struct Contact{

    std::string name;
    int phone_number;
    std::string email;
    
    Contact(){
        this->name = "";
        this->phone_number = 0;
        this->email = "";
    }

    Contact( std::string name, int phone_number, std::string email){
        this->name = name;
        this->phone_number = phone_number;
        this->email = email;
    }
};

Contact& search_contact( const std::array<Contact, 10> contact_list, const std::string name ){
    
    for( int i = 0; i < 10; i++ ){
        if( contact_list[i].name == name ){
            return &contact_list[i];
        }else {
            std::cout << "The contact does not exist. " << std::endl;
            return Contact();
        }
    }
}

std::ostream& operator<<( std::ostream& out, const Contact& contact ){
    out << "Name: " << contact.name << "\n"
        << "Number: " << contact.phone_number << "\n"
        << "Email: " << contact.email << "\n";
    return out;
}

std::array<Contact, 10> contact_list;
int curr_arr_size(0);

int main(){
    std::cout << "Contact list" << "\n";
    
    while( true ){
        int option(0);
        std::cout << "Please select an option: " << "\n";
        std::cout << "1. Add contact" << "\t\t" << "2. Print contact list" << "\n"
                  << "3. Exit " << "\n";
        std::cin >> option;

        switch(option){
            case 1:
                if( curr_arr_size >= 10 ){
                    std::cout << "The list is full." << "\n";
                }else{
                    std::string name;
                    int phone_number(0);
                    std::string email;
                    std::cout << "Please enter the name: ";
                    std::cin >> name;
                    std::cout << "Please enter the number: ";
                    std::cin >> phone_number;
                    std::cout << "Please enter the email: ";
                    std::cin >> email;

                    Contact temp( name, phone_number, email );
                    contact_list[curr_arr_size] = temp;
                    curr_arr_size++;
                }
                break;

            case 2:
            if( contact_list.empty() ){
                std::cout << "The contact is empty" << "\n";
            }else{
                std::cout << "Contact list: " << "\n";
                for( int i = 0; i < contact_list.size(); i++ ){
                    std::cout << contact_list[i];
                }
            }
        
                break;

            case 3:
                return;

            default:
                std::cout << "Invalid option." << "\n";
        }

    }
}
