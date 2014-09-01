/* Loan Payoff Calculator
** Daniel Lilley [2014.01.27]
** ! To check totals if you split money between loans, edit loan balances accordingly.
*/

#include <iostream>
#include <sstream>
#include <windows.h>

// Hold data pertaining to an individual loan
struct loan{
    std::string loan_name;
    double loan_debt=-1;
    double loan_interest=-1;
    double loan_payment=-1;
};

void printInvalid(){
    std::cout << "Invalid input." << std::endl;
}

// Return true or false from Y or N - 0 = fail, 1 = true, 2 = false
int boolCatch(){
    std::string line;
    std::getline(std::cin, line);
    if(line.size()!=1){
        return 0;
    }
    if(line[0]=='y'||line[0]=='Y'){
        return 1;
    }
    else if(line[0]=='n'||line[0]=='N'){
        return 2;
    }
    else{
        return 0;
    }
}

// Return input as a recognizable double
std::string stringCatch(){
    std::string line;
    std::getline(std::cin, line);
    return line;
}

// Return input as a recognizable double -- checks for successful casting
double dNumberCatch(){
    std::string line;
    double dline=0;
    std::getline(std::cin, line);
    std::stringstream numberConvert(line);
    if(!(numberConvert>>dline)){
        dline=-1;
    }
    return dline;
}#include <windows.h>

// Return input as a recognizable integer -- checks for successful casting
int iNumberCatch(){
    std::string line;#include <windows.h>
    int iline=0;
    std::getline(std::cin, line);
    std::stringstream numberConvert(line);
    if(!(numberConvert>>iline)){
        iline=-1;
    }
    return iline;
}

// Display the command menu and return a valid command
int printMenu(){
    std::cout << "\n1  Calculate Total (Full)" << std::endl;
    std::cout << "2  Calculate Total (Early Payoff)" << std::endl;
    std::cout << "3  Edit Loan" << std::endl;
    std::cout << "0  Exit" << std::endl;
    int iline=-1;
    while(iline<0||iline>3){
        std::cout << "Enter command: ";
        iline=iNumberCatch();
        if(iline<0||iline>3){
            printInvalid();
        }
    }
    return iline;
}

// Return the total remaining to be paid on a given loan
double loanTotal(loan* ploan){
    double loan_value=ploan->loan_debt;
    double loan_payment=0;
    while(loan_value>0){
        loan_value+=loan_value*(ploan->loan_interest/100)/12;
        if(loan_value>=ploan->loan_payment){
            loan_value-=ploan->loan_payment;
            loan_payment+=ploan->loan_payment;
        }
        else{
            loan_payment+=loan_value;
            loan_value=0;
        }
    }
    return loan_payment;
}

void loanEdit(loan* ploan, int loan_number){
   // Input name of loan
    std::cout << "Name of loan #" << loan_number << ": ";
    ploan->loan_name=stringCatch();
    // Input value of loan
    while(ploan->loan_debt<0){
        std::cout << "Value of loan #" << loan_number << ": ";
        ploan->loan_debt=dNumberCatch();
        if(ploan->loan_debt<0){
            printInvalid();
        }
    }
    // Input interest on loan
    while(ploan->loan_interest<0){
        std::cout <<"Monthly interest (%) on loan #" << loan_number << ": ";
        ploan->loan_interest=dNumberCatch();
        if(ploan->loan_interest<0){
            printInvalid();
        }
    }
    // Input monthly payment on loan
    while(ploan->loan_payment<0){
        std::cout <<"Monthly payment on loan #" << loan_number << ": ";
        ploan->loan_payment=dNumberCatch();
        if(ploan->loan_payment<0){
            printInvalid();
        }
    }
}

int loanEditChoice(int loan_number){
    int edit=-1;
    while (edit<0||edit>loan_number){
        std::cout << "Edit which loan? (0 to cancel): ";
        edit=iNumberCatch();
        if(edit<0||edit>loan_number){
            printInvalid();
        }
        else{
            return edit;
        }
    }
}

int main()
{
    int command;
    int loan_number=0;

    SetConsoleTitle("Loan Payoff Calculator");
    std::cout << "Loan Payoff Calculator\nDaniel Lilley 2014.01.27\n" << std::endl;
// Determine the number of loans
    while(loan_number<=0){
        std::cout << "Number of loans: ";
        loan_number=iNumberCatch();
        if(loan_number<0){
            printInvalid();
        }
        else if(loan_number==0){
            std::cout << "Enter at least 1 loan." << std::endl;
        }
    }
// Allocate the correct number of loans
    loan* loans;
    loans=new loan[loan_number];

// Feed data to each loan
    for(int i=0;i<loan_number;i++){
        loanEdit(&loans[i], i+1);
    }

// Poll commands
    while(1){
        command=printMenu();
    // Calculations: 1 == Full repayment; 2 == Mark payments completed; 3 == Edit loan
        double* loan_total;
        loan_total=new double[loan_number];
        double full_payment=0;
        switch(command){
            case 0:{
// End program
                delete[] loan_total;
                delete[] loans;
                std::cout << "Good bye." << std::endl;
                return 0;
            }
            case 1:{
        // Full repayment
                std::cout << std::endl;
                for(int i=0;i<loan_number;i++){
                    loan_total[i]=loanTotal(&loans[i]);
                    std::cout << "Loan #" << loans[i].loan_name << ": $" << loan_total[i] << std::endl;
                    full_payment+=loan_total[i];
                }
                std::cout << "Total payment: $" << full_payment << std::endl;
                delete[] loan_total;
                break;
            }
            case 2:{
        // Mark repayments
                bool* paid_loans;
                paid_loans=new bool[loan_number];
                std::string input;
                int bool_check;
                std::cout << std::endl;
        // Catch loans fully paid
        // 0 = fail, 1 = true, 2 = false
                for(int i=0;i<loan_number;i++){
                    std::cout << "Fully pay loan #" << i+1 << "(" << loans[i].loan_name << ") (Y/N): ";
                    bool_check=boolCatch();
                    if(bool_check==1){
                        paid_loans[i]=true;
                    }
                    else if(bool_check==2){
                        paid_loans[i]=false;
                    }
                    else{
                        printInvalid();
                        i--;
                    }
                }
        // Calculate mixed full and immediate repayments
                for(int i=0;i<loan_number;i++){
                    if(paid_loans[i]==false){
                        loan_total[i]=loanTotal(&loans[i]);
                        full_payment+=loanTotal(&loans[i]);
                    }
                    else{
                        loan_total[i]=loans[i].loan_debt;
                        full_payment+=loans[i].loan_debt;
                    }
                    std::cout << "Loan #" << loans[i].loan_name << ": $" << loan_total[i] << std::endl;
                }
                std::cout << "Total payment: $" << full_payment << std::endl;
                delete[] paid_loans;
                delete[] loan_total;
                break;
            }
        // Allow loan editing
            case 3:{
                int loan_choice=loanEditChoice(loan_number);
                if(loan_choice==0){
                    delete[] loan_total;
                    break;
                }
                loans[loan_choice-1].loan_debt=-1;
                loans[loan_choice-1].loan_interest=-1;
                loans[loan_choice-1].loan_payment=-1;
                loanEdit(&loans[loan_choice-1], loan_choice);
                delete[] loan_total;
                break;
            }
        }
    }
}
