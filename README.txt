Admin
Press 1 to add an account
Press 2 to remove an account
Press 3 to modify an account
Press 4 to logout
Press 5 to save the changes

Login credentials for admin are admin and admin.

Operations 1 and 2 will take effect and be updated in the database only if 5 is manually pressed in the end.
Operation 3 is to modify the user name.It will be autosaved.(If necessary file creation does not occur, then press 5 again to
enforce these changes)

Normal User
Press 1 to deposit
Press 2 to withdraw
Press 3 for balance enquiry
Press 4 to view details
Press 5 to change password
Press 6 to exit

The above operations are self explanatory.

Joint user
flock is used for locking. For operations 1 and 2, a write lock is set for both the balance.txt and transactions.txt.
For operations, 3 and 4, a read lock is set.

User_data contains subdirectories where the name of each subdirectory is the user name. Each of those subdirectories will
contain 3 files. There will be balance.txt which contains the current balance. There will be a transactions.txt which
will contain the history of transactions.Lastly, there is also a settings.txt which contains the user name and password of
that respective account.
WARNING: Do not open the user data directory during the execution of the program. Any changes made to the file pointers can
affect the results of the system.

admin.h contains the necessary helper functions that are used for the realization of the use cases for the admin.

login.h is used to for ensuring password protected login credentials.

transmit.h contains helper functions for reading and writing string data between server and client
without expicitly having to worry about the size of the string data that has to be passed as the parameter to
read and write library functions.

accounts.txt acts as a database of user accounts and password. It's useful for ensuring password protected login access to
the server.

Compilation
gcc server.c -o server
gcc client.c -o client

Run ./server in one terminal and ./client in another.
