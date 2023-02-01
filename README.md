## Customers Debts Management System

This program is a customer management system that allows you to read customer data from a CSV file, display it in a list, and perform set and select operations on the data. also, the program can be compile to perform these actions from a client computer via sockets.

The program can be compiled in two ways. localy, where the user can send set select and print queries, or as a client machine which passing the queries to the same logic process.


# How to run

To run the system localy:

        1. `make local`
        2. `./local customers.txt`

To run it from client:

        A.  1. `make server` 
            2. `./server <port> customers.txt`
        B.  3. `make client` 
            4. `./client <port> <ip_address>'

Notes:
    1. only local running will show the user data about the file such as errors and how many lines got processed properly.
    2. customers.txt is an example of csv file with a few lines with errors to demonstrate how it works.
    3. use the same port number. in order to run both on the same machine just enter 127.0.0.1 .
    4. the server must be running before the client. make sure. 

# Operations

Once the program is running, you can perform the following operations:

- `select` - search in the list with select query.
- `set`    - update customer debt or creating a new one.
- `print`  - print the list of customers debts orderd by their size.
- `quit`   - exit the program.


# More details

    -   `select`
        write a select query in this format:
        `select <field><operator><value>`. for example: select debt<100.
        field can be any of the rows in the table (first and second name, id , phone , date, debt ).
        operator can be only one of this: <, >, !=, = .
        no spaces allowed before and after the operator.

    -   `set`
        write a set query in this format:
        `set first name=<value>,second name=<value>,id=<value>,phone=<value>,date=<value>,debt=<value>`
        you need to provide all the fields,with the correct value, and in the right format.
        no spaces are allowed before and after the operator and commas.
    
    -   `print
        will show the current list of debts in a table.