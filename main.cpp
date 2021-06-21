#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class loginManager
{
    public:
        loginManager()
        {
            accessGranted = false;
        }

        /**********************************************************************
        * login() handles the the login process for existing username and
        * passwords.
        **********************************************************************/
        void login()
        {
            cout << "Enter your username and password.\n";
            cout << "Username: ";
            cin >> userNameAttempt;
            int userID = checkFile(userNameAttempt, "user.txt");
            if(userID != 0)
            {
                cout << "Password: ";
                cin >> passWordAttempt;
                int passID = checkFile(passWordAttempt, "password.txt");
                if(userID == passID)
                {
                    cout << "\nYou are Logged in.\n" << endl;
                }
                else
                {
                    cout << "\nIncorrect password. Try again.\n" << endl;
                    login();
                }
            }
            else
            {
                cout << "\nUsername does not exist. Try again.\n" << endl;
                login();
            }
        }

        /**********************************************************************
        * addUser() handles the collection of new users and stores the data
        * in .txt files for usernames and passwords.
        **********************************************************************/
        void addUser()
        {
            cout << "What will your username be?\n";
            cout << "Username: ";
            cin >> username;
            if(checkFile(username, "user.txt") != 0)
            {
                cout << "This username is unavailable. Try another one." << endl;
                return;
            }

            cout << "What will your password be?\n";
            cout << "Password: ";
            cin >> password;
            int id = 1 + getLastID();
            saveFile(username, "user.txt", id);
            saveFile(password, "password.txt", id);
        }

        /**********************************************************************
        * getLastID() ensures that user credentials are given a unique #ID in
        * the .txt files
        **********************************************************************/
        int getLastID()
        {
            fstream file;
            file.open("user.txt", ios::in);
            file.seekg(0, ios::end);

            if(file.tellg() == -1)
            {
                return 0;
            }

            string hash;

            for(int i = -1; hash.find("#") == string::npos; i--)
            {
                file.seekg(i, ios::end);
                file >> hash;
            }

            file.close();
            hash.erase(0, 4);
            int id;
            istringstream(hash) >> id;

            return id;
        }

        /**********************************************************************
        * checkFile() checks for the ID number to help addUser() with making
        * each new user puts in a unique username and it helps login() with
        * matching username ID to its corresponding password ID.
        **********************************************************************/
        int checkFile(string attempt, const char* p_fileName)
        {
            string line;
            fstream file;
            string currentChar;
            int encrytedChar;
            file.open(p_fileName, ios::in);

            while(1)
            {
                file >> currentChar;
                if(currentChar.find("#ID:") != string::npos)
                {
                    if(attempt == line)
                    {
                        file.close();
                        currentChar.erase(0, 4);
                        int id;
                        istringstream(currentChar) >> id;
                        return id;
                    }
                    else
                    {
                        line.erase(line.begin(), line.end());
                    }
                }
                else
                {
                    istringstream(currentChar) >> encrytedChar;
                    line += (char)decrypt(encrytedChar);
                    currentChar = "";
                }
                if(file.peek() == EOF)
                {
                    file.close();
                    return 0;
                }
            }
        }

        /**********************************************************************
        * saveFile() saves data to .txt files
        **********************************************************************/
        void saveFile(string p_line, const char* p_fileName, const int& p_id)
        {
            fstream file;
            file.open(p_fileName, ios::app);
            file.seekg(0, ios::end);

            if(file.tellg() != 0)
            {
                file << "\n";
            }

            file.seekg(0, ios::beg);

            for(int i = 0; i < p_line.length(); i++)
            {
                file << encrypt(p_line[i]);
                file << "\n";
            }

            file << "#ID:" << p_id;
            file.close();
        }

        /**********************************************************************
        * encrypt() very light encryption to prevent hacking.
        **********************************************************************/
        int encrypt(int p_letter)
        {
            return p_letter + 3;
        }

        /**********************************************************************
        * decrypt() used to reverse the encryption process.
        **********************************************************************/
        int decrypt(int p_letter)
        {
            return p_letter - 3;
        }

    private:
        string username;
        string password;
        string userNameAttempt;
        string passWordAttempt;
        bool accessGranted;
};

/******************************************************************************
* main() drives the whole program and creates usable objects of the
* loginManager class.
******************************************************************************/
int main()
{
    loginManager app;
    int key;
    while(key != 3)
    {
        cout << "Options:\n 1 - Add new user\n 2 - Login\n 3 - Quit program\n";
        cout << "Enter: ";
        cin >> key;

        if(key < 1 || key > 3)
        {
            cout << "\nOops!  That's not an option.  Try again.\n";
            cout << "Options:\n 1 - Add new user\n 2 - Login\n 3 - Quit program\n";
            cout << "Enter: ";
            cin >> key;
        }
        else if(key == 1)
        {
            app.addUser();
        }
        else if(key == 2)
        {
            app.login();
        }
    }

    cin.get();
}
